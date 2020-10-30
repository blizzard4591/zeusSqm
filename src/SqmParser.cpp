#include "SqmParser.h"

#include <chrono>
#include <iostream>

SqmParser::SqmParser() : 
	m_regExpProperty("\\s*([a-zA-Z]+)\\s*=\\s*((?:[-+]?(?:\\b[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+\\b)(?:[eE][-+]?[0-9]+\\b)?)|\"(?:[^\"]|\"\")*\");"),
	m_regExpArray("\\s*([a-zA-Z]+)\\[\\]\\s*=\\s*{\\s*((?:(?:[-+]?(?:\\b[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+\\b)(?:[eE][-+]?[0-9]+\\b)?)|\"[^\"]*\")(?:\\s*,\\s*(?:(?:[-+]?(?:\\b[0-9]+(?:\\.[0-9]*)?|\\.[0-9]+\\b)(?:[eE][-+]?[0-9]+\\b)?)|\"[^\"]*\"))*)?\\s*};"),
	m_regExpClass("\\s*class ([a-zA-Z0-9]+)\\s*{") {
	//
	m_regExpProperty.optimize();
	m_regExpArray.optimize();
	m_regExpClass.optimize();
}

SqmObjectList SqmParser::parse(QString const& input) const {
	return parse(input, 0, input.length());
}

SqmObjectList SqmParser::parse(QString const& input, int offset, int length) const {
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
					QStringList trimmedParts;
					for (int i = 0; i < parts.size(); ++i) {
						trimmedParts.push_back(parts.at(i).trimmed().toString());
					}

					int const matchLength = posOfClosingBracket + 2 - offset;
					bool const isMultiLine = input.midRef(offset, matchLength).count(cN) > 0;

					objects.push_back(std::make_shared<SqmArray>(name, trimmedParts, isMultiLine));

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
					objects.push_back(std::make_shared<SqmProperty>(name, value));

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
	while (pos < length) {
		if (input.at(pos) == cQ) {
			// Either string ends here, or its an "" escape
			if ((pos + 1) < length && (input.at(pos + 1) == cQ)) {
				pos += 2;
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
