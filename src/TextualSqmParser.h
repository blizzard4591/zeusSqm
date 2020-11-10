#ifndef ZEUSSQM_PARSER_TEXTUALSQMPARSER_H_
#define ZEUSSQM_PARSER_TEXTUALSQMPARSER_H_

#include <QString>

#include "SqmArray.h"
#include "SqmClass.h"
#include "SqmObjectList.h"
#include "SqmProperty.h"

class TextualSqmParser {
public:
	TextualSqmParser();
	virtual ~TextualSqmParser() {}

	SqmObjectList<SqmStructure> parse(QString const& input) const;
	SqmStructure::FormatType getFormat(QString const& input) const;

	static QString stripComments(QString const& input);
private:
	SqmObjectList<SqmStructure> parse(QString const& input, int offset, int length) const;
	int advanceOverLineBreaks(QString const& input, int offset, int length) const;
	int findMatchingClosingCurlyBracket(QString const& input, int length, int posOfOpeningBracket) const;
	int findMatchingQuote(QString const& input, int posOfOpeningQuote, int length) const;
	void failureReport(QString msg, QString const& file, int offset) const;
};

#endif
