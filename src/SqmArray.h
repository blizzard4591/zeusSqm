#ifndef ZEUSSQM_PARSER_SQMARRAY_H_
#define ZEUSSQM_PARSER_SQMARRAY_H_

#include "SqmStructure.h"

#include <QString>
#include <QStringList>

class SqmArray : public SqmStructure {
public:
	SqmArray(QString const& name, QStringList const& values, bool isMultiLine);
	virtual ~SqmArray() {}

	QString const& getName() const;
	QStringList const& getValues() const;

	virtual QString toSqm(int indentationLevel) const override;
private:
	QString const m_name;
	QStringList const m_values;
	bool const m_isMultiLine;
};

#endif
