#ifndef ZEUSSQM_PARSER_SQMOBJECTLIST_H_
#define ZEUSSQM_PARSER_SQMOBJECTLIST_H_

#include "SqmStructure.h"
#include "SqmArray.h"
#include "SqmFloatProperty.h"
#include "SqmIntProperty.h"
#include "SqmStringProperty.h"

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
	std::shared_ptr<SqmProperty> getProperty(QString const& name) const;

	std::shared_ptr<SqmFloatProperty> getFloatProperty(QString const& name) const;
	std::shared_ptr<SqmIntProperty> getIntProperty(QString const& name) const;
	std::shared_ptr<SqmStringProperty> getStringProperty(QString const& name) const;

	QString const& getPropertyValueAsString(QString const& name) const;
	qint32 getPropertyValueAsInt(QString const& name) const;

	bool hasArray(QString const& name) const;
	SqmArray* getArray(QString const& name) const;

	QByteArray toBinarizedSqm() const;
	virtual void toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const override;
	virtual void toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int> const& stageTwoOffsetMap) const override;
	virtual QString toSqm(int indentationLevel) const override;
	virtual QString const& getName() const override;

	SqmObjectList<T> filter(filterFunc f) const;

	SqmObjectList<SqmArray> onlyArrays() const;
	SqmObjectList<SqmClass> onlyClasses() const;
	SqmObjectList<SqmProperty> onlyProperties() const;

	std::shared_ptr<SqmObjectList<T>> replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, std::shared_ptr<SqmObjectList<T>> const& current) const;
	std::shared_ptr<SqmObjectList<T>> remove(SqmStructure const& old, std::shared_ptr<SqmObjectList<T>> const& current) const;
protected:
	std::vector<std::shared_ptr<T>> replace(SqmStructure const& old, std::shared_ptr<SqmStructure> const& newStructure, bool& didChange) const;
	std::vector<std::shared_ptr<T>> remove(SqmStructure const& old, bool& didChange) const;
private:
	std::vector<std::shared_ptr<T>> const m_objects;
	QHash<QString, std::shared_ptr<T>> const m_nameToObject;

	static QHash<QString, std::shared_ptr<T>> createNameToObjectMapping(std::vector<std::shared_ptr<T>> const& objects);
};

typedef std::shared_ptr<SqmObjectList<SqmStructure>> SqmRoot;

#endif
