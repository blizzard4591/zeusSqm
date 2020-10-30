#include "SqmObjectList.h"

SqmObjectList::SqmObjectList(std::vector<std::shared_ptr<SqmStructure>> const& objects) : m_objects(objects) {
	//
}

std::vector<std::shared_ptr<SqmStructure>> const& SqmObjectList::getObjects() const {
	return m_objects;
}

QString SqmObjectList::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	QString result;
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		result.append(m_objects.at(i)->toSqm(indentationLevel));
	}
	return result;
}

