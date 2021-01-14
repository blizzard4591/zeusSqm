#include "SqmHandling.h"

#include <chrono>
#include <iostream>
#include "exceptions/FormatErrorException.h"

SqmRoot SqmHandling::addVrShapeObjects(SqmRoot const& root, std::vector<Position> const& positions) {
	int const maxObjectsPerLayer = 5000;

	int nextFreeId = -1;
	int const objectCount = static_cast<int>(positions.size());
	int const layerCount = objectCount / maxObjectsPerLayer + (objectCount % maxObjectsPerLayer != 0);
	SqmRoot result = nextItemIds(root, nextFreeId, objectCount + layerCount);

	int nextFreeLayerId = -1;
	result = nextLayerIds(result, nextFreeLayerId, 1);
	auto it = positions.cbegin();
	auto const end = positions.cend();
	while (it != end) {
		QString const layerName = QStringLiteral("Layer%1").arg(nextFreeLayerId++);

		SqmClass* classMission = dynamic_cast<SqmClass*>(result->getByName(QStringLiteral("Mission")));
		if (classMission == nullptr) {
			std::cerr << "FORMAT ERROR: SQM does not contain class 'Mission', is it complete/the correct file?" << std::endl;
			throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Mission', is it complete/the correct file?";
		}

		SqmClass* classEntitiesRoot = dynamic_cast<SqmClass*>(classMission->getByName(QStringLiteral("Entities")));
		if (classEntitiesRoot == nullptr) {
			std::cerr << "FORMAT ERROR: SQM does not contain class 'Entities', is it complete/the correct file?" << std::endl;
			throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Entities', is it complete/the correct file?";
		}

		qint32 const previousEntitiesItemCount = classEntitiesRoot->getIntProperty(QStringLiteral("items"))->getValueAsInt();

		std::vector<SqmObjectList<SqmStructure>> preliminaryClassItem;
		std::vector<std::shared_ptr<SqmStructure>> itemContents;
		itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("dataType"), QStringLiteral("Layer")));
		itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("name"), layerName));
		itemContents.push_back(SqmProperty::newIntegerProperty(QStringLiteral("state"), 1));
		itemContents.push_back(std::make_shared<SqmClass>(QStringLiteral("Entities"), SqmObjectList<SqmStructure>(QString(), { SqmProperty::newIntegerProperty(QStringLiteral("items"), 0) })));
		itemContents.push_back(SqmProperty::newIntegerProperty(QStringLiteral("id"), nextFreeId + static_cast<qint32>(positions.size())));
		preliminaryClassItem.push_back(SqmObjectList<SqmStructure>(QString(), itemContents));
		result = classEntitiesRoot->insertClassItemsWithItemCountIncrement(preliminaryClassItem, result);

		classMission = dynamic_cast<SqmClass*>(result->getByName(QStringLiteral("Mission")));
		if (classMission == nullptr) {
			std::cerr << "FORMAT ERROR: SQM does not contain class 'Mission', is it complete/the correct file?" << std::endl;
			throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Mission', is it complete/the correct file?";
		}

		classEntitiesRoot = dynamic_cast<SqmClass*>(classMission->getByName(QStringLiteral("Entities")));
		if (classEntitiesRoot == nullptr) {
			std::cerr << "FORMAT ERROR: SQM does not contain class 'Entities', is it complete/the correct file?" << std::endl;
			throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Entities', is it complete/the correct file?";
		}

		SqmClass* classLayer = dynamic_cast<SqmClass*>(classEntitiesRoot->getByName(QStringLiteral("Item%1").arg(previousEntitiesItemCount)));
		if (classLayer == nullptr || (classLayer->getStringProperty(QStringLiteral("name"))->getValueAsString().compare(layerName) != 0)) {
			std::cerr << "FORMAT ERROR: SQM does not contain newly inserted layer, is it complete/the correct file?" << std::endl;
			throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain newly inserted layer, is it complete/the correct file?";
		}

		SqmClass* classEntities = dynamic_cast<SqmClass*>(classLayer->getByName(QStringLiteral("Entities")));
		if (classEntities == nullptr) {
			std::cerr << "FORMAT ERROR: SQM does not contain class 'Entities', is it complete/the correct file?" << std::endl;
			throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'Entities', is it complete/the correct file?";
		}

		std::vector<SqmObjectList<SqmStructure>> preliminaryClassItems;
		for (int currentRoundCount = 0; (currentRoundCount < maxObjectsPerLayer) && (it != end); ++currentRoundCount, ++it) {
			std::vector<std::shared_ptr<SqmStructure>> itemContents;
			itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("dataType"), QStringLiteral("Object")));
			itemContents.push_back(newPosition(it->x, it->y, it->z));
			itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("side"), QStringLiteral("Empty")));
			itemContents.push_back(SqmProperty::newIntegerProperty(QStringLiteral("flags"), 5));
			itemContents.push_back(std::make_shared<SqmClass>(QStringLiteral("Attributes"), SqmObjectList<SqmStructure>(QString(), {})));
			itemContents.push_back(SqmProperty::newIntegerProperty(QStringLiteral("id"), nextFreeId++));
			itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("type"), QStringLiteral("Land_VR_Shape_01_cube_1m_F")));
			preliminaryClassItems.push_back(SqmObjectList<SqmStructure>(QString(), itemContents));
		}

		result = classEntities->insertClassItemsWithItemCountIncrement(preliminaryClassItems, result);
	}

	return result;
}

SqmRoot SqmHandling::addObjects(SqmRoot const& root, QString const& name, std::vector<FloatPosition> const& positions) {
	int nextFreeId = -1;
	SqmRoot result = nextItemIds(root, nextFreeId, static_cast<int>(positions.size()));

	std::vector<SqmObjectList<SqmStructure>> preliminaryClassItems;
	for (auto it = positions.cbegin(), end = positions.cend(); it != end; ++it) {
		std::vector<std::shared_ptr<SqmStructure>> itemContents;
		itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("dataType"), QStringLiteral("Object")));
		itemContents.push_back(newPosition(it->x, it->y, it->z));
		itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("side"), QStringLiteral("Empty")));
		itemContents.push_back(SqmProperty::newIntegerProperty(QStringLiteral("flags"), 5));
		itemContents.push_back(std::make_shared<SqmClass>(QStringLiteral("Attributes"), SqmObjectList<SqmStructure>(QString(), {})));
		itemContents.push_back(SqmProperty::newIntegerProperty(QStringLiteral("id"), nextFreeId++));
		itemContents.push_back(SqmProperty::newStringProperty(QStringLiteral("type"), name));
		preliminaryClassItems.push_back(SqmObjectList<SqmStructure>(QString(), itemContents));
	}

	SqmClass* classMission = dynamic_cast<SqmClass*>(result->getByName(QStringLiteral("Mission")));
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
	std::vector<SqmArrayContents::ArrayEntry> positions = { x, z, y };
	std::shared_ptr<SqmArray> positionArray = std::make_shared<SqmArray>(QStringLiteral("position"), positions);
	return std::make_shared<SqmClass>(QStringLiteral("PositionInfo"), SqmObjectList<SqmStructure>(QString(), { positionArray }));
}

std::shared_ptr<SqmClass> SqmHandling::newPosition(float x, float y, float z) {
	std::vector<SqmArrayContents::ArrayEntry> positions = { x, z, y };
	std::shared_ptr<SqmArray> positionArray = std::make_shared<SqmArray>(QStringLiteral("position"), positions);
	return std::make_shared<SqmClass>(QStringLiteral("PositionInfo"), SqmObjectList<SqmStructure>(QString(), { positionArray }));
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

	std::shared_ptr<SqmIntProperty> nextIdProperty = classItemIdProvider->getIntProperty(QStringLiteral("nextID"));
	std::shared_ptr<SqmIntProperty> newNextIdProperty = nextIdProperty->increment(requestedIdCount, &firstNextId);

	return root->replace(*nextIdProperty, newNextIdProperty, root);
}

SqmRoot SqmHandling::nextLayerIds(SqmRoot const& root, int& firstNextId, int requestedIdCount) {
	if (requestedIdCount < 1) throw;
	SqmClass* classEditorData = dynamic_cast<SqmClass*>(root->getByName(QStringLiteral("EditorData")));
	if (classEditorData == nullptr) {
		std::cerr << "FORMAT ERROR: SQM does not contain class 'EditorData', is it complete/the correct file?" << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "FORMAT ERROR: SQM does not contain class 'EditorData', is it complete/the correct file?";
	}

	SqmClass* classLayerIndexProvider = dynamic_cast<SqmClass*>(classEditorData->getByName(QStringLiteral("LayerIndexProvider")));
	if (classLayerIndexProvider == nullptr) {
		// No layers yet, no problem
		return nextLayerIds(classEditorData->add(std::make_shared<SqmClass>(QStringLiteral("LayerIndexProvider"), SqmObjectList<SqmStructure>(QString(), { SqmProperty::newIntegerProperty(QStringLiteral("nextID"), 1) })), root), firstNextId, requestedIdCount);
	}

	std::shared_ptr<SqmIntProperty> nextIdProperty = classLayerIndexProvider->getIntProperty(QStringLiteral("nextID"));
	std::shared_ptr<SqmIntProperty> newNextIdProperty = nextIdProperty->increment(requestedIdCount, &firstNextId);

	return root->replace(*nextIdProperty, newNextIdProperty, root);
}

