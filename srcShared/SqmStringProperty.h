#ifndef ZEUSSQM_PARSER_SQMSTRINGPROPERTY_H_
#define ZEUSSQM_PARSER_SQMSTRINGPROPERTY_H_

#include "SqmProperty.h"

#include <QString>

class SqmStringProperty : public SqmProperty {
public:
	SqmStringProperty(QString const& name, QString const& value);
	virtual ~SqmStringProperty() {}

	virtual std::shared_ptr<SqmStructure> rename(QString const& newName) const override;

	virtual Type getPropertyType() const override {
		return Type::STRING;
	}

	virtual QString const& getValueAsString() const override;
	virtual qint32 getValueAsInt() const override;
	virtual qint64 getValueAsInt64() const override;
	virtual float getValueAsFloat() const override;

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual bool toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel, FormatType const& format) const override;
private:
	QString const m_value;
};

#endif
