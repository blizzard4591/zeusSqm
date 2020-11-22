#ifndef ZEUSSQM_PARSER_TEXTUALSQMPARSER_H_
#define ZEUSSQM_PARSER_TEXTUALSQMPARSER_H_

#include <QString>

#include "SqmArray.h"
#include "SqmClass.h"
#include "SqmObjectList.h"
#include "SqmProperty.h"

class TextualSqmParser {
public:
	TextualSqmParser(bool beQuiet = false);
	virtual ~TextualSqmParser() {}

	SqmObjectList<SqmStructure> parse(QString const& input) const;
	SqmStructure::FormatType getFormat(QString const& input) const;

	static QString stripComments(QString const& input);
private:
	bool const m_beQuiet;

	std::vector<std::shared_ptr<SqmStructure>> parse(QString const& input, int offset, int length) const;
	QVector<QStringRef> splitArray(QStringRef const& contents, QString const& input, int offset) const;
	std::vector<SqmArrayContents::ArrayEntry> parseArray(QStringRef const& contents, QString const& input, int offset) const;
	int advanceOverLineBreaks(QString const& input, int offset, int length) const;
	int findMatchingClosingCurlyBracket(QString const& input, int length, int posOfOpeningBracket) const;
	int findMatchingQuote(QString const& input, int posOfOpeningQuote, int length) const;
	void failureReport(QString msg, QString const& file, int offset) const;

	static const QChar cR;
	static const QChar cN;
	static const QChar cT;
	static const QChar cS;
	static const QChar ca;
	static const QChar cz;
	static const QChar cA;
	static const QChar cZ;
	static const QChar c0;
	static const QChar c9;
	static const QChar cc;
	static const QChar cl;
	static const QChar cs;
	static const QChar cEqual;
	static const QChar cOpeningBracket;
	static const QChar cClosingBracket;
	static const QChar cSemicolon;
	static const QChar cColon;
	static const QChar cSlash;
	static const QChar cHashTag;
	static const QChar cComma;
	static const QChar cQuote;
	static const QChar cStar;
	static const QChar cOpeningCurlyBracket;
	static const QChar cClosingCurlyBracket;
};

#endif
