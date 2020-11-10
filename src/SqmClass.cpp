#include "SqmClass.h"

#include "BinarizedSqm.h"

SqmClass::SqmClass(QString const& name, SqmObjectList const& objects) : SqmObjectList(objects), m_name(name) {
	//
}

void SqmClass::toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	// Type
	BinarizedSqm::writeUint8(output, 0);
	// Class Name
	BinarizedSqm::writeString(output, getName());
	// Offset to Body
	stageTwoOffsetMap.insert(this, BinarizedSqm::writeUint32(output, 0));
}

QString SqmClass::toSqm(int indentationLevel, FormatType const& format) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	QString result = QStringLiteral("%1class %2").arg(indentString).arg(getName());
	if ((format == FormatType::SINGLESPACED) && (getObjects().size() == 0)) {
		result.append(QStringLiteral("{};\r\n"));
	} else {
		result.append(QStringLiteral("\r\n")).append(indentString).append(QStringLiteral("{\r\n"));
		result.append(SqmObjectList::toSqm(indentationLevel + 1, format));
		result.append(indentString).append(QStringLiteral("};\r\n"));
	}
	return result;
}

QString const& SqmClass::getName() const {
	return m_name;
}

std::shared_ptr<SqmClass> SqmClass::replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, std::shared_ptr<SqmClass> current) const {
	bool hasChange = false;
	std::vector<std::shared_ptr<SqmStructure>> objects = SqmObjectList<SqmStructure>::replace(old, newStructure, hasChange);
	if (!hasChange) {
		return current;
	}
	return std::make_shared<SqmClass>(m_name, objects);
}

std::shared_ptr<SqmClass> SqmClass::remove(SqmStructure const& old, std::shared_ptr<SqmClass> current) const {
	bool hasChange = false;
	std::vector<std::shared_ptr<SqmStructure>> objects = SqmObjectList<SqmStructure>::remove(old, hasChange);
	if (!hasChange) {
		return current;
	}
	return std::make_shared<SqmClass>(m_name, objects);
}

SqmRoot SqmClass::insertClassItemsWithItemCountIncrement(std::vector<SqmObjectList<SqmStructure>> const& itemObjects, SqmRoot const& root) const {
	// 1. Get current item count in ourself for ItemName
	std::shared_ptr<SqmIntProperty> const itemsProperty = this->getIntProperty(QStringLiteral("items"));
	int currentItemCount = -1;
	std::shared_ptr<SqmIntProperty> const newItemsProperty = itemsProperty->increment(static_cast<int>(itemObjects.size()), &currentItemCount);
	// 2. Fix our content (item count + new Item)
	std::vector<std::shared_ptr<SqmStructure>> objects;
	for (std::size_t i = 0; i < getObjects().size(); ++i) {
		if (*getObjects().at(i) == *itemsProperty) {
			objects.push_back(newItemsProperty);
		} else {
			objects.push_back(getObjects().at(i));
		}
	}

	for (auto it = itemObjects.cbegin(), end = itemObjects.cend(); it != end; ++it) {
		// 3. New ItemName
		QString const itemName = QStringLiteral("Item%1").arg(currentItemCount++);
		std::shared_ptr<SqmClass> newItem = std::make_shared<SqmClass>(itemName, *it);
		objects.push_back(newItem);
	}
	
	std::shared_ptr<SqmClass> newSelf = std::make_shared<SqmClass>(getName(), objects);
	return root->replace(*this, newSelf, root);
}
