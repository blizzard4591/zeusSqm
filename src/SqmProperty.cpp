#include "SqmProperty.h"

#include "SqmFloatProperty.h"
#include "SqmIntProperty.h"
#include "SqmStringProperty.h"

SqmProperty::SqmProperty(QString const& name) : m_name(name) {
	//
}

QString const& SqmProperty::getName() const {
	return m_name;
}

std::shared_ptr<SqmProperty> SqmProperty::newStringProperty(QString const& name, QString const& value) {
	return std::make_shared<SqmStringProperty>(name, value);
}

std::shared_ptr<SqmProperty> SqmProperty::newIntegerProperty(QString const& name, qint32 value) {
	return std::make_shared<SqmIntProperty>(name, value);
}
