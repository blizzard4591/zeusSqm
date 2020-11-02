#include "StatisticsCheckModule.h"

#include <iostream>

#include "SqmArray.h"
#include "SqmClass.h"
#include "SqmProperty.h"
#include "exceptions/FormatErrorException.h"

StatisticsCheckModule::StatisticsCheckModule() :
	CheckModule(),
	setBluforRelationOption(QStringList() << "setBluforVsIndependent", QCoreApplication::translate("main", "Overwrite side relations between BLUFOR and independent"), QCoreApplication::translate("main", "<new relation>, either 'war' or 'peace'")),
	setOpforRelationOption(QStringList() << "setBluforVsIndependent", QCoreApplication::translate("main", "Overwrite side relations between BLUFOR and independent"), QCoreApplication::translate("main", "<new relation>, either 'war' or 'peace'")),
	changeBluforRelation(false), bluforAtWar(false), changeOpforRelation(false), opforAtWar(false)
{
	//
}

void StatisticsCheckModule::registerOptions(QCommandLineParser& parser) {
	parser.addOption(setBluforRelationOption);
	parser.addOption(setOpforRelationOption);
}

bool StatisticsCheckModule::checkArguments(QCommandLineParser& parser) {
	if (parser.isSet(setBluforRelationOption)) {
		changeBluforRelation = true;
		QString const newRelation = parser.value(setBluforRelationOption);
		if (newRelation.compare("war", Qt::CaseInsensitive) == 0) {
			bluforAtWar = true;
		} else if (newRelation.compare("peace", Qt::CaseInsensitive) == 0) {
			bluforAtWar = false;
		} else {
			std::cerr << "New BLUFOR relation needs to be either war or peace!" << std::endl;
			return false;
		}
	}

	if (parser.isSet(setOpforRelationOption)) {
		changeOpforRelation = true;
		QString const newRelation = parser.value(setOpforRelationOption);
		if (newRelation.compare("war", Qt::CaseInsensitive) == 0) {
			opforAtWar = true;
		} else if (newRelation.compare("peace", Qt::CaseInsensitive) == 0) {
			opforAtWar = false;
		} else {
			std::cerr << "New OPFOR relation needs to be either war or peace!" << std::endl;
			return false;
		}
	}

	return true;
}

std::shared_ptr<SqmObjectList<SqmStructure>> StatisticsCheckModule::perform(std::shared_ptr<SqmObjectList<SqmStructure>> const& sqmObjects) {
	std::shared_ptr<SqmObjectList<SqmStructure>> result = sqmObjects;

	std::cout << "Showing mission data..." << std::endl;
	
	SqmClass* classMission = dynamic_cast<SqmClass*>(sqmObjects->getByName(QStringLiteral("Mission")));
	if (classMission == nullptr) {
		std::cerr << "FORMAT ERROR: SQM does not contain class 'Mission', is it complete/the correct file?" << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Mission', is it complete/the correct file?";
	}

	SqmClass* classIntel = dynamic_cast<SqmClass*>(classMission->getByName(QStringLiteral("Intel")));
	if (classIntel == nullptr) {
		std::cerr << "FORMAT ERROR: SQM does not contain class 'Intel', is it complete/the correct file?" << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Intel', is it complete/the correct file?";
	}

	// resistanceWest = 1 means PEACE, default
	// resistanceEast = 0 means WAR, default
	bool const independentAtWarWithBlufor = (classIntel->hasProperty(QStringLiteral("resistanceWest"))) && (classIntel->getPropertyValue(QStringLiteral("resistanceWest")).compare("0") == 0);
	bool const independentAtWarWithOpfor = (!classIntel->hasProperty(QStringLiteral("resistanceEast"))) || (classIntel->getPropertyValue(QStringLiteral("resistanceEast")).compare("0") == 0);

	std::cout << "Current Side relations:" << std::endl;
	std::cout << "BLUFOR vs. Independent: " << (independentAtWarWithBlufor ? "WAR" : "peaceful") << std::endl;
	std::cout << "OPFOR vs. Independent: " << (independentAtWarWithOpfor ? "WAR" : "peaceful") << std::endl;

	return result;
}
