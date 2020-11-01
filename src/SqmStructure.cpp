#include "SqmStructure.h"

int64_t SqmStructure::getNextUniqueId() {
	static int64_t counter = 0;
	return counter++;
}

SqmStructure::SqmStructure() : uniqueId(getNextUniqueId()) {
	//
}

SqmStructure::SqmStructure(SqmStructure const& other) : uniqueId(other.uniqueId) {
	//
}

int64_t SqmStructure::getUniqueId() const {
	return uniqueId;
}

bool SqmStructure::operator ==(SqmStructure const& other) const {
	return (uniqueId == other.uniqueId);
}
