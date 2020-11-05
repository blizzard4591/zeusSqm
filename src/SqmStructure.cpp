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

QString SqmStructure::escapeQuotesInString(QString const& s) {
	static const QChar cQ('"');
	if (s.indexOf(cQ) == -1) return s;

	QString result;
	result.reserve(s.size());
	static const QChar cS(' ');
	static const QChar cB('\\');
	static const QChar cn('n');
	for (int pos = 0; pos < s.size(); ++pos) {
		if (s.at(pos) == cQ) {
			// Its either an " requiring escape, or a " \n " sequence
			if ((pos + 5) < s.size() && (s.at(pos + 1) == cS) && (s.at(pos + 2) == cB) && (s.at(pos + 3) == cn) && (s.at(pos + 4) == cS) && (s.at(pos + 5) == cQ)) {
				for (int i = 0; i < 6; ++i) result.append(s.at(pos + i));
				pos += 5;
			} else {
				result.append(cQ).append(cQ);
			}
		} else {
			result.append(s.at(pos));
		}
	}
	return result;
}
