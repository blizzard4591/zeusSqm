#include "BinarizedSqmParser.h"

#include <chrono>
#include <iostream>

#include <QDataStream>
#include <QTextStream>

#include "SqmDeleteClass.h"
#include "SqmExternClass.h"

#include "TextualSqmParser.h"
#include "exceptions/FormatErrorException.h"

BinarizedSqmParser::BinarizedSqmParser(bool beQuiet) : m_beQuiet(beQuiet) {
	//
}

QByteArray BinarizedSqmParser::stripComments(QByteArray const& input) {
	if (hasBinarizedSqmHeader(input)) {
		return input;
	}

	QTextStream stream(input);
	stream.setCodec("UTF-8");
	QString const missionFileData = stream.readAll();

	QString const strippedMission = TextualSqmParser::stripComments(missionFileData);

	QByteArray outputData;
	QTextStream outputStream(&outputData);
	outputStream.setCodec("UTF-8");
	outputStream << strippedMission;
	outputStream.flush();

	return outputData;
}

float BinarizedSqmParser::parseFloat(QByteArray const& data, int& offset) {
	float result = *reinterpret_cast<float const*>(data.constData() + offset);
	offset += 4;
	return result;
}

qint32 BinarizedSqmParser::parseInt32(QByteArray const& data, int& offset) {
	qint32 result = *reinterpret_cast<qint32 const*>(data.constData() + offset);
	offset += 4;
	return result;
}

quint32 BinarizedSqmParser::parseUInt32(QByteArray const& data, int& offset) {
	quint32 result = *reinterpret_cast<quint32 const*>(data.constData() + offset);
	offset += 4;
	return result;
}

quint8 BinarizedSqmParser::parseUint8(QByteArray const& data, int& offset) {
	quint8 result = *reinterpret_cast<quint8 const*>(data.constData() + offset);
	offset += 1;
	return result;
}

quint64 BinarizedSqmParser::parseCompressedInteger(QByteArray const& data, int& offset) {
	quint64 result = 0;
	QByteArray bytes;
	bytes.append(data.at(offset));
	quint8 v = parseUint8(data, offset);
	result += v;

	while (v & 0x80) {
		bytes.append(data.at(offset));
		v = parseUint8(data, offset);
		result += (v - 1uLL) * 0x80uLL;
	}

	return result;
}

QString BinarizedSqmParser::parseString(QByteArray const& data, int& offset) {
	QByteArray stringBytes;
	char c;
	c = data.at(offset++);
	while (c != '\0') {
		stringBytes.append(static_cast<char>(c));
		c = data.at(offset++);
	}
	return QString(stringBytes);
}

SqmArrayContents::ArrayEntry BinarizedSqmParser::parseArrayElement(QByteArray const& data, int& offset) {
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
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Unhandled Type '" << (int)type << "' in Array at offset " << offset << "!");
	}
}

SqmArrayContents BinarizedSqmParser::parseArrayContents(QByteArray const& data, int& offset) {
	quint64 const entryCount = parseCompressedInteger(data, offset);
	std::vector<SqmArrayContents::ArrayEntry> entries;
	for (quint64 i = 0; i < entryCount; ++i) {
		entries.push_back(parseArrayElement(data, offset));
	}
	return SqmArrayContents(entries);
}

std::shared_ptr<SqmArray> BinarizedSqmParser::parseArray(QByteArray const& data, int& offset) {
	QString const name = parseString(data, offset);
	return std::make_shared<SqmArray>(name, parseArrayContents(data, offset));
}

std::shared_ptr<SqmArrayWithFlags> BinarizedSqmParser::parseArrayWithFlags(QByteArray const& data, int& offset) {
	quint32 const flags = parseUInt32(data, offset);
	if (flags != 1) {
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Unhandled flags '" << (int)flags << "' in Array at offset " << offset << "!");
	}

	QString const name = parseString(data, offset);
	return std::make_shared<SqmArrayWithFlags>(name, flags, parseArrayContents(data, offset));
}

std::shared_ptr<SqmStructure> BinarizedSqmParser::parseClassEntry(QByteArray const& data, int& offset) {
	quint8 const id = parseUint8(data, offset);
	switch (id) {
	case 0:
	{
		QString const className = parseString(data, offset);
		int classOffset = parseUInt32(data, offset);
		if (classOffset >= data.size()) {
			LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Invalid class body offset '" << classOffset << "' in class '" << className.toStdString() <<"' et " << offset << "!");
		}
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
			LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Unhandled SubId '" << (int)subId << "' in ClassEntry at offset " << offset << "!");
		}
		break;
	}
	case 2:
	{
		// Array
		return parseArray(data, offset);
	}
	case 3:
	{
		// Extern Class
		QString const className = parseString(data, offset);
		return std::make_shared<SqmExternClass>(className);
	}
	case 4:
	{
		// Delete Class
		QString const className = parseString(data, offset);
		return std::make_shared<SqmDeleteClass>(className);
	}
	case 5:
	{
		// Array with Flags
		return parseArrayWithFlags(data, offset);
	}
	default:
	{
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Unhandled ClassEntry ID '" << (int)id << "' in ClassEntry at offset " << offset << "!");
	}
	}
}

SqmObjectList<SqmStructure> BinarizedSqmParser::parseClassBody(QByteArray const& data, int& offset) {
	QString inheritedClassname = parseString(data, offset);

	quint64 const entryCount = parseCompressedInteger(data, offset);
	std::vector<std::shared_ptr<SqmStructure>> items;

	for (quint64 i = 0; i < entryCount; ++i) {
		items.push_back(parseClassEntry(data, offset));
	}

	return SqmObjectList<SqmStructure>(inheritedClassname, items);
}

bool BinarizedSqmParser::hasBinarizedSqmHeader(QByteArray const& file) {
	if (file.size() < 4) return false;
	return ((file.at(0) == '\0') && (file.at(1) == 'r') && (file.at(2) == 'a') && (file.at(3) == 'P'));
}

SqmObjectList<SqmStructure> BinarizedSqmParser::parse(QByteArray& input) const {
	auto t1 = std::chrono::high_resolution_clock::now();

	if (!hasBinarizedSqmHeader(input)) {
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "Header of SQM file corrupted, either this is not a binarized SQM or its broken.");
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
	if (!m_beQuiet) {
		std::cout << "Parsing binarized SQM took " << duration << "ms." << std::endl;
	}

	return result;
}
