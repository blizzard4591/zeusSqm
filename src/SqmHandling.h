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

	struct FloatPosition {
		float x;
		float y;
		float z;
	};

	static SqmRoot nextItemIds(SqmRoot const& root, int& firstNextId, int requestedIdCount = 1);
	static SqmRoot nextLayerIds(SqmRoot const& root, int& firstNextId, int requestedIdCount = 1);
	static SqmRoot addVrShapeObjects(SqmRoot const& root, std::vector<Position> const& positions);
	static SqmRoot addObjects(SqmRoot const& root, QString const& name, std::vector<FloatPosition> const& positions);

private:
	static std::shared_ptr<SqmClass> newPosition(int x, int y, int z);
	static std::shared_ptr<SqmClass> newPosition(float x, float y, float z);
	static std::shared_ptr<SqmClass> attributesWithDisabledSimulation();
	static std::shared_ptr<SqmClass> disabledDamage();
};

#endif
