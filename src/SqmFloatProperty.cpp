#include "SqmFloatProperty.h"

#include "exceptions/InternalErrorException.h"

SqmFloatProperty::SqmFloatProperty(QString const& name, float value) : SqmProperty(name), m_value(value), m_valueString(toFloatRepresentation(value)) {
	//
}

SqmFloatProperty::SqmFloatProperty(QString const& name, QString const& value) : SqmProperty(name), m_value(value.toFloat()), m_valueString(value) {
	bool ok = false;
	float const f = value.toFloat(&ok);
	if (!ok) {
		LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Failed to parse value '" << value.toStdString() << "' in field '" << name.toStdString() << "' into float!");
	}
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

void SqmFloatProperty::toSqmStageOne(QByteArray& output) const {

}

void SqmFloatProperty::toSqmStageTwo(QByteArray& output) const {

}

QString SqmFloatProperty::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);
	return QStringLiteral("%1%2=%3;\r\n").arg(indentString).arg(getName()).arg(m_valueString);
}
