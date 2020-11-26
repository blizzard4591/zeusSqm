#ifndef ZEUSSQM_PARSER_SQMPROPERTY_H_
#define ZEUSSQM_PARSER_SQMPROPERTY_H_

#include "SqmStructure.h"

#include <QString>

class SqmProperty : public SqmStructure {
public:
	SqmProperty(QString const& name);
	virtual ~SqmProperty() {}

	enum class Type {
		STRING, FLOAT, INT, INT64
	};
	virtual Type getPropertyType() const = 0;

	virtual QString const& getValueAsString() const = 0;
	virtual qint32 getValueAsInt() const = 0;
	virtual qint64 getValueAsInt64() const = 0;
	virtual float getValueAsFloat() const = 0;

	virtual QString const& getName() const override;

	static std::shared_ptr<SqmProperty> newStringProperty(QString const& name, QString const& value);
	static std::shared_ptr<SqmProperty> newIntegerProperty(QString const& name, qint32 value);
	static std::shared_ptr<SqmProperty> newInteger64Property(QString const& name, qint64 value);
protected:
	QString const m_name;
};

#endif
