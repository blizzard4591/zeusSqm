#include "SqmArrayContents.h"

#include "BinarizedSqm.h"
#include "SqmStructure.h"
#include "exceptions/InternalErrorException.h"

SqmArrayContents::ArrayEntry::ArrayEntry() : type(ArrayEntryType::STRING), stringValue(), content() {
	//
}

SqmArrayContents::ArrayEntry::ArrayEntry(float value) : type(ArrayEntryType::FLOAT), stringValue(), content(value) {
	//
}

SqmArrayContents::ArrayEntry::ArrayEntry(float value, QString const& stringValue) : type(ArrayEntryType::FLOAT), stringValue(stringValue), content(value) {
	//
}

SqmArrayContents::ArrayEntry::ArrayEntry(qint32 value) : type(ArrayEntryType::INT), stringValue(), content(value) {
	//
}

SqmArrayContents::ArrayEntry::ArrayEntry(QString const& value) : type(ArrayEntryType::STRING), stringValue(), content(value) {
	//
}

SqmArrayContents::ArrayEntry::ArrayEntry(SqmArrayContents const& value) : type(ArrayEntryType::RECURSIVE_ARRAY), stringValue(), content(std::make_shared<SqmArrayContents>(value)) {
	//
}

SqmArrayContents::SqmArrayContents(std::vector<ArrayEntry> const& entries) : m_values(entries) {
	//
}

std::vector<SqmArrayContents::ArrayEntry> const& SqmArrayContents::getValues() const {
	return m_values;
}

bool SqmArrayContents::shoudBeMultiLine() const {
	bool isPure = true;
	for (int i = 0; i < m_values.size(); ++i) {
		if (m_values.at(i).type == ArrayEntryType::STRING) {
			isPure = false;
			break;
		} else if (m_values.at(i).type == ArrayEntryType::RECURSIVE_ARRAY) {
			if (std::get<std::shared_ptr<SqmArrayContents>>(m_values.at(i).content)->shoudBeMultiLine()) {
				isPure = false;
				break;
			}
		}
	}
	return !isPure;
}

void SqmArrayContents::toSqmStageOne(QByteArray& output) const {
	// Entry Count
	BinarizedSqm::writeCompressedInteger(output, m_values.size());
	for (int i = 0; i < m_values.size(); ++i) {
		switch (m_values.at(i).type) {
		case ArrayEntryType::STRING:
			BinarizedSqm::writeUint8(output, 0);
			BinarizedSqm::writeString(output, std::get<QString>(m_values.at(i).content));
			break;
		case ArrayEntryType::FLOAT:
			BinarizedSqm::writeUint8(output, 1);
			BinarizedSqm::writeFloat(output, std::get<float>(m_values.at(i).content));
			break;
		case ArrayEntryType::INT:
			BinarizedSqm::writeUint8(output, 2);
			BinarizedSqm::writeInt32(output, std::get<qint32>(m_values.at(i).content));
			break;
		case ArrayEntryType::RECURSIVE_ARRAY:
			BinarizedSqm::writeUint8(output, 3);
			std::get<std::shared_ptr<SqmArrayContents>>(m_values.at(i).content)->toSqmStageOne(output);
			break;
		default:
			LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Unhandled ArrayContents subType!");
		}
	}
}

QString SqmArrayContents::toSqm(int indentationLevel, bool& isMultiline, SqmStructure::FormatType const& format) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	isMultiline = shoudBeMultiLine() && format != SqmStructure::FormatType::SINGLESPACED;
	if (isMultiline) {
		QString result;
		result.append(indentString).append(QStringLiteral("{\r\n"));
		for (int i = 0; i < m_values.size(); ++i) {
			switch (m_values.at(i).type) {
			case ArrayEntryType::FLOAT:
			{
				if (!m_values.at(i).stringValue.isEmpty()) {
					result.append(indentString).append('\t').append(m_values.at(i).stringValue);
				} else {
					result.append(indentString).append('\t').append(SqmStructure::toFloatRepresentation(std::get<float>(m_values.at(i).content)));
				}
				break;
			}
			case ArrayEntryType::INT:
				result.append(indentString).append('\t').append(QString::number(std::get<qint32>(m_values.at(i).content)));
				break;
			case ArrayEntryType::RECURSIVE_ARRAY:
			{
				bool subIsMultiline;
				QString const subValue = std::get<std::shared_ptr<SqmArrayContents>>(m_values.at(i).content)->toSqm(indentationLevel + 1, subIsMultiline, format);
				if (subIsMultiline) {
					result.append(subValue);
				} else {
					result.append(indentString).append('\t').append(subValue);
				}
				break;
			}
			case ArrayEntryType::STRING:
			{
				result.append(indentString).append('\t').append('"').append(SqmStructure::escapeQuotesInString(std::get<QString>(m_values.at(i).content))).append('"');
				break;
			}
			default:
				throw;
			}
			
			if (i < (m_values.size() - 1)) {
				result.append(',');
			}		
			result.append(QStringLiteral("\r\n"));
		}
		result.append(indentString).append(QStringLiteral("}"));
		return result;
	} else {
		QString result = QStringLiteral("{");
		for (int i = 0; i < m_values.size(); ++i) {
			switch (m_values.at(i).type) {
			case ArrayEntryType::FLOAT:
			{
				if (!m_values.at(i).stringValue.isEmpty()) {
					result.append(m_values.at(i).stringValue);
				} else {
					result.append(SqmStructure::toFloatRepresentation(std::get<float>(m_values.at(i).content)));
				}
				break;
			}
			case ArrayEntryType::INT:
				result.append(QString::number(std::get<qint32>(m_values.at(i).content)));
				break;
			case ArrayEntryType::RECURSIVE_ARRAY:
			{
				bool subIsMultiline;
				QString const subValue = std::get<std::shared_ptr<SqmArrayContents>>(m_values.at(i).content)->toSqm(indentationLevel + 1, subIsMultiline, format);
				result.append(subValue);
				break;
			}
			case ArrayEntryType::STRING:
			{
				result.append('"').append(SqmStructure::escapeQuotesInString(std::get<QString>(m_values.at(i).content))).append('"');
				break;
			}
			default:
				throw;
			}

			if (i < (m_values.size() - 1)) {
				result.append(',');
			}
		}
		result.append(QStringLiteral("}"));
		return result;
	}
}

float SqmArrayContents::getEntryAsFloat(int index) const {
	if ((index < 0) || (index >= m_values.size())) {
		throw;
	}

	if (m_values.at(index).type == ArrayEntryType::FLOAT) {
		return std::get<float>(m_values.at(index).content);
	} else if (m_values.at(index).type == ArrayEntryType::INT) {
		return std::get<qint32>(m_values.at(index).content);
	}

	throw;
}
