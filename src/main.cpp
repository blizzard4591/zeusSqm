#include <QCoreApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QTextStream>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>

#include "MarkerCheckModule.h"
#include "StatisticsCheckModule.h"
#include "ObjectBuilderModule.h"
#include "SqmParser.h"

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
	QString missionFileData;

	QString const inputFilename = args.at(0);
	if (inputFilename.endsWith(".pbo")) {
		PBO::PBO pbo_file(inputFilename.toStdString());
		try {
			pbo_file.unpack();

			for (auto entry : pbo_file) {
				auto size = entry->get_data_size();
				auto offset = entry->get_data_offset();

				if (size > 0) {
					//std::vector<char> tempstorage(size);
					missionBinaryData = QByteArray(size, '\0');

					auto outfilename = entry->get_path().string();
					if (outfilename != "mission.sqm") {
						continue;
					}

					std::ifstream input(inputFilename.toStdString(), std::ios_base::binary);
					input.seekg(offset);
					input.read(missionBinaryData.data(), size);
					missionFileData = QString(missionBinaryData);

					input.close();
					std::cout << "Loaded mission.sqm (" << size << " Bytes) from PBO." << std::endl;
					//QFile test("debug.pbo.sqm");
					//test.open(QFile::WriteOnly);
					//test.write(tempstorage);
					//test.close();
					break;
				}
			}
		} catch (std::exception const& e) {
			std::cerr << "pbounpack : " << e.what() << std::endl;
			return EXIT_FAILURE;
		}

		if (missionBinaryData.isEmpty()) {
			std::cerr << "Failed to find 'mission.sqm' in PBO, can not continue." << std::endl;
			return EXIT_FAILURE;
		}
	} else {
		QFile inputFile(args.at(0));
		if (!inputFile.open(QFile::ReadOnly)) {
			std::cerr << "Failed to open input file '" << args.at(0).toStdString() << "'. Terminating..." << std::endl;
			return EXIT_FAILURE;
		}

		missionBinaryData = inputFile.readAll();
		inputFile.seek(0);
		QTextStream stream(&inputFile);
		missionFileData = stream.readAll();
		inputFile.close();
	}

	if (!markerCheck.checkArguments(parser)) {
		return -1;
	} else if (!statisticsCheck.checkArguments(parser)) {
		return -1;
	} else if (!objectBuilder.checkArguments(parser)) {
		return -1;
	}

	SqmParser sqmParser;
	std::shared_ptr<SqmObjectList<SqmStructure>> sqmObjects;
	if (missionBinaryData.size() > 0 && missionBinaryData.at(0) == '\0') {
		sqmObjects = std::make_shared<SqmObjectList<SqmStructure>>(sqmParser.parse(missionBinaryData));
	} else if (missionFileData.size() == 0) {
		std::cerr << "Input SQM file is empty :(" << std::endl;
		return EXIT_FAILURE;
	} else {
		sqmObjects = std::make_shared<SqmObjectList<SqmStructure>>(sqmParser.parse(missionFileData));
	}
	
	sqmObjects = markerCheck.perform(sqmObjects);
	sqmObjects = statisticsCheck.perform(sqmObjects);
	sqmObjects = objectBuilder.perform(sqmObjects);

	QString const rebuildMissionFileData = sqmObjects->toSqm(0);

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
	QTextStream outputStream(&outputFile);
	outputStream << rebuildMissionFileData;
	outputFile.close();
	std::cout << std::endl << "Saved SQM to '" << outputFile.fileName().toStdString() << "'." << std::endl;

	std::cout << "Bye bye!" << std::endl;
    return 0;
}

/*#ifdef _MSC_VER
int __stdcall WinMain(struct HINSTANCE__ *hInstance, struct HINSTANCE__ *hPrevInstance, char *lpszCmdLine, int nCmdShow) {
	return main(__argc, __argv);
}

#endif*/
