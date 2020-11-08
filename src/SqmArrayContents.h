#ifndef ZEUSSQM_PARSER_SQMARRAYCONTENTS_H_
#define ZEUSSQM_PARSER_SQMARRAYCONTENTS_H_

#include "SqmStructure.h"

#include <memory>
#include <variant>

#include <QByteArray>
#include <QString>
#include <QStringList>

class SqmArrayContents;

class SqmArrayContents {
public:
	enum class ArrayEntryType {
		STRING, FLOAT, INT, RECURSIVE_ARRAY
	};

	struct ArrayEntry {
		ArrayEntryType type;
		QString stringValue;
		std::variant<QString, std::shared_ptr<SqmArrayContents>, float, qint32> content;

		ArrayEntry();
		ArrayEntry(float value);
		ArrayEntry(float value, QString const& valueString);
		ArrayEntry(qint32 value);
		ArrayEntry(QString const& value);
		ArrayEntry(SqmArrayContents const& value);
	};


	SqmArrayContents(std::vector<ArrayEntry> const& entries);
	virtual ~SqmArrayContents() {}

	virtual void toSqmStageOne(QByteArray& output) const;

	std::vector<ArrayEntry> const& getValues() const;

	float getEntryAsFloat(int index) const;

	virtual QString toSqm(int indentationLevel, bool& isMultiline) const;
private:
	std::vector<ArrayEntry> m_values;

	bool shoudBeMultiLine() const;
};

#endif
