#include "SqmStringProperty.h"

#include "BinarizedSqm.h"
#include <QString>

SqmStringProperty::SqmStringProperty(QString const& name, QString const& value) : SqmProperty(name), m_value(value) {
	//
}

std::shared_ptr<SqmStructure> SqmStringProperty::rename(QString const& newName) const {
	return std::make_shared<SqmStringProperty>(newName, m_value);
}

QString const& SqmStringProperty::getValueAsString() const {
	return m_value;
}

qint32 SqmStringProperty::getValueAsInt() const {
	throw;
}

qint64 SqmStringProperty::getValueAsInt64() const {
	throw;
}

float SqmStringProperty::getValueAsFloat() const {
	throw;
}

void SqmStringProperty::toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	// Type
	BinarizedSqm::writeUint8(output, 1);
	// SubType
	BinarizedSqm::writeUint8(output, 0);
	BinarizedSqm::writeString(output, getName());
	BinarizedSqm::writeString(output, m_value);
}

bool SqmStringProperty::toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	return false;
}

QString SqmStringProperty::toSqm(int indentationLevel, FormatType const& format) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);
	QString result;
	if (format == FormatType::NOSPACE) {
		result = QStringLiteral("%1%2=\"%3\";\r\n");
	} else {
		result = QStringLiteral("%1%2 = \"%3\";\r\n");
	}

	return result.arg(indentString).arg(getName()).arg(escapeQuotesInString(m_value));
}
