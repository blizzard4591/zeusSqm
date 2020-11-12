#include "SqmExternClass.h"

#include "BinarizedSqm.h"

SqmExternClass::SqmExternClass(QString const& name) : m_name(name) {
	//
}

void SqmExternClass::toSqmStageOne(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	// Type
	BinarizedSqm::writeUint8(output, 3);
	// Class Name
	BinarizedSqm::writeString(output, getName());
}

bool SqmExternClass::toSqmStageTwo(QByteArray& output, QHash<SqmStructure const*, int>& stageTwoOffsetMap) const {
	return false;
}

QString SqmExternClass::toSqm(int indentationLevel, FormatType const& format) const {
	QString const indentString = QStringLiteral("\t").repeated(indentationLevel);

	return QStringLiteral("%1class %2;").arg(indentString).arg(getName());
}

QString const& SqmExternClass::getName() const {
	return m_name;
}

std::shared_ptr<SqmStructure> SqmExternClass::rename(QString const& newName) const {
	return std::make_shared<SqmExternClass>(newName);
}
