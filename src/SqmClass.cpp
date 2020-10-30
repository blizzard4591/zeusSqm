#include "SqmClass.h"

SqmClass::SqmClass(QString const& name, SqmObjectList const& objects) : m_name(name), m_objects(objects) {
	//
}

QString const& SqmClass::getName() const {
	return m_name;
}

SqmObjectList const& SqmClass::getObjects() const {
	return m_objects;
}

QString SqmClass::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	QString result = QStringLiteral("%1class %2\r\n").arg(indentString).arg(getName());
	result.append(indentString).append(QStringLiteral("{\r\n"));
	result.append(m_objects.toSqm(indentationLevel + 1));
	result.append(indentString).append(QStringLiteral("};\r\n"));
	return result;
}

