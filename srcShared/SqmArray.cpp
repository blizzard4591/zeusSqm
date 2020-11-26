#include "SqmArray.h"

#include "BinarizedSqm.h"

SqmArray::SqmArray(QString const& name, std::vector<ArrayEntry> const& entries) : SqmArrayContents(entries), m_name(name) {
	//
}

SqmArray::SqmArray(QString const& name, SqmArrayContents const& entries) : SqmArrayContents(entries), m_name(name) {
	//
}

void SqmArray::toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	// Type
	BinarizedSqm::writeUint8(output, 2);
	// Array Name
	BinarizedSqm::writeString(output, getName());
	SqmArrayContents::toSqmStageOne(output);
}

bool SqmArray::toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	return false;
}

QString SqmArray::toSqm(int indentationLevel, FormatType const& format) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	bool isMultiline = false;
	QString const subValue = SqmArrayContents::toSqm(indentationLevel, isMultiline, format);

	QString result;
	if (isMultiline) {
		if (format == FormatType::NOSPACE) {
			result = QStringLiteral("%1%2[]=\r\n");
		} else {
			result = QStringLiteral("%1%2[] = \r\n");
		}
		result = result.arg(indentString).arg(getName());
		result.append(subValue).append(QStringLiteral(";\r\n"));
		return result;
	} else {
		if (format == FormatType::NOSPACE) {
			result = QStringLiteral("%1%2[]=");
		} else {
			result = QStringLiteral("%1%2[] = ");
		}
		result = result.arg(indentString).arg(getName());
		result.append(subValue);
		result.append(QStringLiteral(";\r\n"));
		return result;
	}
}

QString const& SqmArray::getName() const {
	return m_name;
}

std::shared_ptr<SqmStructure> SqmArray::rename(QString const& newName) const {
	return std::make_shared<SqmArray>(newName, getValues());
}

SqmArray SqmArray::setEntry(int index, float value) const {
	if ((index < 0) || (static_cast<std::size_t>(index) >= getValues().size())) {
		throw;
	}

	std::vector<ArrayEntry> values = getValues();
	values[index].content = value;
	values[index].type = ArrayEntryType::FLOAT;
	values[index].stringValue = QString();

	return SqmArray(m_name, values);
}
