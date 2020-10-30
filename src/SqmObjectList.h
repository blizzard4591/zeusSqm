#ifndef ZEUSSQM_PARSER_SQMOBJECTLIST_H_
#define ZEUSSQM_PARSER_SQMOBJECTLIST_H_

#include "SqmStructure.h"

#include <memory>
#include <vector>

class SqmObjectList : public SqmStructure {
public:
	SqmObjectList(std::vector<std::shared_ptr<SqmStructure>> const& objects);
	virtual ~SqmObjectList() {}

	std::vector<std::shared_ptr<SqmStructure>> const& getObjects() const;

	virtual QString toSqm(int indentationLevel) const override;
private:
	std::vector<std::shared_ptr<SqmStructure>> const m_objects;
};

#endif
