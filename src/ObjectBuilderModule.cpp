#include "ObjectBuilderModule.h"

#include <iostream>

#include <QImageReader>

#include "SqmArray.h"
#include "SqmClass.h"
#include "SqmProperty.h"
#include "SqmHandling.h"
#include "exceptions/FormatErrorException.h"

ObjectBuilderModule::ObjectBuilderModule() :
	CheckModule(),
	buildFromImageOption(QStringList() << "buildFromImage", QCoreApplication::translate("main", "Build a 2D structure from an image, where a non-transparent pixel is translated to a block and to air otherwise."), QCoreApplication::translate("main", "path to image")),
	buildMinAlphaValueOption(QStringList() << "buildMinAlpha", QCoreApplication::translate("main", "Minimal alpha channel value for a pixel not to be considered transparent."), QCoreApplication::translate("main", "1-255")),
	buildPlaneOption(QStringList() << "buildPlane", QCoreApplication::translate("main", "In which plane the structure is constructed."), QCoreApplication::translate("main", "xy, xz or yz")),
	buildStartingPointOption(QStringList() << "buildStartingPoint", QCoreApplication::translate("main", "Coordinates of where to start building the structure."), QCoreApplication::translate("main", "x,y,z (only integers, no floating points)")),
	buildFromImage(false), minAlphaValue(25), image(), buildPlane(BuildPlane::XY), startingPosition()
{
	//
}

void ObjectBuilderModule::registerOptions(QCommandLineParser& parser) {
	parser.addOption(buildFromImageOption);
	parser.addOption(buildMinAlphaValueOption);
	parser.addOption(buildPlaneOption);
	parser.addOption(buildStartingPointOption);
}

bool ObjectBuilderModule::checkArguments(QCommandLineParser& parser) {
	buildFromImage = parser.isSet(buildFromImageOption);
	if (buildFromImage) {
		QString const imagePath = parser.value(buildFromImageOption);
		image = QImage(imagePath);
		if (image.isNull()) {
			std::cerr << "Could not load/open input image '" << imagePath.toStdString() << "'." << std::endl;
			std::cerr << "Supported formats are: ";
			QList<QByteArray> const supportedFormats = QImageReader::supportedImageFormats();
			bool isFirst = true;
			for (auto it = supportedFormats.constBegin(); it != supportedFormats.constEnd(); ++it) {
				if (!isFirst) std::cerr << ", ";
				std::cerr << QString(*it).toStdString();
				isFirst = false;
			}
			std::cerr << std::endl;
			return false;
		}
	}

	if (parser.isSet(buildMinAlphaValueOption)) {
		QString const buildMinAlphaValue = parser.value(buildMinAlphaValueOption);
		bool ok = false;
		minAlphaValue = buildMinAlphaValue.toInt(&ok);
		if (!ok || (minAlphaValue < 1) || (minAlphaValue > 255)) {
			std::cerr << "Build minimum alpha-channel value '" << buildMinAlphaValue.toStdString() << "' is out of range. Value must be in 1 - 255." << std::endl;
			return false;
		}
	}

	if (parser.isSet(buildPlaneOption)) {
		QString const value = parser.value(buildPlaneOption);
		if (value.compare("xy", Qt::CaseInsensitive) == 0) {
			buildPlane = BuildPlane::XY;
		} else if (value.compare("xz", Qt::CaseInsensitive) == 0) {
			buildPlane = BuildPlane::XZ;
		} else if (value.compare("yz", Qt::CaseInsensitive) == 0) {
			buildPlane = BuildPlane::YZ;
		} else {
			std::cerr << "Build plane '" << value.toStdString() << "' is not supported. Supported are xy, xz and yz." << std::endl;
			return false;
		}
	}

	if (parser.isSet(buildStartingPointOption)) {
		QString const buildStartingPointValue = parser.value(buildStartingPointOption);
		QStringList const parts = buildStartingPointValue.split(',');
		if (parts.size() != 3) {
			std::cerr << "Build starting position '" << buildStartingPointValue.toStdString() << "' was not understood. Format: x,y,z" << std::endl;
			return false;
		}
		bool ok = false;
		startingPosition[0] = parts.at(0).toInt(&ok);
		if (!ok) { std::cerr << "Build starting position '" << buildStartingPointValue.toStdString() << "' was not understood. Format: x,y,z" << std::endl; return false; }
		startingPosition[1] = parts.at(1).toInt(&ok);
		if (!ok) { std::cerr << "Build starting position '" << buildStartingPointValue.toStdString() << "' was not understood. Format: x,y,z" << std::endl; return false; }
		startingPosition[2] = parts.at(2).toInt(&ok);
		if (!ok) { std::cerr << "Build starting position '" << buildStartingPointValue.toStdString() << "' was not understood. Format: x,y,z" << std::endl; return false; }
	}

	return true;
}

std::shared_ptr<SqmObjectList<SqmStructure>> ObjectBuilderModule::perform(std::shared_ptr<SqmObjectList<SqmStructure>> const& sqmObjects) {
	SqmRoot root = sqmObjects;

	if (buildFromImage) {
		int w = image.width();
		int h = image.height();

		std::size_t addedObjectCount = 0;
		for (int ih = h - 1, z = 0; ih >= 0; --ih, ++z) {
			for (int iw = 0; iw < w ; ++iw) {
				QRgb const rgb = image.pixel(iw, ih);
				
				int a = qAlpha(rgb);
				int r = qRed(rgb);
				int g = qGreen(rgb);
				int b = qBlue(rgb);

				if (a > minAlphaValue) {
					std::cout << " X";
					root = SqmHandling::newVrShapeObject(root, startingPosition[0] + iw, startingPosition[1] + 0, startingPosition[2] + z);
					//$missionAddition . = newObject($baseCoords, $iw, 0, $z);
					++addedObjectCount;
				} else {
					std::cout << "  ";
				}
			}
			std::cout << std::endl;
		}
	}

	return root;
}
