#ifndef ZEUSSQM_PARSER_SQMSTRINGPROPERTY_H_
#define ZEUSSQM_PARSER_SQMSTRINGPROPERTY_H_

#include "SqmProperty.h"

#include <QString>

class SqmStringProperty : public SqmProperty {
public:
	SqmStringProperty(QString const& name, QString const& value);
	virtual ~SqmStringProperty() {}

	virtual Type getPropertyType() const override {
		return Type::STRING;
	}

	virtual QString const& getValueAsString() const override;
	virtual qint32 getValueAsInt() const override;
	virtual float getValueAsFloat() const override;

	virtual QString toSqm(int indentationLevel) const override;
private:
	QString const m_value;
};

#endif
