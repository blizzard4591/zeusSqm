#ifndef ZEUSSQM_PARSER_SQMARRAYWITHFLAGS_H_
#define ZEUSSQM_PARSER_SQMARRAYWITHFLAGS_H_

#include "SqmStructure.h"
#include "SqmArrayContents.h"

#include <variant>

#include <QString>
#include <QStringList>

class SqmArrayWithFlags : public SqmStructure, public SqmArrayContents {
public:

	SqmArrayWithFlags(QString const& name, quint32 flags, std::vector<ArrayEntry> const& entries);
	SqmArrayWithFlags(QString const& name, quint32 flags, SqmArrayContents const& entries);
	virtual ~SqmArrayWithFlags() {}

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual bool toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel, FormatType const& format) const override;
	virtual QString const& getName() const override;
	virtual std::shared_ptr<SqmStructure> rename(QString const& newName) const override;

	SqmArrayWithFlags setEntry(int index, float value) const;
private:
	QString const m_name;
	quint32 const m_flags;
};

#endif
