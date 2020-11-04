#include "SqmProperty.h"

SqmProperty::SqmProperty(QString const& name, QString const& value) : m_name(name), m_value(value) {
	//
}

QString const& SqmProperty::getValue() const {
	return m_value;
}

QString SqmProperty::getValueAsString() const {
	if (m_value.startsWith('"') && m_value.endsWith('"')) {
		return m_value.mid(1, m_value.length() - 2);
	}
	throw;
}

int SqmProperty::getValueAsInt() const {
	bool ok = false;
	int result = m_value.toInt(&ok);
	if (!ok) {
		throw;
	}
	return result;
}

std::shared_ptr<SqmProperty> SqmProperty::increment(int* oldValue) const {
	int const valueBefore = getValueAsInt();
	if (oldValue != nullptr) {
		*oldValue = valueBefore;
	}

	return std::make_shared<SqmProperty>(getName(), QString::number(valueBefore + 1));
}

QString SqmProperty::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);
	return QStringLiteral("%1%2=%3;\r\n").arg(indentString).arg(getName()).arg(getValue());
}

QString const& SqmProperty::getName() const {
	return m_name;
}

std::shared_ptr<SqmProperty> SqmProperty::newStringProperty(QString const& name, QString const& value) {
	return std::make_shared<SqmProperty>(name, QStringLiteral("\"%1\"").arg(value));
}

std::shared_ptr<SqmProperty> SqmProperty::newIntegerProperty(QString const& name, int value) {
	return std::make_shared<SqmProperty>(name, QString::number(value));
}
