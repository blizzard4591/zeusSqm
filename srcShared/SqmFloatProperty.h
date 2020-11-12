#ifndef ZEUSSQM_PARSER_SQMFLOATPROPERTY_H_
#define ZEUSSQM_PARSER_SQMFLOATPROPERTY_H_

#include "SqmProperty.h"

#include <QString>

class SqmFloatProperty : public SqmProperty {
public:
	SqmFloatProperty(QString const& name, float value);
	SqmFloatProperty(QString const& name, QString const& value);
	SqmFloatProperty(QString const& name, float value, QString const& svalue);
	virtual ~SqmFloatProperty() {}

	virtual std::shared_ptr<SqmStructure> rename(QString const& newName) const override;

	virtual Type getPropertyType() const override {
		return Type::FLOAT;
	}

	virtual QString const& getValueAsString() const override;
	virtual qint32 getValueAsInt() const override;
	virtual float getValueAsFloat() const override;

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual bool toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel, FormatType const& format) const override;
private:
	float const m_value;
	QString const m_valueString;
};

#endif
