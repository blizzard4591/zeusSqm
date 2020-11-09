#include <QCoreApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QTextStream>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <variant>

#include "MarkerCheckModule.h"
#include "StatisticsCheckModule.h"
#include "ObjectBuilderModule.h"
#include "SqmParser.h"
#include "BinarizedSqm.h"

#include "libpbo/pbo.hpp"

#include "version.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

	QCoreApplication::setApplicationName("zeusSqm");
	QCoreApplication::setApplicationVersion(QString::fromStdString(Version::versionWithTagString()));

	QCommandLineParser parser;
	parser.setApplicationDescription("ARMA3 SQM Parser/Validator");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("mission", QCoreApplication::translate("main", "Mission SQM file to read"));
	parser.addPositionalArgument("outputMission", QCoreApplication::translate("main", "Mission SQM file to write (output) when applying changes"));

	QCommandLineOption inplaceOption(QStringList() << "inplace", QCoreApplication::translate("main", "Do changes inplace, i.e. write to input file. USE WITH CARE."));
	parser.addOption(inplaceOption);

	QCommandLineOption extractFromPboOption(QStringList() << "extractFromPbo", QCoreApplication::translate("main", "Extract the mission.sqm from PBO file and save it unchanged."), QCoreApplication::translate("main", "extracted mission.sqm"));
	parser.addOption(extractFromPboOption);

	QCommandLineOption stripCommentsOption(QStringList() << "stripComments", QCoreApplication::translate("main", "Remove comments from SQMs, even from the 'unchanged' extract from PBO."));
	parser.addOption(stripCommentsOption);

	QCommandLineOption saveBinarizedOption(QStringList() << "saveBinarized", QCoreApplication::translate("main", "Write <outputMission> as binarized SQM."));
	parser.addOption(saveBinarizedOption);

	QCommandLineOption saveBinarizedIfBinarizedInputOption(QStringList() << "saveBinarizedIfBinarizedInput", QCoreApplication::translate("main", "Write <outputMission> as binarized SQM iff the input file is binarized."));
	parser.addOption(saveBinarizedIfBinarizedInputOption);

	MarkerCheckModule markerCheck;
	markerCheck.registerOptions(parser);

	StatisticsCheckModule statisticsCheck;
	statisticsCheck.registerOptions(parser);

	ObjectBuilderModule objectBuilder;
	objectBuilder.registerOptions(parser);

	// Process the actual command line arguments given by the user
	parser.process(app);

	QStringList args = parser.positionalArguments();
	if (args.isEmpty()) {
		args << "mission.sqm";
	}

	if (args.isEmpty() || (args.size() < 2 && !parser.isSet(inplaceOption))) {
		parser.showHelp(EXIT_FAILURE);
	}

	// HEADER
	std::cout << "ARMA3 SQM Parser/Validator/Fixer v" << Version::versionWithTagString() << std::endl;
	std::cout << std::endl;

	QByteArray missionBinaryData;
	QString const inputFilename = args.at(0);
	bool const isFromPbo = inputFilename.endsWith(".pbo");
	if (isFromPbo) {
		PBO::PBO pbo_file(inputFilename.toStdString());
		try {
			pbo_file.unpack();

			bool foundMission = false;
			for (auto entry : pbo_file) {
				auto size = entry->get_data_size();
				auto offset = entry->get_data_offset();

				if (size > 0) {
					auto outfilename = entry->get_path().string();
					if (outfilename != "mission.sqm") {
						continue;
					}

					foundMission = true;
					missionBinaryData = QByteArray(size, '\0');

					std::ifstream input(inputFilename.toStdString(), std::ios_base::binary);
					input.seekg(offset);
					input.read(missionBinaryData.data(), size);
					input.close();

					std::cout << "Loaded mission.sqm (" << size << " Bytes) from PBO." << std::endl;
					break;
				}
			}

			if (!foundMission) {
				std::cerr << "Could not locate 'mission.sqm' in PBO, is the archive complete?" << std::endl;
				return EXIT_FAILURE;
			}
		} catch (std::exception const& e) {
			std::cerr << "pbounpack : " << e.what() << std::endl;
			return EXIT_FAILURE;
		}
	} else {
		QFile inputFile(args.at(0));
		if (!inputFile.open(QFile::ReadOnly)) {
			std::cerr << "Failed to open input file '" << args.at(0).toStdString() << "'. Terminating..." << std::endl;
			return EXIT_FAILURE;
		}

		missionBinaryData = inputFile.readAll();
	}

	if (!markerCheck.checkArguments(parser)) {
		return -1;
	} else if (!statisticsCheck.checkArguments(parser)) {
		return -1;
	} else if (!objectBuilder.checkArguments(parser)) {
		return -1;
	}

	// Strip comments if requested
	if (parser.isSet(stripCommentsOption)) {
		missionBinaryData = SqmParser::stripComments(missionBinaryData);
	}

	if (isFromPbo && parser.isSet(extractFromPboOption)) {
		QFile pboSqm(parser.value(extractFromPboOption));
		if (pboSqm.open(QFile::WriteOnly)) {
			pboSqm.write(missionBinaryData);
			pboSqm.close();
			std::cout << "Saved mission.sqm from PBO to '" << parser.value(extractFromPboOption).toStdString() << "' (untouched)." << std::endl;
		} else {
			std::cout << "Failed to save mission.sqm from PBO to '" << parser.value(extractFromPboOption).toStdString() << "', is it writable?" << std::endl;
			return EXIT_FAILURE;
		}
	}

	bool useSimpleNewline = false;
	bool const isBinarized = SqmParser::hasBinarizedSqmHeader(missionBinaryData);
	SqmParser sqmParser;
	std::shared_ptr<SqmObjectList<SqmStructure>> sqmObjects;
	if (isBinarized) {
		sqmObjects = std::make_shared<SqmObjectList<SqmStructure>>(sqmParser.parse(missionBinaryData));
	} else {
		QTextStream stream(missionBinaryData);
		stream.setCodec("UTF-8");
		QString const missionFileData = stream.readAll();

		// Check if the input used Linux-style \n newlines.
		useSimpleNewline = missionFileData.count("\r\n") == 0;
		sqmObjects = std::make_shared<SqmObjectList<SqmStructure>>(sqmParser.parse(missionFileData));
	}
	
	if (sqmObjects->getIntProperty(QStringLiteral("version"))->getValueAsInt() != 53) {
		std::cerr << "Incompatile version: SQM version is not 53, but instead " << sqmObjects->getIntProperty(QStringLiteral("version"))->getValueAsInt() << ". Can not parse!" << std::endl;
		return EXIT_FAILURE;
	}

	sqmObjects = markerCheck.perform(sqmObjects);
	sqmObjects = statisticsCheck.perform(sqmObjects);
	sqmObjects = objectBuilder.perform(sqmObjects);

	QString rebuildMissionFileData = sqmObjects->toSqm(0);

	// Match the newline-style of the input
	if (useSimpleNewline) {
		rebuildMissionFileData.replace("\r\n", "\n");
	}

	QFile outputFile;
	if (parser.isSet(inplaceOption)) {
		outputFile.setFileName(args.at(0));
	} else {
		outputFile.setFileName(args.at(1));
	}

	if (!outputFile.open(QFile::WriteOnly)) {
		std::cerr << "Failed to open output file '" << outputFile.fileName().toStdString() << "' for writing, can not write result!" << std::endl;
		return -5;
	}

	if (parser.isSet(saveBinarizedOption) || (isBinarized && parser.isSet(saveBinarizedIfBinarizedInputOption))) {
		outputFile.write(sqmObjects->toBinarizedSqm());
		std::cout << std::endl << "Saved binarized SQM to '" << outputFile.fileName().toStdString() << "'." << std::endl;
	} else {
		QTextStream outputStream(&outputFile);
		outputStream.setCodec("UTF-8");
		outputStream << rebuildMissionFileData;
		std::cout << std::endl << "Saved SQM to '" << outputFile.fileName().toStdString() << "'." << std::endl;
	}
	outputFile.close();

	std::cout << "Bye bye!" << std::endl;
    return 0;
}

/*#ifdef _MSC_VER
int __stdcall WinMain(struct HINSTANCE__ *hInstance, struct HINSTANCE__ *hPrevInstance, char *lpszCmdLine, int nCmdShow) {
	return main(__argc, __argv);
}

#endif*/
