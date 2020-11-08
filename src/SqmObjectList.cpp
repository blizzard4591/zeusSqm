#include "SqmObjectList.h"

#include "BinarizedSqm.h"
#include "SqmClass.h"
#include "exceptions/InternalErrorException.h"

#include <QHash>

#include <iostream>
#include <memory>

template <typename T>
SqmObjectList<T>::SqmObjectList(std::vector<std::shared_ptr<T>> const& objects) : m_objects(objects), m_nameToObject(createNameToObjectMapping(objects)) {
	//
}

template <typename T>
std::vector<std::shared_ptr<T>> const& SqmObjectList<T>::getObjects() const {
	return m_objects;
}

template <typename T>
QByteArray SqmObjectList<T>::toBinarizedSqm() const {
	QByteArray result;
	QHash<SqmStructure const*, int> stageTwoOffsetMap;

	result.append("\0raP", 4);
	BinarizedSqm::writeUint32(result, 0);
	BinarizedSqm::writeUint32(result, 8);
	int const enumOffset = BinarizedSqm::writeUint32(result, 0);

	stageTwoOffsetMap.insert(this, -1);
	this->toSqmStageTwo(result, stageTwoOffsetMap);

	return result;
}

template <typename T>
void SqmObjectList<T>::toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Failed to write binarized SQM, toSqmStageOne called on ObjectList!");
}

template <typename T>
void SqmObjectList<T>::toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int> const& stageTwoOffsetMap) const {
	if (!stageTwoOffsetMap.contains(this)) {
		LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Failed to write binarized SQM, offset correction for class '" << getName().toStdString() << "' failed!");
	}
	int const offset = stageTwoOffsetMap.value(this);
	// -1 marks this being the root classbody, which does not care about its location
	if (offset != -1) {
		int const currentOffset = output.size();
		*reinterpret_cast<quint32*>(output.data() + offset) = currentOffset;
	}

	// Inherited className, unsupported by us
	BinarizedSqm::writeUint8(output, 0);
	// EntryCount as compressed integer
	BinarizedSqm::writeCompressedInteger(output, m_objects.size());
	// Class Entries
	QHash<SqmStructure const*, int> localOffsetMap = stageTwoOffsetMap;
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		m_objects.at(i)->toSqmStageOne(output, localOffsetMap);
	}

	// Write delayed stageTwos
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		m_objects.at(i)->toSqmStageTwo(output, localOffsetMap);
	}
}

template <typename T>
QString SqmObjectList<T>::toSqm(int indentationLevel) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	QString result;
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		result.append(m_objects.at(i)->toSqm(indentationLevel));
	}
	return result;
}

template <typename T>
T* SqmObjectList<T>::getByName(QString const& name) const {
	if (m_nameToObject.contains(name)) {
		return m_nameToObject.constFind(name)->get();
	}
	return nullptr;
}

template <typename T>
std::size_t SqmObjectList<T>::size() const {
	return m_objects.size();
}

template <typename T>
QString const& SqmObjectList<T>::getName() const {
	throw;
}

template <typename T>
QHash<QString, std::shared_ptr<T>> SqmObjectList<T>::createNameToObjectMapping(std::vector<std::shared_ptr<T>> const& objects) {
	QHash<QString, std::shared_ptr<T>> result;
	for (std::size_t i = 0; i < objects.size(); ++i) {
		result.insert(objects.at(i)->getName(), objects.at(i));
	}
	return result;
}

template <typename T>
SqmObjectList<T> SqmObjectList<T>::filter(filterFunc f) const {
	std::vector<std::shared_ptr<T>> result;
	std::copy_if(m_objects.begin(), m_objects.end(), std::back_inserter(result), f);
	return SqmObjectList<T>(result);
}

template <typename T>
typename std::vector<std::shared_ptr<T>>::const_iterator SqmObjectList<T>::begin() const {
	return getObjects().cbegin();
}

template <typename T>
typename std::vector<std::shared_ptr<T>>::const_iterator SqmObjectList<T>::end() const {
	return getObjects().cend();
}

template <typename T>
bool SqmObjectList<T>::hasProperty(QString const& name) const {
	SqmProperty* const property = dynamic_cast<SqmProperty*>(getByName(name));
	return (property != nullptr);
}

template <typename T>
std::shared_ptr<SqmProperty> SqmObjectList<T>::getProperty(QString const& name) const {
	if (m_nameToObject.contains(name)) {
		std::shared_ptr<SqmProperty> property = std::dynamic_pointer_cast<SqmProperty>(*m_nameToObject.constFind(name));
		if (property != nullptr) {
			return property;
		}
		LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Property '" << name.toStdString() << "' is not of type PROPERTY!");
	}
	LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Property '" << name.toStdString() << "' does not exist!");
}

template <typename T>
std::shared_ptr<SqmFloatProperty> SqmObjectList<T>::getFloatProperty(QString const& name) const {
	if (m_nameToObject.contains(name)) {
		std::shared_ptr<SqmFloatProperty> property = std::dynamic_pointer_cast<SqmFloatProperty>(*m_nameToObject.constFind(name));
		if (property != nullptr) {
			return property;
		}
		LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Property '" << name.toStdString() << "' is not of type FLOAT!");
	}
	LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Property '" << name.toStdString() << "' does not exist!");
}

template <typename T>
std::shared_ptr<SqmIntProperty> SqmObjectList<T>::getIntProperty(QString const& name) const {
	if (m_nameToObject.contains(name)) {
		std::shared_ptr<SqmIntProperty> property = std::dynamic_pointer_cast<SqmIntProperty>(*m_nameToObject.constFind(name));
		if (property != nullptr) {
			return property;
		}
		LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Property '" << name.toStdString() << "' is not of type INT!");
	}
	LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Property '" << name.toStdString() << "' does not exist!");
}

template <typename T>
std::shared_ptr<SqmStringProperty> SqmObjectList<T>::getStringProperty(QString const& name) const {
	if (m_nameToObject.contains(name)) {
		std::shared_ptr<SqmStringProperty> property = std::dynamic_pointer_cast<SqmStringProperty>(*m_nameToObject.constFind(name));
		if (property != nullptr) {
			return property;
		}
		LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Property '" << name.toStdString() << "' is not of type STRING!");
	}
	LOG_AND_THROW(zeusops::exceptions::InternalErrorException, "Property '" << name.toStdString() << "' does not exist!");
}

template <typename T>
QString const& SqmObjectList<T>::getPropertyValueAsString(QString const& name) const {
	return getStringProperty(name)->getValueAsString();
}

template <typename T>
qint32 SqmObjectList<T>::getPropertyValueAsInt(QString const& name) const {
	return getProperty(name)->getValueAsInt();
}

template <typename T>
bool SqmObjectList<T>::hasArray(QString const& name) const {
	SqmArray* const property = dynamic_cast<SqmArray*>(getByName(name));
	return (property != nullptr);
}

template <typename T>
SqmArray* SqmObjectList<T>::getArray(QString const& name) const {
	SqmArray* const array = dynamic_cast<SqmArray*>(getByName(name));
	if (array == nullptr) {
		throw;
	}

	return array;
}

template <typename T>
SqmObjectList<SqmArray> SqmObjectList<T>::onlyArrays() const {
	std::vector<std::shared_ptr<SqmArray>> result;
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		std::shared_ptr<SqmArray> const array = std::dynamic_pointer_cast<SqmArray>(m_objects.at(i));
		if (array != nullptr) {
			result.push_back(array);
		}
	}
	return result;
}

template <typename T>
SqmObjectList<SqmClass> SqmObjectList<T>::onlyClasses() const {
	std::vector<std::shared_ptr<SqmClass>> result;
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		std::shared_ptr<SqmClass> const array = std::dynamic_pointer_cast<SqmClass>(m_objects.at(i));
		if (array != nullptr) {
			result.push_back(array);
		}
	}
	return result;
}

template <typename T>
SqmObjectList<SqmProperty> SqmObjectList<T>::onlyProperties() const {
	std::vector<std::shared_ptr<SqmProperty>> result;
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		std::shared_ptr<SqmProperty> const array = std::dynamic_pointer_cast<SqmProperty>(m_objects.at(i));
		if (array != nullptr) {
			result.push_back(array);
		}
	}
	return result;
}

template <typename T>
std::vector<std::shared_ptr<T>> SqmObjectList<T>::remove(SqmStructure const& old, bool& didChange) const {
	std::vector<std::shared_ptr<T>> objects;
	didChange = false;
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		if (*m_objects.at(i) == old) {
			didChange = true;
			continue;
		}

		std::shared_ptr<SqmClass> const sqmClass = std::dynamic_pointer_cast<SqmClass>(m_objects.at(i));
		if (sqmClass != nullptr) {
			std::shared_ptr<SqmClass> const replacedSqmClass = sqmClass->remove(old, sqmClass);
			std::shared_ptr<T> const downcastedReplaced = std::dynamic_pointer_cast<T>(replacedSqmClass);
			objects.push_back(downcastedReplaced);
			if (*sqmClass != *downcastedReplaced) {
				didChange = true;
			}
		} else {
			objects.push_back(m_objects.at(i));
		}
	}

	return objects;
}

template <typename T>
std::vector<std::shared_ptr<T>> SqmObjectList<T>::replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, bool& didChange) const {
	std::vector<std::shared_ptr<T>> objects;
	didChange = false;
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		if (*m_objects.at(i) == old) {
			std::shared_ptr<T> upcastedPtr = std::dynamic_pointer_cast<T>(newStructure);
			if (upcastedPtr == nullptr) {
				throw;
			}
			objects.push_back(upcastedPtr);
			didChange = true;
			continue;
		}

		std::shared_ptr<SqmClass> const sqmClass = std::dynamic_pointer_cast<SqmClass>(m_objects.at(i));
		if (sqmClass != nullptr) {
			std::shared_ptr<SqmClass> const replacedSqmClass = sqmClass->replace(old, newStructure, sqmClass);
			std::shared_ptr<T> const downcastedReplaced = std::dynamic_pointer_cast<T>(replacedSqmClass);
			objects.push_back(downcastedReplaced);
			if (*sqmClass != *downcastedReplaced) {
				didChange = true;
			}
		} else {
			objects.push_back(m_objects.at(i));
		}
	}

	return objects;
}


template <>
std::vector<std::shared_ptr<SqmArray>> SqmObjectList<SqmArray>::replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, bool& didChange) const {
	std::vector<std::shared_ptr<SqmArray>> objects;
	didChange = false;
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		if (*m_objects.at(i) == old) {
			std::shared_ptr<SqmArray> upcastedPtr = std::dynamic_pointer_cast<SqmArray>(newStructure);
			if (upcastedPtr == nullptr) {
				throw;
			}
			objects.push_back(upcastedPtr);
			didChange = true;
			continue;
		}

		objects.push_back(m_objects.at(i));
	}

	return objects;
}

template <>
std::vector<std::shared_ptr<SqmProperty>> SqmObjectList<SqmProperty>::replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, bool& didChange) const {
	std::vector<std::shared_ptr<SqmProperty>> objects;
	didChange = false;
	for (std::size_t i = 0; i < m_objects.size(); ++i) {
		if (*m_objects.at(i) == old) {
			std::shared_ptr<SqmProperty> upcastedPtr = std::dynamic_pointer_cast<SqmProperty>(newStructure);
			if (upcastedPtr == nullptr) {
				throw;
			}
			objects.push_back(upcastedPtr);
			didChange = true;
			continue;
		}

		objects.push_back(m_objects.at(i));
	}

	return objects;
}

template <typename T>
std::shared_ptr<SqmObjectList<T>> SqmObjectList<T>::replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, std::shared_ptr<SqmObjectList<T>> const& current) const {
	bool hasChange = false;
	std::vector<std::shared_ptr<T>> objects = replace(old, newStructure, hasChange);
	if (!hasChange) {
		return current;
	}
	return std::make_shared<SqmObjectList<T>>(objects);
}

template <typename T>
std::shared_ptr<SqmObjectList<T>> SqmObjectList<T>::remove(SqmStructure const& old, std::shared_ptr<SqmObjectList<T>> const& current) const {
	bool hasChange = false;
	std::vector<std::shared_ptr<T>> objects = remove(old, hasChange);
	if (!hasChange) {
		return current;
	}
	return std::make_shared<SqmObjectList<T>>(objects);
}

// Explicit template instantiation
template class SqmObjectList<SqmStructure>;
template class SqmObjectList<SqmArray>;
template class SqmObjectList<SqmClass>;
template class SqmObjectList<SqmProperty>;
