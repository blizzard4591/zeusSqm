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

#include "libpbo/pbo.hpp"

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

bool pboHasFile(PBO::PBO& pbo, std::string const& filename) {
	for (auto entry : pbo) {
		auto size = entry->get_data_size();
		auto offset = entry->get_data_offset();

		if (size > 0) {
			auto outfilename = entry->get_path().string();
			if (outfilename.compare(filename) != 0) {
				continue;
			}

			return true;
		}
	}

	return false;
}

QByteArray loadFileFromPbo(std::string const& pboFileName, PBO::PBO& pbo, std::string const& filename) {
	for (auto entry : pbo) {
		auto size = entry->get_data_size();
		auto offset = entry->get_data_offset();

		if (size > 0) {
			auto outfilename = entry->get_path().string();
			if (outfilename.compare(filename) != 0) {
				continue;
			}

			QByteArray fileData(size, '\0');
			std::ifstream input(pboFileName, std::ios_base::binary);
			input.seekg(offset);
			input.read(fileData.data(), size);
			input.close();

			std::cout << "Loaded '" << filename << "' (" << size << " Bytes) from PBO." << std::endl;
			/*QFile debugOut(QString::fromStdString(pboFileName) + "_" + "config.bin");
			debugOut.open(QFile::WriteOnly);
			debugOut.write(fileData);
			debugOut.close();*/

			return fileData;
		}
	}

	LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Could not locate '" << filename << "' in PBO '" << pboFileName << "', is the archive complete?");
}

QByteArray loadConfigFromPbo(std::string const& pboFileName) {
	std::string const searchFileNameBin = "config.bin";
	std::string const searchFileNameTxt = "config.cpp";
	PBO::PBO pbo_file(pboFileName);
	try {
		pbo_file.unpack();

		if (pboHasFile(pbo_file, searchFileNameBin)) {
			QByteArray const configBin = loadFileFromPbo(pboFileName, pbo_file, searchFileNameBin);
			QFile debugOut(QString("debug_%1_config.bin").arg(QFileInfo(QString::fromStdString(pboFileName)).baseName()));
			debugOut.open(QFile::WriteOnly);
			debugOut.write(configBin);
			debugOut.close();

			return configBin;
		} else if (pboHasFile(pbo_file, searchFileNameTxt)) {
			QByteArray configCpp = loadFileFromPbo(pboFileName, pbo_file, searchFileNameTxt);
			//QString config = QString::fromUtf8(configCpp);
	
			static const QByteArray includeBytes = QStringLiteral("#include \"").toUtf8();

			int pos = configCpp.indexOf(includeBytes);
			while (pos != -1) {
				int const startPos = pos;
				// move to first "
				pos = configCpp.indexOf('"', pos);
				int posOfClosingQuote = pos + 1;
				while ((posOfClosingQuote < configCpp.size()) && (configCpp.at(posOfClosingQuote) != '"')) {
					++posOfClosingQuote;
				}
				if (posOfClosingQuote >= configCpp.size()) {
					LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Could not locate end of #include section in 'config.cpp' in PBO '" << pboFileName << "', is the archive complete?");
				}

				QByteArray const matched = configCpp.mid(pos + 1, posOfClosingQuote - pos - 1);
				std::cout << "Found include for '" << matched.toStdString() << "', following... " << std::endl;
				QByteArray const includeData = loadFileFromPbo(pboFileName, pbo_file, matched.toStdString());
				configCpp.replace(startPos, posOfClosingQuote - startPos + 1, includeData);

				pos = configCpp.indexOf(includeBytes);
			}

			QFile debugOut(QString("debug_%1_config.txt").arg(QFileInfo(QString::fromStdString(pboFileName)).baseName()));
			debugOut.open(QFile::WriteOnly);
			debugOut.write(configCpp);
			debugOut.close();

			return configCpp;
		} else {
			LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Could not locate '" << searchFileNameBin << "' or '" << searchFileNameTxt << "' in PBO '" << pboFileName << "', is the archive complete?");
		}
	} catch (std::exception const& e) {
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "pbounpack : " << e.what());
	}

	return QByteArray();
}

QStringList extractModNamesFromPbo(std::string const& pboFileName) {
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
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Failed to parse file '" << pboFileName << "', class 'CfgPatches' could not be found!");
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
			QStringList const subModNames = extractModNamesFromPbo(pboFiles.at(i).toStdString());
			modNames.append(subModNames);
			for (int i = 0; i < subModNames.size(); ++i) {
				std::cout << "Found mod with name '" << subModNames.at(i).toStdString() << "'." << std::endl;
			}
		} catch (zeusops::exceptions::FormatErrorExceptionImpl const& e) {
			// Ignore for now.
			return -1;
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
