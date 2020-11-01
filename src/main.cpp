#include <QCoreApplication>
#include <QCommandLineParser>
#include <QFile>
#include <QTextStream>

#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>

#include "MarkerCheck.h"
#include "SqmParser.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

	QCoreApplication::setApplicationName("zeusSqm");
	QCoreApplication::setApplicationVersion("1.0");


	QCommandLineParser parser;
	parser.setApplicationDescription("ARMA3 SQM Parser/Validator");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("mission", QCoreApplication::translate("main", "Mission SQM file to read"));

	MarkerCheck markerCheck(parser);

	// Process the actual command line arguments given by the user
	parser.process(app);

	QStringList args = parser.positionalArguments();
	if (args.isEmpty()) {
		args << "mission.sqm";
	}

	if (args.isEmpty()) {
		parser.showHelp(-1);
	}

	QFile inputFile(args.at(0));
	if (!inputFile.open(QFile::ReadOnly)) {
		std::cerr << "Failed to open input file '" << args.at(0).toStdString() << "'. Terminating..." << std::endl;
		return -1;
	}

	if (!markerCheck.checkArguments(parser)) {
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

	QString const rebuildMissionFileData = sqmObjects->toSqm(0);
	QFile outputFile("debug.sqm");
	outputFile.open(QFile::WriteOnly);
	QTextStream outputStream(&outputFile);
	outputStream << rebuildMissionFileData;
	outputFile.close();

	std::cout << "Bye bye!" << std::endl;
    return 0;
}

/*#ifdef _MSC_VER
int __stdcall WinMain(struct HINSTANCE__ *hInstance, struct HINSTANCE__ *hPrevInstance, char *lpszCmdLine, int nCmdShow) {
	return main(__argc, __argv);
}

#endif*/
