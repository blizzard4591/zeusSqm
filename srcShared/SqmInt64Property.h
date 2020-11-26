#ifndef ZEUSSQM_PARSER_SQMINT64PROPERTY_H_
#define ZEUSSQM_PARSER_SQMINT64PROPERTY_H_

#include "SqmProperty.h"

#include <QString>

class SqmInt64Property : public SqmProperty {
public:
	SqmInt64Property(QString const& name, qint64 value);
	virtual ~SqmInt64Property() {}

	virtual std::shared_ptr<SqmStructure> rename(QString const& newName) const override;

	virtual Type getPropertyType() const override {
		return Type::INT;
	}

	virtual QString const& getValueAsString() const override;
	virtual qint32 getValueAsInt() const override;
	virtual qint64 getValueAsInt64() const override;
	virtual float getValueAsFloat() const override;

	std::shared_ptr<SqmInt64Property> increment(qint64 incrementBy, qint64* oldValue = nullptr) const;
	std::shared_ptr<SqmInt64Property> decrement(qint64 decrementBy, qint64* oldValue = nullptr) const;

	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual bool toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel, FormatType const& format) const override;
private:
	qint64 const m_value;
};

#endif
