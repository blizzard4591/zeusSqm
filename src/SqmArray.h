#ifndef ZEUSSQM_PARSER_SQMARRAY_H_
#define ZEUSSQM_PARSER_SQMARRAY_H_

#include "SqmStructure.h"
#include "SqmArrayContents.h"

#include <variant>

#include <QString>
#include <QStringList>

class SqmArray : public SqmStructure, public SqmArrayContents {
public:

	SqmArray(QString const& name, std::vector<ArrayEntry> const& entries);
	SqmArray(QString const& name, SqmArrayContents const& entries);
	virtual ~SqmArray() {}

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual bool toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel, FormatType const& format) const override;
	virtual QString const& getName() const override;

	SqmArray setEntry(int index, float value) const;
private:
	QString const m_name;
};

#endif
