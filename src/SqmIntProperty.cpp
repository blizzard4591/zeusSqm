#include "SqmIntProperty.h"

#include "BinarizedSqm.h"

SqmIntProperty::SqmIntProperty(QString const& name, qint32 value) : SqmProperty(name), m_value(value) {
	//
}

QString const& SqmIntProperty::getValueAsString() const {
	throw;
}

qint32 SqmIntProperty::getValueAsInt() const {
	return m_value;
}

float SqmIntProperty::getValueAsFloat() const {
	throw;
}

void SqmIntProperty::toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	// Type
	BinarizedSqm::writeUint8(output, 1);
	// SubType
	BinarizedSqm::writeUint8(output, 2);
	BinarizedSqm::writeString(output, getName());
	BinarizedSqm::writeInt32(output, m_value);
}

bool SqmIntProperty::toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int> const& stageTwoOffsetMap) const {
	return false;
}

QString SqmIntProperty::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);
	return QStringLiteral("%1%2=%3;\r\n").arg(indentString).arg(getName()).arg(m_value);
}

std::shared_ptr<SqmIntProperty> SqmIntProperty::increment(int incrementBy, int* oldValue) const {
	if (incrementBy < 1) throw;
	int const valueBefore = getValueAsInt();
	if (oldValue != nullptr) {
		*oldValue = valueBefore;
	}

	return std::make_shared<SqmIntProperty>(getName(), valueBefore + incrementBy);
}
