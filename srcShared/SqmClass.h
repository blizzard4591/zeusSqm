#ifndef ZEUSSQM_PARSER_SQMCLASS_H_
#define ZEUSSQM_PARSER_SQMCLASS_H_

#include "SqmObjectList.h"

#include <QSet>
#include <QString>
#include <QStringList>

#include <vector>

class SqmClass : public SqmObjectList<SqmStructure> {
public:
	SqmClass(QString const& name, SqmObjectList<SqmStructure> const& objects);
	virtual ~SqmClass() {}

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel, FormatType const& format) const override;
	virtual QString const& getName() const override;
	virtual std::shared_ptr<SqmStructure> rename(QString const& newName) const override;

	SqmRoot add(std::shared_ptr<SqmStructure> const& object, SqmRoot const& root) const;
	SqmRoot insertClassItemsWithItemCountIncrement(std::vector<SqmObjectList<SqmStructure>> const& objects, SqmRoot const& root) const;
	SqmRoot removeClassItemsWithItemCountDecrement(QSet<SqmStructure const*> const& objectsToBeRemoved, SqmRoot const& root) const;

	std::shared_ptr<SqmClass> replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, std::shared_ptr<SqmClass> current) const;
	std::shared_ptr<SqmClass> remove(SqmStructure const& old, std::shared_ptr<SqmClass> current) const;
private:
	QString const m_name;
};

#endif
