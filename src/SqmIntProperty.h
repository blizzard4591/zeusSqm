#ifndef ZEUSSQM_PARSER_SQMINTPROPERTY_H_
#define ZEUSSQM_PARSER_SQMINTPROPERTY_H_

#include "SqmProperty.h"

#include <QString>

class SqmIntProperty : public SqmProperty {
public:
	SqmIntProperty(QString const& name, qint32 value);
	virtual ~SqmIntProperty() {}

	virtual Type getPropertyType() const override {
		return Type::INT;
	}

	virtual QString const& getValueAsString() const override;
	virtual qint32 getValueAsInt() const override;
	virtual float getValueAsFloat() const override;

	std::shared_ptr<SqmIntProperty> increment(int incrementBy, int* oldValue = nullptr) const;

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual void toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int> const& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel) const override;
private:
	qint32 const m_value;
};

#endif
