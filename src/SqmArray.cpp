#include "SqmArray.h"

SqmArray::SqmArray(QString const& name, std::vector<ArrayEntry> const& entries) : SqmArrayContents(entries), m_name(name) {
	//
}

SqmArray::SqmArray(QString const& name, SqmArrayContents const& entries) : SqmArrayContents(entries), m_name(name) {
	//
}

void SqmArray::toSqmStageOne(QByteArray& output) const {

}

void SqmArray::toSqmStageTwo(QByteArray& output) const {

}

QString SqmArray::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	bool isMultiline = false;
	QString const subValue = SqmArrayContents::toSqm(indentationLevel, isMultiline);

	if (isMultiline) {
		QString result = QStringLiteral("%1%2[]=\r\n").arg(indentString).arg(getName());
		result.append(subValue).append(QStringLiteral(";\r\n"));
		return result;
	} else {
		QString result = QStringLiteral("%1%2[]=").arg(indentString).arg(getName());
		result.append(subValue);
		result.append(QStringLiteral(";\r\n"));
		return result;
	}
}

QString const& SqmArray::getName() const {
	return m_name;
}

SqmArray SqmArray::setEntry(int index, float value) const {
	if ((index < 0) || (index >= getValues().size())) {
		throw;
	}

	std::vector<ArrayEntry> values = getValues();
	values[index].content = value;
	values[index].type = ArrayEntryType::FLOAT;
	values[index].stringValue = QString();

	return SqmArray(m_name, values);
}
