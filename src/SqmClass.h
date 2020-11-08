#ifndef ZEUSSQM_PARSER_SQMCLASS_H_
#define ZEUSSQM_PARSER_SQMCLASS_H_

#include "SqmObjectList.h"

#include <QString>
#include <QStringList>

#include <vector>

class SqmClass : public SqmObjectList<SqmStructure> {
public:
	SqmClass(QString const& name, SqmObjectList<SqmStructure> const& objects);
	virtual ~SqmClass() {}

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel) const override;
	virtual QString const& getName() const override;

	SqmRoot insertClassItemsWithItemCountIncrement(std::vector<SqmObjectList<SqmStructure>> const& objects, SqmRoot const& root) const;

	std::shared_ptr<SqmClass> replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, std::shared_ptr<SqmClass> current) const;
	std::shared_ptr<SqmClass> remove(SqmStructure const& old, std::shared_ptr<SqmClass> current) const;
private:
	QString const m_name;
};

#endif
