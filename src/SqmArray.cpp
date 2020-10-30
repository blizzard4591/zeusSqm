#include "SqmArray.h"

SqmArray::SqmArray(QString const& name, QStringList const& values, bool isMultiLine) : m_name(name), m_values(values), m_isMultiLine(isMultiLine) {
	//
}

QString const& SqmArray::getName() const {
	return m_name;
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

