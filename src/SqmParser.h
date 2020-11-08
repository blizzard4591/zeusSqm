#ifndef ZEUSSQM_PARSER_SQMPARSER_H_
#define ZEUSSQM_PARSER_SQMPARSER_H_

#include <QString>

#include "SqmArray.h"
#include "SqmClass.h"
#include "SqmObjectList.h"
#include "SqmProperty.h"

class SqmParser {
public:
	SqmParser();
	virtual ~SqmParser() {}

	SqmObjectList<SqmStructure> parse(QString const& input) const;
	SqmObjectList<SqmStructure> parse(QByteArray& input) const;

	static QString stripComments(QString const& input);
	static QByteArray stripComments(QByteArray const& input);

	static bool hasBinarizedSqmHeader(QByteArray const& file);
	static quint64 parseCompressedInteger(QByteArray const& data, int& offset);
private:
	// Text-based
	SqmObjectList<SqmStructure> parse(QString const& input, int offset, int length) const;
	int advanceOverLineBreaks(QString const& input, int offset, int length) const;
	int findMatchingClosingCurlyBracket(QString const& input, int length, int posOfOpeningBracket) const;
	int findMatchingQuote(QString const& input, int posOfOpeningQuote, int length) const;
	void failureReport(QString msg, QString const& file, int offset) const;

	// Binarized
	static float parseFloat(QByteArray const& data, int& offset);
	static qint32 parseInt32(QByteArray const& data, int& offset);
	static quint32 parseUInt32(QByteArray const& data, int& offset);
	static quint8 parseUint8(QByteArray const& data, int& offset);
	static QString parseString(QByteArray const& data, int& offset);

	static std::shared_ptr<SqmArray> parseArray(QByteArray const& data, int& offset);
	static SqmArrayContents parseArrayContents(QByteArray const& data, int& offset);
	static SqmArrayContents::ArrayEntry parseArrayElement(QByteArray const& data, int& offset);

	static SqmObjectList<SqmStructure> parseClassBody(QByteArray const& data, int& offset);
	static std::shared_ptr<SqmStructure> parseClassEntry(QByteArray const& data, int& offset);
};

#endif
