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

QString SqmStructure::toFloatRepresentation(float f) {
	QString result = QString::number(f, 'g', 8);
	int pos = result.indexOf(QStringLiteral("e-0"));
	if (pos > -1) {
		if (result.at(pos + 3) != '0') {
			result.insert(pos + 3, '0');
		}
	}
	return result;
}
