#include "SqmArray.h"

SqmArray::SqmArray(QString const& name, QStringList const& values, bool isMultiLine) : m_name(name), m_values(values), m_isMultiLine(isMultiLine) {
	//
}

QStringList const& SqmArray::getValues() const {
	return m_values;
}

QString SqmArray::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	if (m_isMultiLine) {
		QString result = QStringLiteral("%1%2[]=\r\n").arg(indentString).arg(getName());
		result.append(indentString).append(QStringLiteral("{\r\n"));
		for (int i = 0; i < m_values.size(); ++i) {
			result.append(indentString).append('\t').append(m_values.at(i));
			if (i < (m_values.size() - 1)) {
				result.append(',');
			}
			result.append(QStringLiteral("\r\n"));
		}
		result.append(indentString).append(QStringLiteral("};\r\n"));
		return result;
	} else {
		QString result = QStringLiteral("%1%2[]={").arg(indentString).arg(getName());
		result.append(getValues().join(','));
		result.append(QStringLiteral("};\r\n"));
		return result;
	}
}

QString const& SqmArray::getName() const {
	return m_name;
}

float SqmArray::getEntryAsFloat(int index) const {
	if ((index < 0) || (index >= m_values.size())) {
		throw;
	}

	bool ok = false;
	float const result = m_values.at(index).toFloat(&ok);
	if (!ok) {
		throw;
	}

	return result;
}

