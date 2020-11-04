#ifndef ZEUSSQM_PARSER_SQMHANDLING_H_
#define ZEUSSQM_PARSER_SQMHANDLING_H_

#include <memory>

#include "SqmArray.h"
#include "SqmClass.h"
#include "SqmObjectList.h"
#include "SqmProperty.h"

class SqmHandling {
public:
	virtual ~SqmHandling() {}

	static SqmRoot nextItemId(SqmRoot const& root, int& nextId);
	static SqmRoot newVrShapeObject(SqmRoot const& root, int x, int y, int z);
private:
	static std::shared_ptr<SqmClass> newPosition(int x, int y, int z);
};

#endif
