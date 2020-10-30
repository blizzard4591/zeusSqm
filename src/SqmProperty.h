#ifndef ZEUSSQM_PARSER_SQMPROPERTY_H_
#define ZEUSSQM_PARSER_SQMPROPERTY_H_

#include "SqmStructure.h"

#include <QString>

class SqmProperty : public SqmStructure {
public:
	SqmProperty(QString const& name, QString const& value);
	virtual ~SqmProperty() {}

	QString const& getName() const;
	QString const& getValue() const;

	virtual QString toSqm(int indentationLevel) const override;
private:
	QString const m_name;
	QString const m_value;
};

#endif
