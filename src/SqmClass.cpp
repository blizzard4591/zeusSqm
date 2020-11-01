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
	if (*this == old) {
		std::shared_ptr<SqmClass> const newClassStructure = std::dynamic_pointer_cast<SqmClass>(newStructure);
		if (newClassStructure == nullptr) {
			throw;
		}

		return newClassStructure;
	}

	bool hasChange = false;
	std::vector<std::shared_ptr<SqmStructure>> objects = SqmObjectList<SqmStructure>::replace(old, newStructure, hasChange);
	if (!hasChange) {
		return current;
	}
	return std::make_shared<SqmClass>(m_name, objects);
}
