#include "SqmArrayWithFlags.h"

#include "BinarizedSqm.h"

SqmArrayWithFlags::SqmArrayWithFlags(QString const& name, quint32 flags, std::vector<ArrayEntry> const& entries) : SqmArrayContents(entries), m_name(name), m_flags(flags) {
	//
}

SqmArrayWithFlags::SqmArrayWithFlags(QString const& name, quint32 flags, SqmArrayContents const& entries) : SqmArrayContents(entries), m_name(name), m_flags(flags) {
	//
}

void SqmArrayWithFlags::toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	// Type
	BinarizedSqm::writeUint8(output, 5);
	// Flags
	BinarizedSqm::writeUint32(output, m_flags);
	// Array Name
	BinarizedSqm::writeString(output, getName());
	SqmArrayContents::toSqmStageOne(output);
}

bool SqmArrayWithFlags::toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	return false;
}

QString SqmArrayWithFlags::toSqm(int indentationLevel, FormatType const& format) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	bool isMultiline = false;
	QString const subValue = SqmArrayContents::toSqm(indentationLevel, isMultiline, format);

	QString result;
	if (isMultiline) {
		if (format == FormatType::NOSPACE) {
			result = QStringLiteral("%1%2[]+=\r\n");
		} else {
			result = QStringLiteral("%1%2[] += \r\n");
		}
		result = result.arg(indentString).arg(getName());
		result.append(subValue).append(QStringLiteral(";\r\n"));
		return result;
	} else {
		if (format == FormatType::NOSPACE) {
			result = QStringLiteral("%1%2[]+=");
		} else {
			result = QStringLiteral("%1%2[] += ");
		}
		result = result.arg(indentString).arg(getName());
		result.append(subValue);
		result.append(QStringLiteral(";\r\n"));
		return result;
	}
}

QString const& SqmArrayWithFlags::getName() const {
	return m_name;
}

std::shared_ptr<SqmStructure> SqmArrayWithFlags::rename(QString const& newName) const {
	return std::make_shared<SqmArrayWithFlags>(newName, m_flags, getValues());
}

SqmArrayWithFlags SqmArrayWithFlags::setEntry(int index, float value) const {
	if ((index < 0) || (index >= getValues().size())) {
		throw;
	}

	std::vector<ArrayEntry> values = getValues();
	values[index].content = value;
	values[index].type = ArrayEntryType::FLOAT;
	values[index].stringValue = QString();

	return SqmArrayWithFlags(m_name, m_flags, values);
}
