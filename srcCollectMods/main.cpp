#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDirIterator>
#include <QFile>
#include <QRegularExpression>
#include <QStringList>
#include <QTextStream>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <variant>

#include "BinarizedSqmParser.h"
#include "TextualSqmParser.h"
#include "exceptions/FormatErrorException.h"

#include "pbo/Pbo.h"

#include "version.h"

QStringList scanDirectoryForPbos(QString const& dirName) {
	QStringList result;
	QDirIterator it(dirName, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
	while (it.hasNext()) {
		QString const path = it.next();
		if (path.endsWith(QStringLiteral(".pbo"), Qt::CaseInsensitive)) {
			result.append(path);
		}
	}
	return result;
}

bool pboHasFile(PBO::PBO& pbo, QByteArray const& filename) {
	return pbo.has_file_ignore_case(filename);
}

QByteArray loadFileFromPbo(QString const& pboFileName, PBO::PBO& pbo, QByteArray const& filename) {
	return pbo.read_file_ignore_case(filename, true);
}

QByteArray loadConfigFromPbo(QString const& pboFileName) {
	static QByteArray const searchFileNameBin = QStringLiteral("config.bin").toUtf8();
	static QByteArray const searchFileNameTxt = QStringLiteral("config.cpp").toUtf8();
	PBO::PBO pbo_file(pboFileName);

	pbo_file.unpack();

	if (pboHasFile(pbo_file, searchFileNameBin)) {
		QByteArray const configBin = loadFileFromPbo(pboFileName, pbo_file, searchFileNameBin);
		return configBin;
	} else if (pboHasFile(pbo_file, searchFileNameTxt)) {
		QByteArray configCpp = loadFileFromPbo(pboFileName, pbo_file, searchFileNameTxt);	
		static const QByteArray includeBytes = QStringLiteral("#include \"").toUtf8();

		int startOffset = 0;
		int pos = configCpp.indexOf(includeBytes, startOffset);
		while (pos != -1) {
			int const startPos = pos;
			// move to first "
			pos = configCpp.indexOf('"', pos);
			int posOfClosingQuote = pos + 1;
			while ((posOfClosingQuote < configCpp.size()) && (configCpp.at(posOfClosingQuote) != '"')) {
				++posOfClosingQuote;
			}
			if (posOfClosingQuote >= configCpp.size()) {
				throw zeusops::exceptions::FormatErrorException() << "Could not locate end of #include section in 'config.cpp' in PBO '" << pboFileName.toStdString() << "', is the archive complete?";
			}

			QByteArray const matched = configCpp.mid(pos + 1, posOfClosingQuote - pos - 1);
			std::cout << "Found include for '" << matched.toStdString() << "' (" << QString(matched.toHex()).toStdString() << "), following... " << std::endl;
			if (pboHasFile(pbo_file, matched)) {
				QByteArray const includeData = loadFileFromPbo(pboFileName, pbo_file, matched);
				configCpp.replace(startPos, posOfClosingQuote - startPos + 1, includeData);
				startOffset = 0;
			} else {
				std::cerr << "Warning: Could not resolve include '" << matched.toStdString() << "', ignoring!" << std::endl;
				startOffset = pos + 1;
			}

			pos = configCpp.indexOf(includeBytes, startOffset);
		}

		return configCpp;
	} else {
		throw zeusops::exceptions::FormatErrorException() << "Could not locate '" << searchFileNameBin.toStdString() << "' or '" << searchFileNameTxt.toStdString() << "' in PBO '" << pboFileName.toStdString() << "', is the archive complete?";
	}
}

QStringList extractModNamesFromPbo(QString const& pboFileName) {
	QByteArray configFile = loadConfigFromPbo(pboFileName);
	bool const isBinarized = BinarizedSqmParser::hasBinarizedSqmHeader(configFile);
	std::shared_ptr<SqmObjectList<SqmStructure>> sqmObjects;
	if (isBinarized) {
		BinarizedSqmParser sqmParser(true);
		sqmObjects = std::make_shared<SqmObjectList<SqmStructure>>(sqmParser.parse(configFile));
	} else {
		QTextStream stream(configFile);
		stream.setCodec("UTF-8");
		QString const missionFileData = stream.readAll();

		TextualSqmParser sqmParser(true);
		sqmParser.setQuitAfterCfgPatches(true);
		sqmObjects = std::make_shared<SqmObjectList<SqmStructure>>(sqmParser.parse(missionFileData));
	}

	bool const hasCfgPatches = sqmObjects->hasClassIgnoreCase(QStringLiteral("CfgPatches"));
	if (!hasCfgPatches) {
		throw zeusops::exceptions::FormatErrorException() << "Failed to parse file '" << pboFileName.toStdString() << "', class 'CfgPatches' could not be found!";
	}

	SqmObjectList<SqmClass> patchClasses = sqmObjects->getClassIgnoreCase(QStringLiteral("CfgPatches"))->onlyClasses();
	QStringList result;
	for (auto it = patchClasses.begin(), end = patchClasses.end(); it != end; ++it) {
		result.append((*it)->getName());
	}

	return result;
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

	QCoreApplication::setApplicationName("zeusSqmCollectMods");
	QCoreApplication::setApplicationVersion(QString::fromStdString(Version::versionWithTagString()));

	QCommandLineParser parser;
	parser.setApplicationDescription("ARMA3 mod name collector");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("targetFolder", QCoreApplication::translate("main", "Folder to search in for PBOs."));
	parser.addPositionalArgument("outputFile", QCoreApplication::translate("main", "File to write the mod names to."));

	// Process the actual command line arguments given by the user
	parser.process(app);

	QStringList args = parser.positionalArguments();
	if (args.size() < 2) {
		parser.showHelp(EXIT_FAILURE);
	}

	// HEADER
	std::cout << "ARMA3 mod name collector v" << Version::versionWithTagString() << std::endl;
	std::cout << std::endl;

	QStringList pboFiles = scanDirectoryForPbos(args.at(0));
	QStringList modNames;
	std::cout << "Found " << pboFiles.size() << " PBO files." << std::endl;

	std::size_t errorCount = 0;
	std::size_t successCount = 0;
	auto const t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < pboFiles.size(); ++i) {
		try {
			QStringList const subModNames = extractModNamesFromPbo(pboFiles.at(i));
			modNames.append(subModNames);
			for (int i = 0; i < subModNames.size(); ++i) {
				std::cout << "Found mod with name '" << subModNames.at(i).toStdString() << "'." << std::endl;
			}
			++successCount;
		} catch (zeusops::exceptions::FormatErrorExceptionImpl const& e) {
			// Ignore for now.
			++errorCount;
			std::cerr << "Ignoring file '" << pboFiles.at(i).toStdString() << "' due to parsing exception: " << e.what() << std::endl;
		}
	}
	auto const t2 = std::chrono::high_resolution_clock::now();
	auto const duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "Found a total of " << modNames.size() << " mods (successfully parsed " << successCount << " PBOs in " << duration << "ms, failed on " << errorCount << ")." << std::endl;

	modNames.sort(Qt::CaseInsensitive);

	QFile output(args.at(1));
	if (!output.open(QFile::WriteOnly | QIODevice::Text)) {
		std::cerr << "Failed to open given output file '" << args.at(1).toStdString() << "'! Can not write list." << std::endl;
		return 1;
	}
	output.write(modNames.join('\n').toUtf8());
	output.close();

	std::cout << "Bye bye!" << std::endl;
    return 0;
}
