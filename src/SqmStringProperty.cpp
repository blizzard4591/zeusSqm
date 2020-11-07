#include "SqmStringProperty.h"

#include <QString>

SqmStringProperty::SqmStringProperty(QString const& name, QString const& value) : SqmProperty(name), m_value(value) {
	//
}

QString const& SqmStringProperty::getValueAsString() const {
	return m_value;
}

qint32 SqmStringProperty::getValueAsInt() const {
	throw;
}

float SqmStringProperty::getValueAsFloat() const {
	throw;
}

void SqmStringProperty::toSqmStageOne(QByteArray& output) const {

}

void SqmStringProperty::toSqmStageTwo(QByteArray& output) const {

}

QString SqmStringProperty::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);
	return QStringLiteral("%1%2=\"%3\";\r\n").arg(indentString).arg(getName()).arg(escapeQuotesInString(m_value));
}
