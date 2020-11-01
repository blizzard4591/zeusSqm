#ifndef ZEUSSQM_PARSER_SQMOBJECTLIST_H_
#define ZEUSSQM_PARSER_SQMOBJECTLIST_H_

#include "SqmStructure.h"
#include "SqmArray.h"
#include "SqmProperty.h"

#include <QHash>

#include <memory>
#include <vector>

class SqmClass;

template <typename T = SqmStructure>
class SqmObjectList : public SqmStructure {
public:
	typedef bool filterFunc(std::shared_ptr<T> const& a);

	SqmObjectList(std::vector<std::shared_ptr<T>> const& objects);
	virtual ~SqmObjectList() {}

	std::vector<std::shared_ptr<T>> const& getObjects() const;
	T* getByName(QString const& name) const;
	std::size_t size() const;

	typename std::vector<std::shared_ptr<T>>::const_iterator begin() const;
	typename std::vector<std::shared_ptr<T>>::const_iterator end() const;

	bool hasProperty(QString const& name) const;
	QString const& getPropertyValue(QString const& name) const;

	bool hasArray(QString const& name) const;
	SqmArray* getArray(QString const& name) const;

	virtual QString toSqm(int indentationLevel) const override;
	virtual QString const& getName() const override;

	SqmObjectList<T> filter(filterFunc f) const;

	SqmObjectList<SqmArray> onlyArrays() const;
	SqmObjectList<SqmClass> onlyClasses() const;
	SqmObjectList<SqmProperty> onlyProperties() const;

	/*template <std::enable_if_t<std::is_same<T, SqmClass>::value, int> = 0>
	SqmObjectList<T> filterRequireProperty(QString const& name) const;*/

	std::shared_ptr<SqmObjectList<T>> replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, std::shared_ptr<SqmStructure> current) const;
protected:
	std::vector<std::shared_ptr<T>> replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, bool& didChange) const;
private:
	std::vector<std::shared_ptr<T>> const m_objects;
	QHash<QString, std::shared_ptr<T>> const m_nameToObject;

	static QHash<QString, std::shared_ptr<T>> createNameToObjectMapping(std::vector<std::shared_ptr<T>> const& objects);
};

#endif
