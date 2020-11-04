#include "SqmClass.h"

SqmClass::SqmClass(QString const& name, SqmObjectList const& objects) : SqmObjectList(objects), m_name(name) {
	//
}

QString SqmClass::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	QString result = QStringLiteral("%1class %2\r\n").arg(indentString).arg(getName());
	result.append(indentString).append(QStringLiteral("{\r\n"));
	result.append(SqmObjectList::toSqm(indentationLevel + 1));
	result.append(indentString).append(QStringLiteral("};\r\n"));
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

SqmRoot SqmClass::insertClassItemWithItemCountIncrement(SqmObjectList<SqmStructure> const& itemObjects, SqmRoot const& root) const {
	// 1. Get current item count in ourself for ItemName
	std::shared_ptr<SqmProperty> const itemsProperty = this->getProperty(QStringLiteral("items"));
	int currentItemCount = -1;
	std::shared_ptr<SqmProperty> const newItemsProperty = itemsProperty->increment(&currentItemCount);
	// 2. New ItemName
	QString const itemName = QStringLiteral("Item%1").arg(currentItemCount);
	// 3. Fix our content (item count + new Item)
	std::vector<std::shared_ptr<SqmStructure>> objects;
	for (std::size_t i = 0; i < getObjects().size(); ++i) {
		if (*getObjects().at(i) == *itemsProperty) {
			objects.push_back(newItemsProperty);
		} else {
			objects.push_back(getObjects().at(i));
		}
	}

	std::shared_ptr<SqmClass> newItem = std::make_shared<SqmClass>(itemName, itemObjects);
	objects.push_back(newItem);
	std::shared_ptr<SqmClass> newSelf = std::make_shared<SqmClass>(getName(), objects);
	return root->replace(*this, newSelf, root);
}
