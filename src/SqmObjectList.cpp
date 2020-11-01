#include "SqmObjectList.h"

#include "SqmClass.h"

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
QString const& SqmObjectList<T>::getPropertyValue(QString const& name) const {
	SqmProperty* const property = dynamic_cast<SqmProperty*>(getByName(name));
	if (property == nullptr) {
		throw;
	}

	return property->getValue();
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

// Explicit template instantiation
template class SqmObjectList<SqmStructure>;
template class SqmObjectList<SqmArray>;
template class SqmObjectList<SqmClass>;
template class SqmObjectList<SqmProperty>;
