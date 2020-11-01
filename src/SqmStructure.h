#ifndef ZEUSSQM_PARSER_SQMSTRUCTURE_H_
#define ZEUSSQM_PARSER_SQMSTRUCTURE_H_

#include <QString>

class SqmStructure {
public:
	virtual ~SqmStructure() {}

	virtual QString toSqm(int indentationLevel) const = 0;
	virtual QString const& getName() const = 0;

	enum class Type {
		PROPERTY,
		ARRAY,
		CLASS
	};
};

#endif
