#include "MarkerCheck.h"

#include <iostream>

#include "SqmArray.h"
#include "SqmClass.h"
#include "SqmProperty.h"
#include "exceptions/FormatErrorException.h"

MarkerCheck::MarkerCheck(QCommandLineParser& parser) :
	checkMarkerOption(QStringList() << "m" << "markers", QCoreApplication::translate("main", "Check marker placements.")),
	markerMaxDistanceOption(QStringList() << "markerMaxDistance", QCoreApplication::translate("main", "Maximum distance of marker from center to be considered for correction (to leave deliberately placed markers where they are, default: 25).")),
	markerAskConfirmationOption(QStringList() << "markerAskConfirmation", QCoreApplication::translate("main", "Will ask for each marker whether it should be moved (default: no).")),
	markerGridOption(QStringList() << "markerGrid", QCoreApplication::translate("main", "What grid the markers should snap to (default: 100).")),
	checkMarkers(false), markerGrid(0), markerMaxDistance(0), markerAskConfirmation(false)
{
	parser.addOption(checkMarkerOption);
	parser.addOption(markerMaxDistanceOption);
	parser.addOption(markerAskConfirmationOption);
	parser.addOption(markerGridOption);
}

bool MarkerCheck::checkArguments(QCommandLineParser& parser) {
	checkMarkers = parser.isSet(checkMarkerOption);
	markerGrid = 100;
	if (parser.isSet(markerGridOption)) {
		bool ok = false;
		markerGrid = parser.value(markerGridOption).toInt(&ok);
		if (!ok || (markerGrid <= 0)) {
			std::cerr << "Marker grid must be > 0!" << std::endl;
			return false;
		}
	}
	markerMaxDistance = 25;
	if (parser.isSet(markerMaxDistanceOption)) {
		bool ok = false;
		markerMaxDistance = parser.value(markerMaxDistanceOption).toInt(&ok);
		if (!ok || (markerMaxDistance <= 0)) {
			std::cerr << "Maximum distance of Marker to grid center must be > 0!" << std::endl;
			return false;
		}
	}

	markerAskConfirmation = false;
	if (parser.isSet(markerAskConfirmationOption)) {
		markerAskConfirmation = true;
	}

	return true;
}

void MarkerCheck::perform(SqmObjectList<SqmStructure> const& sqmObjects) {
	if (checkMarkers) {
		std::cout << "Checking marker locations..." << std::endl;
		std::cout << "Asking for confirmation before moving: " << (markerAskConfirmation ? "yes" : "no") << std::endl;
		std::cout << "Marker grid size: " << markerGrid << std::endl;
		std::cout << "Maximum distance of marker to grid-center to consider move: " << markerMaxDistance << std::endl;

		std::size_t markerCount = 0;
		SqmClass* classMission = dynamic_cast<SqmClass*>(sqmObjects.getByName(QStringLiteral("Mission")));
		if (classMission == nullptr) {
			std::cerr << "FORMAT ERROR: SQM does not contain class 'Mission', is it complete/the correct file?" << std::endl;
			throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Mission', is it complete/the correct file?";
		}

		SqmClass* classEntities = dynamic_cast<SqmClass*>(classMission->getByName(QStringLiteral("Entities")));
		if (classEntities == nullptr) {
			std::cerr << "FORMAT ERROR: SQM does not contain class 'Entities', is it complete/the correct file?" << std::endl;
			throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Entities', is it complete/the correct file?";
		}

		static const QString fieldDataType = QStringLiteral("dataType");
		static const QString fieldType = QStringLiteral("type");
		static const QString fieldPosition = QStringLiteral("position");
		static const QString valueMarker = QStringLiteral("\"Marker\"");

		SqmObjectList<SqmClass> natoMarkers = classEntities->onlyClasses().filter([](std::shared_ptr<SqmClass> const& classItem) {
			if (!classItem->hasProperty(fieldDataType)) throw;
			if (classItem->getPropertyValue(fieldDataType).compare(valueMarker) != 0) return false;
			if (!classItem->hasArray(fieldPosition)) return false;
			if (!classItem->hasProperty(fieldDataType)) return false;
			QString const& v = classItem->getPropertyValue(fieldType);
			return (v.startsWith("\"b_") || v.startsWith("\"o_") || v.startsWith("\"n_") || v.startsWith("\"n_"));
		});

		for (auto it = natoMarkers.begin(), end = natoMarkers.end(); it != end; ++it) {
			++markerCount;
			SqmArray* const position = (**it).getArray(fieldPosition);
			float const x = position->getEntryAsFloat(0);
			float const y = position->getEntryAsFloat(2);

			float const xShouldBe = (static_cast<int64_t>(std::trunc(x)) / markerGrid) * markerGrid + (markerGrid / 2.0);
			float const yShouldBe = (static_cast<int64_t>(std::trunc(y)) / markerGrid) * markerGrid + (markerGrid / 2.0);

			float const xDiff = std::abs(xShouldBe - x);
			float const yDiff = std::abs(yShouldBe - y);
			if (xDiff <= markerMaxDistance && yDiff <= markerMaxDistance) {
				std::cout << "Marker at (" << x << ", " << y << ") should be moved to (" << xShouldBe << ", " << yShouldBe << ")." << std::endl;
			}
		}
		std::cout << "Saw " << markerCount << " markers on map." << std::endl;
	}
}
