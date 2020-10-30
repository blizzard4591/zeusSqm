#ifndef ZEUSSQM_PARSER_SQMCLASS_H_
#define ZEUSSQM_PARSER_SQMCLASS_H_

#include "SqmStructure.h"
#include "SqmObjectList.h"

#include <QString>
#include <QStringList>

#include <memory>
#include <vector>

class SqmClass : public SqmStructure {
public:
	SqmClass(QString const& name, SqmObjectList const& objects);
	virtual ~SqmClass() {}

	QString const& getName() const;
	SqmObjectList const& getObjects() const;

	virtual QString toSqm(int indentationLevel) const override;
private:
	QString const m_name;
	SqmObjectList const m_objects;
};

#endif
