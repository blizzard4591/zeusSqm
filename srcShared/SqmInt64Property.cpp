#include "SqmInt64Property.h"

#include "BinarizedSqm.h"

SqmInt64Property::SqmInt64Property(QString const& name, qint64 value) : SqmProperty(name), m_value(value) {
	//
}

std::shared_ptr<SqmStructure> SqmInt64Property::rename(QString const& newName) const {
	return std::make_shared<SqmInt64Property>(newName, m_value);
}

QString const& SqmInt64Property::getValueAsString() const {
	throw;
}

qint32 SqmInt64Property::getValueAsInt() const {
	return m_value;
}

qint64 SqmInt64Property::getValueAsInt64() const {
	return m_value;
}

float SqmInt64Property::getValueAsFloat() const {
	throw;
}

void SqmInt64Property::toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	// Type
	BinarizedSqm::writeUint8(output, 1);
	// SubType
	BinarizedSqm::writeUint8(output, 6);
	BinarizedSqm::writeString(output, getName());
	BinarizedSqm::writeInt64(output, m_value);
}

bool SqmInt64Property::toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	return false;
}

QString SqmInt64Property::toSqm(int indentationLevel, FormatType const& format) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);
	QString result;
	if (format == FormatType::NOSPACE) {
		result = QStringLiteral("%1%2=%3;\r\n");
	} else {
		result = QStringLiteral("%1%2 = %3;\r\n");
	}

	return result.arg(indentString).arg(getName()).arg(m_value);
}

std::shared_ptr<SqmInt64Property> SqmInt64Property::increment(qint64 incrementBy, qint64* oldValue) const {
	if (incrementBy < 1) throw;
	int const valueBefore = getValueAsInt();
	if (oldValue != nullptr) {
		*oldValue = valueBefore;
	}

	return std::make_shared<SqmInt64Property>(getName(), valueBefore + incrementBy);
}

std::shared_ptr<SqmInt64Property> SqmInt64Property::decrement(qint64 decrementBy, qint64* oldValue) const {
	if (decrementBy < 1) throw;
	qint64 const valueBefore = getValueAsInt64();
	if (oldValue != nullptr) {
		*oldValue = valueBefore;
	}

	return std::make_shared<SqmInt64Property>(getName(), valueBefore - decrementBy);
}
