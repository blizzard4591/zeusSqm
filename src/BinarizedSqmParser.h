#ifndef ZEUSSQM_PARSER_BINARIZEDSQMPARSER_H_
#define ZEUSSQM_PARSER_BINARIZEDSQMPARSER_H_

#include <QByteArray>

#include "SqmArray.h"
#include "SqmClass.h"
#include "SqmObjectList.h"
#include "SqmProperty.h"

class BinarizedSqmParser {
public:
	BinarizedSqmParser();
	virtual ~BinarizedSqmParser() {}

	SqmObjectList<SqmStructure> parse(QByteArray& input) const;

	static QByteArray stripComments(QByteArray const& input);

	static bool hasBinarizedSqmHeader(QByteArray const& file);
	static quint64 parseCompressedInteger(QByteArray const& data, int& offset);
private:
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
