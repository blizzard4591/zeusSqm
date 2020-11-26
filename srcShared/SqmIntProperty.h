#ifndef ZEUSSQM_PARSER_SQMINTPROPERTY_H_
#define ZEUSSQM_PARSER_SQMINTPROPERTY_H_

#include "SqmProperty.h"

#include <QString>

class SqmIntProperty : public SqmProperty {
public:
	SqmIntProperty(QString const& name, qint32 value);
	virtual ~SqmIntProperty() {}

	virtual std::shared_ptr<SqmStructure> rename(QString const& newName) const override;

	virtual Type getPropertyType() const override {
		return Type::INT;
	}

	virtual QString const& getValueAsString() const override;
	virtual qint32 getValueAsInt() const override;
	virtual qint64 getValueAsInt64() const override;
	virtual float getValueAsFloat() const override;

	std::shared_ptr<SqmIntProperty> increment(int incrementBy, int* oldValue = nullptr) const;
	std::shared_ptr<SqmIntProperty> decrement(int decrementBy, int* oldValue = nullptr) const;

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual bool toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel, FormatType const& format) const override;
private:
	qint32 const m_value;
};

#endif
