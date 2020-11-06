#include "SqmParser.h"

#include <chrono>
#include <iostream>

#include <QDataStream>
#include <QTextStream>

#include "exceptions/FormatErrorException.h"

SqmParser::SqmParser() {
	//
}

SqmObjectList<SqmStructure> SqmParser::parse(QString const& input) const {
	auto t1 = std::chrono::high_resolution_clock::now();
	SqmObjectList<SqmStructure> root = parse(input, 0, input.length());
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "Parsing SQM took " << duration << "ms." << std::endl;
	return root;
}

QByteArray SqmParser::stripComments(QByteArray const& input) {
	if (hasBinarizedSqmHeader(input)) {
		return input;
	}

	QTextStream stream(input);
	QString const missionFileData = stream.readAll();

	QString const strippedMission = stripComments(missionFileData);

	QByteArray outputData;
	QTextStream outputStream(&outputData);
	outputStream.setCodec("UTF-8");
	outputStream << strippedMission;
	outputStream.flush();

	return outputData;
}

QString SqmParser::stripComments(QString const& input) {
	QString const newLine = (input.count(QStringLiteral("\r\n")) > 0) ? QStringLiteral("\r\n") : QStringLiteral("\n");
	QString result;
	result.reserve(input.size());

	int offset = 0;
	int nextNewline = input.indexOf(newLine, offset);
	while (nextNewline > -1) {
		int pos = offset;
		while (input.at(pos) == ' ' || input.at(pos) == '\t') {
			++pos;
		}
		if (input.at(pos) == '/' && input.at(pos + 1) == '/') {
			// this is a comment line, strip.
		} else if (input.at(pos) == '#') {
			// this is a pragma line, strip.
		} else {
			result.append(input.mid(offset, nextNewline - offset + newLine.size()));
		}
		offset = nextNewline + newLine.size();
		nextNewline = input.indexOf(newLine, offset);
	}
	
	if (offset < input.size()) {
		result.append(input.mid(offset));
	}
	return result;
}

float SqmParser::parseFloat(QByteArray const& data, int& offset) {
	float result = *reinterpret_cast<float const*>(data.constData() + offset);
	offset += 4;
	return result;
}

qint32 SqmParser::parseInt32(QByteArray const& data, int& offset) {
	qint32 result = *reinterpret_cast<qint32 const*>(data.constData() + offset);
	offset += 4;
	return result;
}

quint32 SqmParser::parseUInt32(QByteArray const& data, int& offset) {
	quint32 result = *reinterpret_cast<quint32 const*>(data.constData() + offset);
	offset += 4;
	return result;
}

quint8 SqmParser::parseUint8(QByteArray const& data, int& offset) {
	quint8 result = *reinterpret_cast<quint8 const*>(data.constData() + offset);
	offset += 1;
	return result;
}

quint64 SqmParser::parseCompressedInteger(QByteArray const& data, int& offset) {
	quint64 result = 0;
	quint8 v = parseUint8(data, offset);
	result += v;
	while (v & 0x80) {
		v = parseUint8(data, offset);
		result += (v - 1uLL) * 0x80uLL;
	}

	return result;
}

QString SqmParser::parseString(QByteArray const& data, int& offset) {
	QByteArray stringBytes;
	char c;
	c = data.at(offset++);
	while (c != '\0') {
		stringBytes.append(static_cast<char>(c));
		c = data.at(offset++);
	}
	return QString(stringBytes);
}

SqmArrayContents::ArrayEntry SqmParser::parseArrayElement(QByteArray const& data, int& offset) {
	quint8 const type = parseUint8(data, offset);
	switch (type) {
	case 0:
	{
		// String
		return SqmArrayContents::ArrayEntry(parseString(data, offset));
	}
	case 1:
	{
		// Float
		return SqmArrayContents::ArrayEntry(parseFloat(data, offset));
	}
	case 2:
	{
		// Long
		return SqmArrayContents::ArrayEntry(parseInt32(data, offset));
	}
	case 3:
	{
		return SqmArrayContents::ArrayEntry(parseArrayContents(data, offset));
	}
	default:
		std::cerr << "Unhandled Type '" << (int)type << "' in Array!" << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "Unhandled Type '" << (int)type << "' in Array!";
	}
}

SqmArrayContents SqmParser::parseArrayContents(QByteArray const& data, int& offset) {
	quint64 const entryCount = parseCompressedInteger(data, offset);
	std::vector<SqmArrayContents::ArrayEntry> entries;
	for (quint64 i = 0; i < entryCount; ++i) {
		entries.push_back(parseArrayElement(data, offset));
	}
	return SqmArrayContents(entries);
}

std::shared_ptr<SqmArray> SqmParser::parseArray(QByteArray const& data, int& offset) {
	QString const name = parseString(data, offset);
	return std::make_shared<SqmArray>(name, parseArrayContents(data, offset));
}

std::shared_ptr<SqmStructure> SqmParser::parseClassEntry(QByteArray const& data, int& offset) {
	quint8 const id = parseUint8(data, offset);
	switch (id) {
	case 0:
	{
		QString const className = parseString(data, offset);
		int classOffset = parseUInt32(data, offset);
		return std::make_shared<SqmClass>(className, parseClassBody(data, classOffset));
	}
	case 1:
	{
		// Property
		quint8 const subId = parseUint8(data, offset);
		QString propertyName = parseString(data, offset);

		switch (subId) {
		case 0:
		{
			// String
			QString value = parseString(data, offset);
			return std::make_shared<SqmStringProperty>(propertyName, value);
		}
		case 1:
		{
			// Float
			float const value = parseFloat(data, offset);
			return std::make_shared<SqmFloatProperty>(propertyName, value);
		}
		case 2:
		{
			// Long
			qint32 const value = parseInt32(data, offset);
			return std::make_shared<SqmIntProperty>(propertyName, value);
		}
		default:
			std::cerr << "Unhandled SubId '" << (int)subId << "' in ClassEntry!" << std::endl;
			throw zeusops::exceptions::FormatErrorException() << "Unhandled SubId '" << (int)subId << "' in ClassEntry!";
		}
		break;
	}
	case 2:
	{
		// Array
		return parseArray(data, offset);
	}
	default:
	{
		std::cerr << "Unhandled ClassEntry ID '" << (int)id << "' in ClassEntry!" << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "Unhandled ClassEntry ID '" << (int)id << "' in ClassEntry!";
	}
	}
}

SqmObjectList<SqmStructure> SqmParser::parseClassBody(QByteArray const& data, int& offset) {
	QString inheritedClassname = parseString(data, offset);

	quint64 const entryCount = parseCompressedInteger(data, offset);
	std::vector<std::shared_ptr<SqmStructure>> items;

	for (quint64 i = 0; i < entryCount; ++i) {
		items.push_back(parseClassEntry(data, offset));
	}

	if ((!inheritedClassname.isNull()) && (!inheritedClassname.isEmpty())) {
		std::cout << "Parsed class with inherited class '" << inheritedClassname.toStdString() << "'." << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "Parsed class with inherited class '" << inheritedClassname.toStdString() << "'.";
	}

	return SqmObjectList<SqmStructure>(items);
}

bool SqmParser::hasBinarizedSqmHeader(QByteArray const& file) {
	if (file.size() < 4) return false;
	return ((file.at(0) == '\0') && (file.at(1) == 'r') && (file.at(2) == 'a') && (file.at(3) == 'P'));
}

SqmObjectList<SqmStructure> SqmParser::parse(QByteArray& input) const {
	auto t1 = std::chrono::high_resolution_clock::now();

	if (!hasBinarizedSqmHeader(input)) {
		std::cerr << "Header of SQM file corrupted, either this is not a binarized SQM or its broken." << std::endl;
		throw zeusops::exceptions::FormatErrorException() << "Header of SQM file corrupted, either this is not a binarized SQM or its broken.";
	}

	// Skip first 4 bytes (header)
	int offset = 4;

	quint32 const alwaysZero = parseUInt32(input, offset);
	if (alwaysZero != 0) throw;

	quint32 const alwaysEight = parseUInt32(input, offset);
	if (alwaysEight != 8) throw;

	quint32 const offsetToEnums = parseUInt32(input, offset);

	SqmObjectList<SqmStructure> result = parseClassBody(input, offset);

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	std::cout << "Parsing binarized SQM took " << duration << "ms." << std::endl;

	return result;
}

SqmObjectList<SqmStructure> SqmParser::parse(QString const& input, int offset, int length) const {
	std::vector<std::shared_ptr<SqmStructure>> objects;

	static const QChar cR('\r');
	static const QChar cN('\n');
	static const QChar cT('\t');
	static const QChar cS(' ');
	static const QChar ca('a');
	static const QChar cz('z');
	static const QChar cA('A');
	static const QChar cZ('Z');
	static const QChar c0('0');
	static const QChar c9('9');
	static const QChar cc('c');
	static const QChar cl('l');
	static const QChar cs('s');
	static const QChar cEqual('=');
	static const QChar cOpeningBracket('[');
	static const QChar cClosingBracket(']');
	static const QChar cOpeningCurlyBracket('{');
	static const QChar cClosingCurlyBracket('}');
	static const QChar cSemicolon(';');
	static const QChar cComma(',');
	static const QChar cQuote('"');

	while (offset < length) {
		QChar const c = input.at(offset);
		if (c == cR || c == cN || c == cT || c == cS) {
			++offset;
		} else if (((c >= cA) && (c <= cZ)) || ((c >= ca) && (c <= cz)) || ((c >= c0) || (c <= c9))) {
			// Could be either a class or a propery or array
			if ((c == cc) && ((offset + 5) < length) && (input.at(offset + 1) == cl) && (input.at(offset + 2) == ca) && (input.at(offset + 3) == cs) && (input.at(offset + 4) == cs) && (input.at(offset + 5) == cS)) {
				// Class
				int const posOfOpeningCurlyBracket = input.indexOf(cOpeningCurlyBracket, offset + 6);
				if (posOfOpeningCurlyBracket == -1) {
					failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
				}

				QString const name = input.mid(offset + 6, posOfOpeningCurlyBracket - offset - 6).trimmed();
				
				int const posOfClosingCurlyBracket = findMatchingClosingCurlyBracket(input, length, posOfOpeningCurlyBracket);
				if (posOfClosingCurlyBracket == -1) {
					failureReport("Failed to match closing curly bracket in line LINE!", input, offset);
				}

				int innerPos = posOfOpeningCurlyBracket + 1;
				innerPos = advanceOverLineBreaks(input, innerPos, length);

				objects.push_back(std::make_shared<SqmClass>(name, parse(input, innerPos, posOfClosingCurlyBracket - 1)));

				offset = posOfClosingCurlyBracket + 2;
				offset = advanceOverLineBreaks(input, offset, length);
			} else {
				int const equalPos = input.indexOf(cEqual, offset);
				if (equalPos == -1) {
					failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
				}

				if (input.at(equalPos - 1) == cClosingBracket) {
					// Array
					QString const name = input.mid(offset, equalPos - offset - 2);
					int posOfOpeningBracket = equalPos + 1;
					while ((posOfOpeningBracket < length) && (input.at(posOfOpeningBracket) != cOpeningCurlyBracket)) {
						++posOfOpeningBracket;
					}

					if (posOfOpeningBracket >= length) {
						failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
					}

					int const posOfClosingBracket = findMatchingClosingCurlyBracket(input, length, posOfOpeningBracket);
					if (posOfClosingBracket == -1) {
						failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
					} else if ((posOfClosingBracket + 1) >= length || (input.at(posOfClosingBracket + 1) != cSemicolon)) {
						failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
					}

					QStringRef const arrayContent = input.midRef(posOfOpeningBracket + 1, posOfClosingBracket - posOfOpeningBracket - 1);
					QVector<QStringRef> parts = arrayContent.split(cComma);
					std::vector<SqmArrayContents::ArrayEntry> arrayEntries;
					for (int i = 0; i < parts.size(); ++i) {
						QString const trimmed = parts.at(i).trimmed().toString();
						SqmArrayContents::ArrayEntry entry;
						if (trimmed.at(0) == cQuote) {
							entry = SqmArrayContents::ArrayEntry(trimmed.mid(1, trimmed.size() - 2));
						} else if ((trimmed.indexOf('.') > -1) || (trimmed.compare(QStringLiteral("-0")) == 0)) {
							entry = SqmArrayContents::ArrayEntry(trimmed.toFloat(), trimmed);
						} else {
							entry = SqmArrayContents::ArrayEntry(trimmed.toInt());
						}
						arrayEntries.push_back(entry);
					}
					

					int const matchLength = posOfClosingBracket + 2 - offset;
					bool const isMultiLine = input.midRef(offset, matchLength).count(cN) > 0;

					objects.push_back(std::make_shared<SqmArray>(name, arrayEntries));

					offset += matchLength;
					offset = advanceOverLineBreaks(input, offset, length);
				} else {
					// Property
					QString const name = input.mid(offset, equalPos - offset);
					int posOfClosingSemicolon = -1;
					if (((equalPos + 1) < length) && (input.at(equalPos + 1) == cQuote)) {
						int const posOfOpeningQuote = equalPos + 1;
						int const posOfClosingQuote = findMatchingQuote(input, posOfOpeningQuote, length);
						if (posOfClosingQuote == -1) {
							failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
						} else if ((posOfClosingQuote + 1) >= length || (input.at(posOfClosingQuote + 1) != cSemicolon)) {
							failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
						}
						posOfClosingSemicolon = posOfClosingQuote + 1;
					} else {
						posOfClosingSemicolon = input.indexOf(cSemicolon, equalPos + 1);
						if (posOfClosingSemicolon == -1) {
							failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
						}
					}
					QString const value = input.mid(equalPos + 1, posOfClosingSemicolon - equalPos - 1);
					if (value.at(0) == cQuote) {
						objects.push_back(std::make_shared<SqmStringProperty>(name, value.mid(1, value.size() - 2).replace("\"\"", "\"")));
					} else if (value.indexOf('.') > -1) {
						objects.push_back(std::make_shared<SqmFloatProperty>(name, value));
					} else {
						bool ok = false;
						objects.push_back(std::make_shared<SqmIntProperty>(name, value.toInt(&ok)));
						if (!ok) failureReport("Could not parse integer property (line LINE, offset OFFSET)", input, offset);
					}

					offset = posOfClosingSemicolon + 1;
					offset = advanceOverLineBreaks(input, offset, length);
				}
			}
		} else {
			failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
		}
	}

	return objects;
}

int SqmParser::advanceOverLineBreaks(QString const& input, int offset, int length) const {
	while ((offset < length) && ((input.at(offset) == '\r') || (input.at(offset) == '\n'))) {
		++offset;
	}
	return offset;
}

int SqmParser::findMatchingQuote(QString const& input, int posOfOpeningQuote, int length) const {
	int pos = posOfOpeningQuote + 1;
	static const QChar cQ('"');
	static const QChar cS(' ');
	static const QChar cB('\\');
	static const QChar cn('n');
	while (pos < length) {
		if (input.at(pos) == cQ) {
			// Either string ends here, or its an "" escape
			if ((pos + 1) < length && (input.at(pos + 1) == cQ)) {
				pos += 2;
				continue;
			} else if ((pos + 5) < length && (input.at(pos + 1) == cS) && (input.at(pos + 2) == cB) && (input.at(pos + 3) == cn) && (input.at(pos + 4) == cS) && (input.at(pos + 5) == cQ)) {
				pos += 6;
				continue;
			} else {
				return pos;
			}
		}
		++pos;
	}
	return -1;
}

int SqmParser::findMatchingClosingCurlyBracket(QString const& input, int length, int posOfOpeningBracket) const {
	int pos = posOfOpeningBracket + 1;
	int depth = 0;
	while (pos < length) {
		if (input.at(pos) == '{') {
			++depth;
		} else if (input.at(pos) == '}') {
			if (depth == 0) {
				return pos;
			}
			--depth;
		}
		++pos;
	}
	return -1;
}

void SqmParser::failureReport(QString msg, QString const& file, int offset) const {
	int const line = file.midRef(0, offset).count('\n');
	int const nextNewline = file.indexOf('\n', offset);
	int const length = nextNewline - offset;
	QString const nextInput = file.midRef(offset, length).trimmed().toString();

	msg = msg.replace(QStringLiteral("NEXT_INPUT"), nextInput);
	msg = msg.replace(QStringLiteral("LINE"), QString::number(line));
	msg = msg.replace(QStringLiteral("OFFSET"), QString::number(offset));

	std::cerr << "Failed to parse SQM: " << msg.toStdString() << std::endl;
	exit(-1);
}
