#include "SqmProperty.h"

SqmProperty::SqmProperty(QString const& name, QString const& value) : m_name(name), m_value(value) {
	//
}

QString const& SqmProperty::getValue() const {
	return m_value;
}

QString SqmProperty::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);
	return QStringLiteral("%1%2=%3;\r\n").arg(indentString).arg(getName()).arg(getValue());
}

QString const& SqmProperty::getName() const {
	return m_name;
}
