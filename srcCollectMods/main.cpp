#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDirIterator>
#include <QFile>
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

QByteArray loadConfigFromPbo(std::string const& pboFileName) {
	std::string const searchFileName = "config.bin";
	PBO::PBO pbo_file(pboFileName);
	bool foundConfig = false;
	try {
		pbo_file.unpack();

		for (auto entry : pbo_file) {
			auto size = entry->get_data_size();
			auto offset = entry->get_data_offset();

			if (size > 0) {
				auto outfilename = entry->get_path().string();
				if (outfilename != searchFileName) {
					continue;
				}

				foundConfig = true;
				QByteArray configBinaryData(size, '\0');
				std::ifstream input(pboFileName, std::ios_base::binary);
				input.seekg(offset);
				input.read(configBinaryData.data(), size);
				input.close();

				std::cout << "Loaded '" << searchFileName << "' (" << size << " Bytes) from PBO." << std::endl;
				return configBinaryData;
			}
		}
	} catch (std::exception const& e) {
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "pbounpack : " << e.what());
	}
	if (!foundConfig) {
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Could not locate '" << searchFileName << "' in PBO '" << pboFileName << "', is the archive complete?");
	}

	return QByteArray();
}

QString extractModNameFromPbo(std::string const& pboFileName) {
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
	if (patchClasses.size() != 1) {
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Failed to parse file '" << pboFileName << "', class 'CfgPatches' has " << patchClasses.size() << " sub-classes instead of 1!");
	}

	return (*patchClasses.begin())->getName();
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
			QString const modName = extractModNameFromPbo(pboFiles.at(i).toStdString());
			modNames.append(modName);
			std::cout << "Found mod with name '" << modName.toStdString() << "'." << std::endl;
		} catch (zeusops::exceptions::FormatErrorExceptionImpl const& e) {
			// Ignore for now.
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
