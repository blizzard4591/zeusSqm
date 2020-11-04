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

uint8_t convertA(char c) {
	return (uint8_t)c;
}

uint8_t convertB(char c) {
	uint8_t result = *reinterpret_cast<uint8_t*>(&c);
	return result;
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

	QCoreApplication::setApplicationName("zeusSqm");
	QCoreApplication::setApplicationVersion("1.0");

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
		parser.showHelp(-1);
	}

	QFile inputFile(args.at(0));
	if (!inputFile.open(QFile::ReadOnly)) {
		std::cerr << "Failed to open input file '" << args.at(0).toStdString() << "'. Terminating..." << std::endl;
		return -1;
	}

	if (!markerCheck.checkArguments(parser)) {
		return -1;
	} else if (!statisticsCheck.checkArguments(parser)) {
		return -1;
	} else if (!objectBuilder.checkArguments(parser)) {
		return -1;
	}

	QTextStream stream(&inputFile);
	QString const missionFileData = stream.readAll();
	inputFile.close();

	SqmParser sqmParser;

	auto t1 = std::chrono::high_resolution_clock::now();
	std::shared_ptr<SqmObjectList<SqmStructure>> sqmObjects = std::make_shared<SqmObjectList<SqmStructure>>(sqmParser.parse(missionFileData));
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "Parsing SQM took " << duration << "ms." << std::endl;

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
	std::cout << "Saved SQM to '" << outputFile.fileName().toStdString() << "'." << std::endl;

	std::cout << "Bye bye!" << std::endl;
    return 0;
}

/*#ifdef _MSC_VER
int __stdcall WinMain(struct HINSTANCE__ *hInstance, struct HINSTANCE__ *hPrevInstance, char *lpszCmdLine, int nCmdShow) {
	return main(__argc, __argv);
}

#endif*/
