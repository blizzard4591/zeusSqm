#ifndef ZEUSSQM_PARSER_SQMPROPERTY_H_
#define ZEUSSQM_PARSER_SQMPROPERTY_H_

#include "SqmStructure.h"

#include <QString>

class SqmProperty : public SqmStructure {
public:
	SqmProperty(QString const& name);
	virtual ~SqmProperty() {}

	enum class Type {
		STRING, FLOAT, INT
	};
	virtual Type getPropertyType() const = 0;

	virtual QString const& getValueAsString() const = 0;
	virtual qint32 getValueAsInt() const = 0;
	virtual float getValueAsFloat() const = 0;

	virtual QString const& getName() const override;

	static std::shared_ptr<SqmProperty> newStringProperty(QString const& name, QString const& value);
	static std::shared_ptr<SqmProperty> newIntegerProperty(QString const& name, qint32 value);
protected:
	QString const m_name;
};

#endif
