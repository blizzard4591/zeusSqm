#include "SqmFloatProperty.h"

#include "BinarizedSqm.h"
#include "exceptions/InternalErrorException.h"

SqmFloatProperty::SqmFloatProperty(QString const& name, float value) : SqmProperty(name), m_value(value), m_valueString(toFloatRepresentation(value)) {
	//
}

SqmFloatProperty::SqmFloatProperty(QString const& name, QString const& value) : SqmProperty(name), m_value(value.toFloat()), m_valueString(value) {
	bool ok = false;
	float const f = value.toDouble(&ok); // See https://bugreports.qt.io/browse/QTBUG-88371
	if (!ok) {
		throw zeusops::exceptions::InternalErrorException() << "Failed to parse value '" << value.toStdString() << "' in field '" << name.toStdString() << "' into float!";
	}
}

SqmFloatProperty::SqmFloatProperty(QString const& name, float value, QString const& svalue) : SqmProperty(name), m_value(value), m_valueString(svalue) {
	//
}

std::shared_ptr<SqmStructure> SqmFloatProperty::rename(QString const& newName) const {
	return std::make_shared<SqmFloatProperty>(newName, m_value, m_valueString);
}

QString const& SqmFloatProperty::getValueAsString() const {
	throw;
}

qint32 SqmFloatProperty::getValueAsInt() const {
	return m_value;
}

qint64 SqmFloatProperty::getValueAsInt64() const {
	return m_value;
}

float SqmFloatProperty::getValueAsFloat() const {
	return m_value;
}

void SqmFloatProperty::toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	// Type
	BinarizedSqm::writeUint8(output, 1);
	// SubType
	BinarizedSqm::writeUint8(output, 1);
	BinarizedSqm::writeString(output, getName());
	BinarizedSqm::writeFloat(output, m_value);
}

bool SqmFloatProperty::toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	return false;
}

QString SqmFloatProperty::toSqm(int indentationLevel, FormatType const& format) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);
	QString result;
	if (format == FormatType::NOSPACE) {
		result = QStringLiteral("%1%2=%3;\r\n");
	} else {
		result = QStringLiteral("%1%2 = %3;\r\n");
	}

	return result.arg(indentString).arg(getName()).arg(m_valueString);
}
