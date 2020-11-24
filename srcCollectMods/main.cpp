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

#include "pbo/pbo.h"

#include "version.h"

QStringList scanDirectoryForPbos(QString const& dirName) {
	QStringList result;
	QDirIterator it(dirName, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		QString const path = it.next();
		if (path.endsWith(QStringLiteral(".pbo"), Qt::CaseInsensitive)) {
			result.append(path);
		}
	}
	return result;
}

bool pboHasFile(PBO::PBO& pbo, QString const& filename) {
	for (auto entry : pbo) {
		auto size = entry->get_data_size();
		auto offset = entry->get_data_offset();

		if (size > 0) {
			auto outfilename = entry->get_path();
			std::cout << "Filename: " << outfilename.toStdString() << std::endl;
			if (outfilename.compare(filename) != 0) {
				continue;
			}

			return true;
		}
	}

	return false;
}

QByteArray loadFileFromPbo(QString const& pboFileName, PBO::PBO& pbo, QString const& filename) {
	for (auto entry : pbo) {
		auto size = entry->get_data_size();
		auto offset = entry->get_data_offset();

		if (size > 0) {
			auto outfilename = entry->get_path();
			if (outfilename.compare(filename) != 0) {
				continue;
			}

			QFile input(pboFileName);
			if (!input.open(QFile::ReadOnly)) {
				LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Could not open '" << pboFileName.toStdString() << "' for reading, is the archive still there?");
			}
			input.skip(offset);
			QByteArray const fileData = input.readAll();
			input.close();

			std::cout << "Loaded '" << filename.toStdString() << "' (" << size << " Bytes) from PBO." << std::endl;
			return fileData;
		}
	}

	LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Could not locate '" << filename.toStdString() << "' in PBO '" << pboFileName.toStdString() << "', is the archive complete?");
}

QByteArray loadConfigFromPbo(QString const& pboFileName) {
	QString const searchFileNameBin = QStringLiteral("config.bin");
	QString const searchFileNameTxt = QStringLiteral("config.cpp");
	PBO::PBO pbo_file(pboFileName);
	try {
		pbo_file.unpack();

		if (pboHasFile(pbo_file, searchFileNameBin)) {
			QByteArray const configBin = loadFileFromPbo(pboFileName, pbo_file, searchFileNameBin);

			QFile debugOut(QString("debug_%1_config.bin").arg(QFileInfo(pboFileName).baseName()));
			debugOut.open(QFile::WriteOnly);
			debugOut.write(configBin);
			debugOut.close();

			return configBin;
		} else if (pboHasFile(pbo_file, searchFileNameTxt)) {
			QByteArray configCpp = loadFileFromPbo(pboFileName, pbo_file, searchFileNameTxt);
			//QString config = QString::fromUtf8(configCpp);
	
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
					LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Could not locate end of #include section in 'config.cpp' in PBO '" << pboFileName.toStdString() << "', is the archive complete?");
				}

				QByteArray const matched = configCpp.mid(pos + 1, posOfClosingQuote - pos - 1);
				std::cout << "Found include for '" << matched.toStdString() << "', following... " << std::endl;
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

			QFile debugOut(QString("debug_%1_config.txt").arg(QFileInfo(pboFileName).baseName()));
			debugOut.open(QFile::WriteOnly);
			debugOut.write(configCpp);
			debugOut.close();

			return configCpp;
		} else {
			LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Could not locate '" << searchFileNameBin.toStdString() << "' or '" << searchFileNameTxt.toStdString() << "' in PBO '" << pboFileName.toStdString() << "', is the archive complete?");
		}
	} catch (std::exception const& e) {
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "pbounpack : " << e.what());
	}

	return QByteArray();
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
		sqmObjects = std::make_shared<SqmObjectList<SqmStructure>>(sqmParser.parse(missionFileData));
	}

	if (!sqmObjects->hasClass(QStringLiteral("CfgPatches"))) {
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Failed to parse file '" << pboFileName.toStdString() << "', class 'CfgPatches' could not be found!");
	}

	SqmObjectList<SqmClass> patchClasses = sqmObjects->getClass(QStringLiteral("CfgPatches"))->onlyClasses();
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

	// Process the actual command line arguments given by the user
	parser.process(app);

	QStringList args = parser.positionalArguments();
	if (args.isEmpty()) {
		parser.showHelp(EXIT_FAILURE);
	}

	// HEADER
	std::cout << "ARMA3 mod name collector v" << Version::versionWithTagString() << std::endl;
	std::cout << std::endl;

	QStringList pboFiles = scanDirectoryForPbos(args.at(0));
	QStringList modNames;
	std::cout << "Found " << pboFiles.size() << " PBO files." << std::endl;
	for (int i = 0; i < pboFiles.size(); ++i) {
		try {
			QStringList const subModNames = extractModNamesFromPbo(pboFiles.at(i));
			modNames.append(subModNames);
			for (int i = 0; i < subModNames.size(); ++i) {
				std::cout << "Found mod with name '" << subModNames.at(i).toStdString() << "'." << std::endl;
			}
		} catch (zeusops::exceptions::FormatErrorExceptionImpl const& e) {
			// Ignore for now.
			std::cerr << "Ignoring file '" << pboFiles.at(i).toStdString() << "' due to parsing exception: " << e.what() << std::endl;
		}
	}
	std::cout << "Found a total of " << modNames.size() << " mods." << std::endl;

	std::cout << "Bye bye!" << std::endl;
    return 0;
}

/*#ifdef _MSC_VER
int __stdcall WinMain(struct HINSTANCE__ *hInstance, struct HINSTANCE__ *hPrevInstance, char *lpszCmdLine, int nCmdShow) {
	return main(__argc, __argv);
}

#endif*/
