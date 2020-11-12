#ifndef ZEUSSQM_PARSER_SQMEXTERNCLASS_H_
#define ZEUSSQM_PARSER_SQMEXTERNCLASS_H_

#include "SqmStructure.h"

#include <QString>

class SqmExternClass : public SqmStructure {
public:
	SqmExternClass(QString const& name);
	virtual ~SqmExternClass() {}

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual bool toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel, FormatType const& format) const override;
	virtual QString const& getName() const override;
	virtual std::shared_ptr<SqmStructure> rename(QString const& newName) const override;
private:
	QString const m_name;
};

#endif
