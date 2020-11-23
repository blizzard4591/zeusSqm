#include "TextualSqmParser.h"

#include <chrono>
#include <iostream>

#include <QDataStream>
#include <QTextStream>

#include "SqmArrayWithFlags.h"
#include "exceptions/FormatErrorException.h"


const QChar TextualSqmParser::cR('\r');
const QChar TextualSqmParser::cN('\n');
const QChar TextualSqmParser::cT('\t');
const QChar TextualSqmParser::cS(' ');
const QChar TextualSqmParser::ca('a');
const QChar TextualSqmParser::cz('z');
const QChar TextualSqmParser::cA('A');
const QChar TextualSqmParser::cZ('Z');
const QChar TextualSqmParser::c0('0');
const QChar TextualSqmParser::c9('9');
const QChar TextualSqmParser::cc('c');
const QChar TextualSqmParser::cl('l');
const QChar TextualSqmParser::cs('s');
const QChar TextualSqmParser::cEqual('=');
const QChar TextualSqmParser::cPlus('+');
const QChar TextualSqmParser::cOpeningBracket('[');
const QChar TextualSqmParser::cClosingBracket(']');
const QChar TextualSqmParser::cOpeningCurlyBracket('{');
const QChar TextualSqmParser::cClosingCurlyBracket('}');
const QChar TextualSqmParser::cSemicolon(';');
const QChar TextualSqmParser::cColon(':');
const QChar TextualSqmParser::cComma(',');
const QChar TextualSqmParser::cQuote('"');
const QChar TextualSqmParser::cStar('*');
const QChar TextualSqmParser::cSlash('/');
const QChar TextualSqmParser::cHashTag('#');


TextualSqmParser::TextualSqmParser(bool beQuiet) : m_beQuiet(beQuiet) {
	//
}

SqmObjectList<SqmStructure> TextualSqmParser::parse(QString const& input) const {
	auto t1 = std::chrono::high_resolution_clock::now();
	SqmObjectList<SqmStructure> root = SqmObjectList<SqmStructure>(QString(), parse(input, 0, input.length()));
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
	if (!m_beQuiet) {
		std::cout << "Parsing SQM took " << duration << "ms." << std::endl;
	}
	return root;
}

SqmStructure::FormatType TextualSqmParser::getFormat(QString const& input) const {
	SqmStructure::FormatType format = SqmStructure::FormatType::NOSPACE;
	if (input.indexOf(QStringLiteral("version=")) == -1) {
		format = SqmStructure::FormatType::SINGLESPACED;
	}
	return format;
}

QString TextualSqmParser::stripComments(QString const& input) {
	QString const newLine = (input.count(QStringLiteral("\r\n")) > 0) ? QStringLiteral("\r\n") : QStringLiteral("\n");
	QString result;
	result.reserve(input.size());

	int offset = 0;
	int nextNewline = input.indexOf(newLine, offset);
	while (nextNewline > -1) {
		int pos = offset;
		while (input.at(pos) == cS || input.at(pos) == cT) {
			++pos;
		}
		if (input.at(pos) == cSlash && input.at(pos + 1) == cSlash) {
			// this is a comment line, strip.
		} else if (input.at(pos) == cHashTag) {
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

QVector<QStringRef> TextualSqmParser::splitArray(QStringRef const& contents, QString const& input, int offset) const {
	QString const contentsString = contents.toString();
	QVector<QStringRef> result;
	int pos = 0;
	int lastStart = 0;
	while (pos < contents.length()) {
		if (contents.at(pos) == cComma) {
			result.push_back(contents.mid(lastStart, pos - lastStart));
			lastStart = pos + 1;
		} else if (contents.at(pos) == cOpeningCurlyBracket) {
			int const posOfClosingCurlyBracket = findMatchingClosingCurlyBracket(contentsString, contentsString.length(), pos);
			if (posOfClosingCurlyBracket == -1) {
				failureReport("Failed to find matching closing bracket while splitting array. Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
			}
			pos = posOfClosingCurlyBracket;
		}
		++pos;
	}
	result.push_back(contents.mid(lastStart, pos - lastStart));

	return result;
}

std::vector<SqmArrayContents::ArrayEntry> TextualSqmParser::parseArray(QStringRef const& contents, QString const& input, int offset) const {
	std::vector<SqmArrayContents::ArrayEntry> arrayEntries;
	if (contents.trimmed().length() > 0) {
		QVector<QStringRef> parts = splitArray(contents, input, offset);
		for (int i = 0; i < parts.size(); ++i) {
			QString const trimmed = parts.at(i).trimmed().toString();
			if (trimmed.length() == 0) {
				continue;
			}
			SqmArrayContents::ArrayEntry entry;
			if (trimmed.at(0) == cQuote) {
				entry = SqmArrayContents::ArrayEntry(SqmStructure::unescapeQuotesInString(trimmed.mid(1, trimmed.size() - 2)));
			} else if (trimmed.at(0) == cOpeningCurlyBracket) {
				// This is a sub-array
				int const subPosOfOpeningBracket = 0;
				int const subPosOfClosingBracket = findMatchingClosingCurlyBracket(trimmed, trimmed.length(), subPosOfOpeningBracket);
				if (subPosOfClosingBracket == -1) {
					failureReport("Failed to find matching closing bracket while parsing sub-array. Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
				}

				QStringRef const arrayContent = trimmed.midRef(subPosOfOpeningBracket + 1, subPosOfClosingBracket - subPosOfOpeningBracket - 1);
				entry = SqmArrayContents::ArrayEntry(parseArray(arrayContent, input, offset));
			} else if ((trimmed.indexOf('.') > -1) || (trimmed.compare(QStringLiteral("-0")) == 0)) {
				entry = SqmArrayContents::ArrayEntry(trimmed.toFloat(), trimmed);
			} else {
				entry = SqmArrayContents::ArrayEntry(trimmed.toInt());
			}
			arrayEntries.push_back(entry);
		}
	}
	return arrayEntries;
}

std::vector<std::shared_ptr<SqmStructure>> TextualSqmParser::parse(QString const& input, int offset, int length) const {
	std::vector<std::shared_ptr<SqmStructure>> objects;

	while (offset < length) {
		QChar const c = input.at(offset);
		if (c == cR || c == cN || c == cT || c == cS) {
			++offset;
		} else if (((c >= cA) && (c <= cZ)) || ((c >= ca) && (c <= cz)) || ((c >= c0) || (c <= c9))) {
			if ((c == cSlash) && ((offset + 1) < length) && (input.at(offset + 1) == cSlash)) {
				// Comment, skip!
				int posN = input.indexOf(cN, offset);
				if (posN != -1) {
					offset = posN;
					offset = advanceOverLineBreaks(input, offset, length);
				} else {
					offset = length;
				}
			} else if ((c == cSlash) && ((offset + 1) < length) && (input.at(offset + 1) == cStar)) {
				// Comment, skip!
				int posEnd = input.indexOf(QStringLiteral("*/"), offset);
				if (posEnd == -1) {
					failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
				}
				offset = posEnd + 2;
				offset = advanceOverLineBreaks(input, offset, length);
			} else if ((c == cHashTag) && ((offset + 6) < length) && (input.midRef(offset, 7).compare(QStringLiteral("#define")) == 0)) {
				// Pragma define, skip.
				int posN = input.indexOf(cN, offset);
				if (posN != -1) {
					offset = posN;
					offset = advanceOverLineBreaks(input, offset, length);
				} else {
					offset = length;
				}
			} else if ((c == cHashTag) && ((offset + 7) < length) && (input.midRef(offset, 8).compare(QStringLiteral("#include")) == 0)) {
				// Pragma include, skip.
				int posN = input.indexOf(cN, offset);
				if (posN != -1) {
					offset = posN;
					offset = advanceOverLineBreaks(input, offset, length);
				} else {
					offset = length;
				}
			} else if (((offset + 4) < length) && input.midRef(offset, 4).compare(QStringLiteral("enum")) == 0) {
				// Enum define, skip.
				int const posOfOpeningCurlyBracket = input.indexOf(cOpeningCurlyBracket, offset + 4);
				if (posOfOpeningCurlyBracket == -1) {
					failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
				}

				int const posOfClosingCurlyBracket = findMatchingClosingCurlyBracket(input, length, posOfOpeningCurlyBracket);
				if (posOfClosingCurlyBracket == -1) {
					failureReport("Failed to match closing curly bracket in line LINE!", input, offset);
				}

				offset = posOfClosingCurlyBracket + 2;
				offset = advanceOverLineBreaks(input, offset, length);
			}

			// Could be either a class or a propery or array
			else if ((c == cc) && ((offset + 5) < length) && (input.at(offset + 1) == cl) && (input.at(offset + 2) == ca) && (input.at(offset + 3) == cs) && (input.at(offset + 4) == cs) && (input.at(offset + 5) == cS)) {
				// Class
				int const posOfOpeningCurlyBracket = input.indexOf(cOpeningCurlyBracket, offset + 6);
				// Check for empty class def
				int const posOfFirstSemicolon = input.indexOf(cSemicolon, offset + 6);
				if (posOfFirstSemicolon == -1) {
					failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
				}

				int contentStart = posOfOpeningCurlyBracket;
				if (posOfOpeningCurlyBracket == -1 || (posOfFirstSemicolon < posOfOpeningCurlyBracket)) {
					// This is a class def: "class Bla;"
					contentStart = posOfFirstSemicolon;
				}

				QString inheritedClassName;
				QString className;
				int const posOfColon = input.indexOf(cColon, offset + 6);
				if ((posOfColon != -1) && (posOfColon < contentStart)) {
					// This is a class with inheritance
					className = input.mid(offset + 6, posOfColon - offset - 6).trimmed();
					inheritedClassName = input.mid(posOfColon + 1, contentStart - posOfColon - 1).trimmed();
				} else {
					className = input.mid(offset + 6, contentStart - offset - 6).trimmed();
				}
				
				if (posOfOpeningCurlyBracket == -1 || (posOfFirstSemicolon < posOfOpeningCurlyBracket)) {
					// This is a class def: "class Bla;"
					objects.push_back(std::make_shared<SqmClass>(className, SqmObjectList<SqmStructure>(inheritedClassName, {})));
					offset = posOfFirstSemicolon + 1;
				} else {
					int const posOfClosingCurlyBracket = findMatchingClosingCurlyBracket(input, length, posOfOpeningCurlyBracket);
					if (posOfClosingCurlyBracket == -1) {
						failureReport("Failed to match closing curly bracket in line LINE!", input, offset);
					}

					int innerPos = posOfOpeningCurlyBracket + 1;
					innerPos = advanceOverLineBreaks(input, innerPos, length);

					objects.push_back(std::make_shared<SqmClass>(className, SqmObjectList<SqmStructure>(inheritedClassName, parse(input, innerPos, posOfClosingCurlyBracket - 1))));
					offset = posOfClosingCurlyBracket + 2;
				}				
				offset = advanceOverLineBreaks(input, offset, length);
			} else {
				int const equalPos = input.indexOf(cEqual, offset);
				if (equalPos == -1) {
					failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
				}

				int equalToNonSpaceOffset = 1;
				while (input.at(equalPos - equalToNonSpaceOffset) == cS || input.at(equalPos - equalToNonSpaceOffset) == cT || input.at(equalPos - equalToNonSpaceOffset) == cPlus) {
					++equalToNonSpaceOffset;
				}

				if (input.at(equalPos - equalToNonSpaceOffset) == cClosingBracket) {
					// Array
					if (input.at(equalPos - equalToNonSpaceOffset - 1) != cOpeningBracket) {
						failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
					}
					bool const isPlusArray = input.at(equalPos - 1) == cPlus;

					QString const name = input.mid(offset, (equalPos - equalToNonSpaceOffset - 1) - offset);
					equalToNonSpaceOffset = 1;
					while ((equalPos + equalToNonSpaceOffset < length) && ((input.at(equalPos + equalToNonSpaceOffset) == cS) || (input.at(equalPos + equalToNonSpaceOffset) == cT) || (input.at(equalPos + equalToNonSpaceOffset) == cR) || (input.at(equalPos + equalToNonSpaceOffset) == cN))) {
						++equalToNonSpaceOffset;
					}

					int const posOfOpeningBracket = equalPos + equalToNonSpaceOffset;
					if (posOfOpeningBracket >= length) {
						failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
					} else if (input.at(posOfOpeningBracket) != cOpeningCurlyBracket) {
						failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
					}

					int const posOfClosingBracket = findMatchingClosingCurlyBracket(input, length, posOfOpeningBracket);
					if (posOfClosingBracket == -1) {
						failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
					} else if ((posOfClosingBracket + 1) >= length || (input.at(posOfClosingBracket + 1) != cSemicolon)) {
						failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
					}

					QStringRef const arrayContent = input.midRef(posOfOpeningBracket + 1, posOfClosingBracket - posOfOpeningBracket - 1);
					std::vector<SqmArrayContents::ArrayEntry> arrayEntries = parseArray(arrayContent, input, offset);

					int const matchLength = posOfClosingBracket + 2 - offset;
					bool const isMultiLine = input.midRef(offset, matchLength).count(cN) > 0;

					if (isPlusArray) {
						objects.push_back(std::make_shared<SqmArrayWithFlags>(name, 1, arrayEntries));
					} else {
						objects.push_back(std::make_shared<SqmArray>(name, arrayEntries));
					}

					offset += matchLength;
					offset = advanceOverLineBreaks(input, offset, length);
				} else {
					// Property
					QString const name = input.mid(offset, (equalPos - equalToNonSpaceOffset) - offset + 1);

					if (name.compare("init") == 0 && offset > 933155) {
						std::cout << "Here in init." << std::endl;
					}

					equalToNonSpaceOffset = 1;
					while ((equalPos + equalToNonSpaceOffset < length) && ((input.at(equalPos + equalToNonSpaceOffset) == cS) || (input.at(equalPos + equalToNonSpaceOffset) == cT))) {
						++equalToNonSpaceOffset;
					}

					int posOfClosingSemicolon = -1;
					if (((equalPos + equalToNonSpaceOffset) < length) && (input.at(equalPos + equalToNonSpaceOffset) == cQuote)) {
						int const posOfOpeningQuote = equalPos + equalToNonSpaceOffset;
						int const posOfClosingQuote = findMatchingQuote(input, posOfOpeningQuote, length);
						if (posOfClosingQuote == -1) {
							failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
						} else if ((posOfClosingQuote + 1) >= length || (input.at(posOfClosingQuote + 1) != cSemicolon)) {
							failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
						}
						posOfClosingSemicolon = posOfClosingQuote + 1;
					} else {
						posOfClosingSemicolon = input.indexOf(cSemicolon, equalPos + equalToNonSpaceOffset);
						if (posOfClosingSemicolon == -1) {
							failureReport("Next input 'NEXT_INPUT' could not be parsed (line LINE, offset OFFSET)", input, offset);
						}
					}
					QString const value = input.mid(equalPos + equalToNonSpaceOffset, posOfClosingSemicolon - equalPos - equalToNonSpaceOffset);
					if (value.at(0) == cQuote) {
						objects.push_back(std::make_shared<SqmStringProperty>(name, SqmStructure::unescapeQuotesInString(value.mid(1, value.size() - 2))));
					} else if ((value.indexOf('.') > -1) || (value.indexOf('e', 0, Qt::CaseInsensitive) > -1)) {
						objects.push_back(std::make_shared<SqmFloatProperty>(name, value));
					} else {
						bool ok = false;
						int const i = value.toInt(&ok);
						
						if (!ok) {
							// Fall back to float for big numbers
							objects.push_back(std::make_shared<SqmFloatProperty>(name, value));
						} else {
							objects.push_back(std::make_shared<SqmIntProperty>(name, i));
						}
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

int TextualSqmParser::advanceOverLineBreaks(QString const& input, int offset, int length) const {
	while ((offset < length) && ((input.at(offset) == '\r') || (input.at(offset) == '\n'))) {
		++offset;
	}
	return offset;
}

int TextualSqmParser::findMatchingQuote(QString const& input, int posOfOpeningQuote, int length) const {
	int pos = posOfOpeningQuote + 1;
	static const QChar cQ('"');
	static const QChar cB('\\');
	static const QChar cn('n');
	while (pos < length) {
		if (input.at(pos) == cQ) {
			// Either string ends here, or its an "" escape, or a " \n " escape sequence
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

int TextualSqmParser::findMatchingClosingCurlyBracket(QString const& input, int length, int posOfOpeningBracket) const {
	int pos = posOfOpeningBracket + 1;
	int depth = 0;
	while (pos < length) {
		if (input.at(pos) == cOpeningCurlyBracket) {
			++depth;
		} else if (input.at(pos) == cQuote) {
			// Urgh, a string. Since the string might contain "escaped" curly brackets, we need to skip it...
			int const closingQuoteLocation = findMatchingQuote(input, pos, length);
			pos = closingQuoteLocation;
		} else if (input.at(pos) == cClosingCurlyBracket) {
			if (depth == 0) {
				return pos;
			}
			--depth;
		}
		++pos;
	}
	return -1;
}

void TextualSqmParser::failureReport(QString msg, QString const& file, int offset) const {
	static const QChar cN('\n');
	int const line = file.midRef(0, offset).count(cN);
	int const nextNewline = file.indexOf(cN, offset);
	int const length = nextNewline - offset;
	QString const nextInput = file.midRef(offset, length).trimmed().toString();

	msg = msg.replace(QStringLiteral("NEXT_INPUT"), nextInput);
	msg = msg.replace(QStringLiteral("LINE"), QString::number(line));
	msg = msg.replace(QStringLiteral("OFFSET"), QString::number(offset));

	LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Failed to parse SQM: " << msg.toStdString());
}
