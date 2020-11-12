#ifndef ZEUSSQM_PARSER_SQMDELETECLASS_H_
#define ZEUSSQM_PARSER_SQMDELETECLASS_H_

#include "SqmStructure.h"

#include <QString>

class SqmDeleteClass : public SqmStructure {
public:
	SqmDeleteClass(QString const& name);
	virtual ~SqmDeleteClass() {}

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual bool toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel, FormatType const& format) const override;
	virtual QString const& getName() const override;
	virtual std::shared_ptr<SqmStructure> rename(QString const& newName) const override;
private:
	QString const m_name;
};

#endif
