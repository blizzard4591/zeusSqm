#ifndef ZEUSSQM_PARSER_SQMPROPERTY_H_
#define ZEUSSQM_PARSER_SQMPROPERTY_H_

#include "SqmStructure.h"

#include <QString>

class SqmProperty : public SqmStructure {
public:
	SqmProperty(QString const& name, QString const& value);
	virtual ~SqmProperty() {}

	QString const& getValue() const;
	QString getValueAsString() const;
	int getValueAsInt() const;

	std::shared_ptr<SqmProperty> increment(int incrementBy, int* oldValue = nullptr) const;

	virtual QString toSqm(int indentationLevel) const override;
	virtual QString const& getName() const override;

	static std::shared_ptr<SqmProperty> newStringProperty(QString const& name, QString const& value);
	static std::shared_ptr<SqmProperty> newIntegerProperty(QString const& name, int value);
private:
	QString const m_name;
	QString const m_value;
};

#endif
