#ifndef ZEUSSQM_PARSER_SQMSTRUCTURE_H_
#define ZEUSSQM_PARSER_SQMSTRUCTURE_H_

#include <QString>
#include <memory>

class SqmStructure {
public:
	SqmStructure();
	SqmStructure(SqmStructure const& other);
	virtual ~SqmStructure() {}

	virtual QString toSqm(int indentationLevel) const = 0;
	virtual QString const& getName() const = 0;

	int64_t getUniqueId() const;

	bool operator ==(SqmStructure const& other) const;

	enum class Type {
		PROPERTY,
		ARRAY,
		CLASS
	};

	static QString toFloatRepresentation(float f);
	static QString escapeQuotesInString(QString const& s);
	static QString unescapeQuotesInString(QString const& s);
protected:
	int64_t const uniqueId;
	static int64_t getNextUniqueId();
};

#endif
