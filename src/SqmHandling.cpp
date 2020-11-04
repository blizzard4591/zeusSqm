#include "SqmHandling.h"

#include <chrono>
#include <iostream>
#include "exceptions/FormatErrorException.h"

SqmRoot SqmHandling::addVrShapeObjects(SqmRoot const& root, std::vector<Position> const& positions) {
	int nextFreeId = -1;
	SqmRoot result = nextItemIds(root, nextFreeId, static_cast<int>(positions.size()));

	std::vector<SqmObjectList<SqmStructure>> preliminaryClassItems;
	for (auto it = positions.cbegin(), end = positions.cend(); it != end; ++it) {
		std::vector<std::shared_ptr<SqmStructure>> itemContents;
		itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("dataType"), QStringLiteral("Object")));
		itemContents.push_back(newPosition(it->x, it->y, it->z));
		itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("side"), QStringLiteral("Empty")));
		itemContents.push_back(SqmProperty::newIntegerProperty(QStringLiteral("flags"), 5));
		itemContents.push_back(std::make_shared<SqmClass>(QStringLiteral("Attributes"), SqmObjectList<SqmStructure>({})));
		itemContents.push_back(SqmProperty::newIntegerProperty(QStringLiteral("id"), nextFreeId++));
		itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("type"), QStringLiteral("Land_VR_Shape_01_cube_1m_F")));
		preliminaryClassItems.push_back(SqmObjectList<SqmStructure>(itemContents));
	}
	
	SqmClass* classMission = dynamic_cast<SqmClass*>(root->getByName(QStringLiteral("Mission")));
	if (classMission == nullptr) {
		std::cerr << "FORMAT ERROR: SQM does not contain class 'Mission', is it complete/the correct file?" << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Mission', is it complete/the correct file?";
	}

	SqmClass* classEntities = dynamic_cast<SqmClass*>(classMission->getByName(QStringLiteral("Entities")));
	if (classEntities == nullptr) {
		std::cerr << "FORMAT ERROR: SQM does not contain class 'Entities', is it complete/the correct file?" << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Entities', is it complete/the correct file?";
	}

	return classEntities->insertClassItemsWithItemCountIncrement(preliminaryClassItems, result);
}

std::shared_ptr<SqmClass> SqmHandling::newPosition(int x, int y, int z) {
	QStringList positions = { QString::number(x), QString::number(z), QString::number(y) };
	std::shared_ptr<SqmArray> positionArray = std::make_shared<SqmArray>(QStringLiteral("position"), positions, false);
	return std::make_shared<SqmClass>(QStringLiteral("PositionInfo"), SqmObjectList<SqmStructure>({ positionArray }));
}

SqmRoot SqmHandling::nextItemIds(SqmRoot const& root, int& firstNextId, int requestedIdCount) {
	if (requestedIdCount < 1) throw;
	SqmClass* classEditorData = dynamic_cast<SqmClass*>(root->getByName(QStringLiteral("EditorData")));
	if (classEditorData == nullptr) {
		std::cerr << "FORMAT ERROR: SQM does not contain class 'EditorData', is it complete/the correct file?" << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'EditorData', is it complete/the correct file?";
	}

	SqmClass* classItemIdProvider = dynamic_cast<SqmClass*>(classEditorData->getByName(QStringLiteral("ItemIDProvider")));
	if (classItemIdProvider == nullptr) {
		std::cerr << "FORMAT ERROR: SQM does not contain class 'ItemIDProvider', is it complete/the correct file?" << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'ItemIDProvider', is it complete/the correct file?";
	}

	std::shared_ptr<SqmProperty> nextIdProperty = classItemIdProvider->getProperty(QStringLiteral("nextID"));
	std::shared_ptr<SqmProperty> newNextIdProperty = nextIdProperty->increment(requestedIdCount, &firstNextId);

	return root->replace(*nextIdProperty, newNextIdProperty, root);
}
