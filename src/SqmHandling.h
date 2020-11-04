#ifndef ZEUSSQM_PARSER_SQMHANDLING_H_
#define ZEUSSQM_PARSER_SQMHANDLING_H_

#include <memory>
#include <vector>

#include "SqmArray.h"
#include "SqmClass.h"
#include "SqmObjectList.h"
#include "SqmProperty.h"

class SqmHandling {
public:
	virtual ~SqmHandling() {}

	struct Position {
		int x;
		int y;
		int z;
	};

	static SqmRoot nextItemIds(SqmRoot const& root, int& firstNextId, int requestedIdCount = 1);
	static SqmRoot addVrShapeObjects(SqmRoot const& root, std::vector<Position> const& positions);

private:
	static std::shared_ptr<SqmClass> newPosition(int x, int y, int z);
};

#endif
