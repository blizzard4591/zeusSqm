#include "SqmDeleteClass.h"

#include "BinarizedSqm.h"

SqmDeleteClass::SqmDeleteClass(QString const& name) : m_name(name) {
	//
}

void SqmDeleteClass::toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	// Type
	BinarizedSqm::writeUint8(output, 4);
	// Class Name
	BinarizedSqm::writeString(output, getName());
}

bool SqmDeleteClass::toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	return false;
}

QString SqmDeleteClass::toSqm(int indentationLevel, FormatType const& format) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	return QStringLiteral("%1delete %2;").arg(indentString).arg(getName());
}

QString const& SqmDeleteClass::getName() const {
	return m_name;
}

std::shared_ptr<SqmStructure> SqmDeleteClass::rename(QString const& newName) const {
	return std::make_shared<SqmDeleteClass>(newName);
}
