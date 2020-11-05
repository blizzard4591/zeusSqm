#include "SqmFloatProperty.h"

SqmFloatProperty::SqmFloatProperty(QString const& name, float value) : SqmProperty(name), m_value(value), m_valueString(toFloatRepresentation(value)) {
	//
}

SqmFloatProperty::SqmFloatProperty(QString const& name, QString const& value) : SqmProperty(name), m_value(value.toFloat()), m_valueString(value) {
	//
}

QString const& SqmFloatProperty::getValueAsString() const {
	throw;
}

qint32 SqmFloatProperty::getValueAsInt() const {
	return m_value;
}

float SqmFloatProperty::getValueAsFloat() const {
	return m_value;
}

QString SqmFloatProperty::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);
	return QStringLiteral("%1%2=%3;\r\n").arg(indentString).arg(getName()).arg(m_valueString);
}
