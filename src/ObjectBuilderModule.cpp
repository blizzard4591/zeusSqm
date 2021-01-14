#include "ObjectBuilderModule.h"

#include <chrono>
#include <iostream>

#include <QImageReader>

#include "Maze.h"
#include "SqmArray.h"
#include "SqmClass.h"
#include "SqmProperty.h"
#include "SqmHandling.h"
#include "exceptions/FormatErrorException.h"

ObjectBuilderModule::ObjectBuilderModule() :
	CheckModule(),
	buildFromImageOption(QStringList() << "buildFromImage", QCoreApplication::translate("main", "Build a 2D structure from an image, where a non-transparent pixel is translated to a block and to air otherwise."), QCoreApplication::translate("main", "path_to_image")),
	buildPyramidOption(QStringList() << "buildPyramid", QCoreApplication::translate("main", "Build a 3D structure from objects."), QCoreApplication::translate("main", "object_name")),
	buildMazeOption(QStringList() << "buildMaze", QCoreApplication::translate("main", "Build a 2D maze structure. Size is X * Y."), QCoreApplication::translate("main", "x,y")),
	buildMinAlphaValueOption(QStringList() << "buildMinAlpha", QCoreApplication::translate("main", "Minimal alpha channel value for a pixel not to be considered transparent."), QCoreApplication::translate("main", "1-255")),
	buildPlaneOption(QStringList() << "buildPlane", QCoreApplication::translate("main", "In which plane the structure is constructed."), QCoreApplication::translate("main", "xy, xz or yz")),
	buildStartingPointOption(QStringList() << "buildStartingPoint", QCoreApplication::translate("main", "Coordinates of where to start building the structure. X, Y and Z can only be integers, no floating points."), QCoreApplication::translate("main", "x,y,z")),
	buildPyramidOffsetsOption(QStringList() << "pyramidOffsets", QCoreApplication::translate("main", "Offsets to be used between placed elements in the three planes. X, Y and Z are floating points."), QCoreApplication::translate("main", "x,y,z")),
	buildPyramidLayersOption(QStringList() << "pyramidLayers", QCoreApplication::translate("main", "How many layers the pyramid should have."), QCoreApplication::translate("main", "1-255")),
	buildMazeHedgeHeightOption(QStringList() << "mazeHedgeHeight", QCoreApplication::translate("main", "How tall the maze should be."), QCoreApplication::translate("main", "1-255")),
	buildMazeScalingOption(QStringList() << "mazeScale", QCoreApplication::translate("main", "How wide the paths in the maze should be."), QCoreApplication::translate("main", "1-255")),
	buildFromImage(false), buildPyramid(false), buildMaze(false), objectType(), minAlphaValue(25), image(), buildPlane(BuildPlane::XY), startingPosition(), pyramidOffsets(), pyramidLayers(4), mazeDimensions(), mazeHedgeHeight(3), mazeScale(1)
{
	//
	pyramidOffsets[0] = 0.35f;
	pyramidOffsets[1] = 0.15f;
	pyramidOffsets[2] = 0.115f;
}

void ObjectBuilderModule::registerOptions(QCommandLineParser& parser) {
	parser.addOption(buildFromImageOption);
	parser.addOption(buildPyramidOption);
	parser.addOption(buildMazeOption);
	parser.addOption(buildMinAlphaValueOption);
	parser.addOption(buildPlaneOption);
	parser.addOption(buildStartingPointOption);
	parser.addOption(buildPyramidOffsetsOption);
	parser.addOption(buildPyramidLayersOption);
	parser.addOption(buildMazeHedgeHeightOption);
	parser.addOption(buildMazeScalingOption);
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

	buildPyramid = parser.isSet(buildPyramidOption);
	if (buildPyramid) {
		objectType = parser.value(buildPyramidOption);
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

	if (parser.isSet(buildPyramidOffsetsOption)) {
		QString const buildPyramidOffsetsValue = parser.value(buildPyramidOffsetsOption);
		QStringList const parts = buildPyramidOffsetsValue.split(',');
		if (parts.size() != 3) {
			std::cerr << "Pyramid offsets '" << buildPyramidOffsetsValue.toStdString() << "' was not understood. Format: x,y,z" << std::endl;
			return false;
		}
		bool ok = false;
		pyramidOffsets[0] = parts.at(0).toFloat(&ok);
		if (!ok) { std::cerr << "Pyramid offsets '" << buildPyramidOffsetsValue.toStdString() << "' was not understood. Format: x,y,z" << std::endl; return false; }
		pyramidOffsets[1] = parts.at(1).toFloat(&ok);
		if (!ok) { std::cerr << "Pyramid offsets '" << buildPyramidOffsetsValue.toStdString() << "' was not understood. Format: x,y,z" << std::endl; return false; }
		pyramidOffsets[2] = parts.at(2).toFloat(&ok);
		if (!ok) { std::cerr << "Pyramid offsets '" << buildPyramidOffsetsValue.toStdString() << "' was not understood. Format: x,y,z" << std::endl; return false; }
	}

	if (parser.isSet(buildPyramidLayersOption)) {
		QString const pyramidLayersValue = parser.value(buildPyramidLayersOption);
		bool ok = false;
		pyramidLayers = pyramidLayersValue.toInt(&ok);
		if (!ok || (pyramidLayers < 1) || (pyramidLayers > 255)) {
			std::cerr << "Pyramid layer count value '" << pyramidLayersValue.toStdString() << "' is out of range. Value must be in 1 - 255." << std::endl;
			return false;
		}
	}

	buildMaze = parser.isSet(buildMazeOption);
	if (buildMaze) {
		QString const buildMazeValue = parser.value(buildMazeOption);
		QStringList const parts = buildMazeValue.split(',');
		if (parts.size() != 2) {
			std::cerr << "Maze dimensions '" << buildMazeValue.toStdString() << "' was not understood. Format: x,y" << std::endl;
			return false;
		}
		bool ok = false;
		mazeDimensions[0] = parts.at(0).toInt(&ok);
		if (!ok) { std::cerr << "Maze dimensions '" << buildMazeValue.toStdString() << "' was not understood. Format: x,y" << std::endl; return false; }
		mazeDimensions[1] = parts.at(1).toInt(&ok);
		if (!ok) { std::cerr << "Maze dimensions '" << buildMazeValue.toStdString() << "' was not understood. Format: x,y" << std::endl; return false; }
	}

	if (parser.isSet(buildMazeScalingOption)) {
		QString const mazeScaleValue = parser.value(buildMazeScalingOption);
		bool ok = false;
		mazeScale = mazeScaleValue.toInt(&ok);
		if (!ok || (mazeScale < 1) || (mazeScale > 255)) {
			std::cerr << "Maze scale value '" << mazeScaleValue.toStdString() << "' is out of range. Value must be in 1 - 255." << std::endl;
			return false;
		}
	}

	return true;
}

std::shared_ptr<SqmObjectList<SqmStructure>> ObjectBuilderModule::perform(std::shared_ptr<SqmObjectList<SqmStructure>> const& sqmObjects) {
	SqmRoot root = sqmObjects;

	if (buildFromImage) {
		auto t1 = std::chrono::high_resolution_clock::now();

		int w = image.width();
		int h = image.height();
		std::cout << std::endl << "Inserting your " << w << " x " << h << " image into map..." << std::endl;

		std::size_t addedObjectCount = 0;
		std::vector<SqmHandling::Position> positions;
		for (int ih = h - 1, z = 0; ih >= 0; --ih, ++z) {
			for (int iw = 0; iw < w ; ++iw) {
				QRgb const rgb = image.pixel(iw, ih);
				
				int a = qAlpha(rgb);
				//int r = qRed(rgb);
				//int g = qGreen(rgb);
				//int b = qBlue(rgb);

				if (a > minAlphaValue) {
					std::cout << " X";
					positions.push_back({ startingPosition[0] + iw, startingPosition[1] + 0, startingPosition[2] + z });
					++addedObjectCount;
				} else {
					std::cout << "  ";
				}
			}
			std::cout << std::endl;
		}
		root = SqmHandling::addVrShapeObjects(root, positions);

		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		std::cout << "Building your object into the map took " << duration << "ms." << std::endl;
	}

	if (buildPyramid) {
		auto t1 = std::chrono::high_resolution_clock::now();

		int w = image.width();
		int h = image.height();
		std::cout << std::endl << "Inserting pyramid into map..." << std::endl;

		std::size_t addedObjectCount = 0;
		std::vector<SqmHandling::FloatPosition> positions;
		// +1 for < and easier substraction
		int const layerCount = pyramidLayers + 1;
		for (int layer = 0; layer < layerCount; ++layer) {
			int const countInX = layerCount - layer;
			int const countInY = layerCount - layer;

			float const stepOverX = pyramidOffsets[0];
			float const stepOverY = pyramidOffsets[1];
			float const stepOverZ = pyramidOffsets[2];

			for (int x = 0; x < countInX; ++x) {
				for (int y = 0; y < countInY; ++y) {
					positions.push_back({ startingPosition[0] + (x * stepOverX) + (layer * stepOverX * 0.5f), startingPosition[1] + (y * stepOverY) + (layer * stepOverY * 0.5f), startingPosition[2] + (layer * stepOverZ) });
					++addedObjectCount;
				}
			}
		}
		root = SqmHandling::addObjects(root, objectType, positions);

		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		std::cout << "Building your object into the map took " << duration << "ms." << std::endl;
	}

	if (buildMaze) {
		auto t1 = std::chrono::high_resolution_clock::now();

		Maze maze(mazeDimensions[0], mazeDimensions[1], true, true, (mazeDimensions[0] + mazeDimensions[1]) * 1.3);
		maze.print();

		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		std::cout << "Generating your maze took " << duration << "ms." << std::endl;

		std::cout << std::endl << "Inserting maze into map (" << mazeDimensions[0] << " x " << mazeDimensions[1] << ", height " << mazeHedgeHeight << ", " << mazeScale << " wide paths)..." << std::endl;
		t1 = std::chrono::high_resolution_clock::now();

		std::size_t addedObjectCount = 0;
		std::vector<SqmHandling::FloatPosition> positions;
		for (int w = 0; w < maze.getWidth(); ++w) {
			for (int h = 0; h < maze.getHeight(); ++h) {
				Maze::Cell const& c = maze.getCell(w, h);
				if (c == Maze::Cell::Blocked) {
					for (int height = 0; height < mazeHedgeHeight; ++height) {
						for (int scale = 0; scale < mazeScale; ++scale) {
							float const x = startingPosition[0] + (w * mazeScale) + scale;
							float const y = startingPosition[1] + (h * mazeScale) + scale;
							float const z = startingPosition[2] + height;
							positions.push_back({ x, y, z });
							++addedObjectCount;
						}
					}
				}
			}
		}

		root = SqmHandling::addObjects(root, objectType, positions);

		t2 = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		std::cout << "Building your object into the map took " << duration << "ms." << std::endl;
	}

	return root;
}
