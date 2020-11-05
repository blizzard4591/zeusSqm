#include "SqmArrayContents.h"

#include "SqmStructure.h"

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

QString SqmArrayContents::toSqm(int indentationLevel, bool& isMultiline) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	isMultiline = shoudBeMultiLine();
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
				QString const subValue = std::get<std::shared_ptr<SqmArrayContents>>(m_values.at(i).content)->toSqm(indentationLevel + 1, subIsMultiline);
				if (subIsMultiline) {
					result.append(subValue);
				} else {
					result.append(indentString).append('\t').append(subValue);
				}
				break;
			}
			case ArrayEntryType::STRING:
			{
				result.append(indentString).append('\t').append('"').append(std::get<QString>(m_values.at(i).content)).append('"');
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
				QString const subValue = std::get<std::shared_ptr<SqmArrayContents>>(m_values.at(i).content)->toSqm(indentationLevel + 1, subIsMultiline);
				result.append(subValue);
				break;
			}
			case ArrayEntryType::STRING:
			{
				result.append('"').append(std::get<QString>(m_values.at(i).content)).append('"');
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
