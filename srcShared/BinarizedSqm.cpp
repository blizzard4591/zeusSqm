#include "BinarizedSqm.h"

#include "exceptions/FormatErrorException.h"

void BinarizedSqm::writeFloat(QByteArray& output, float f) {
	QByteArray bytes(4, '\0');
	*reinterpret_cast<float*>(bytes.data()) = f;
	output.append(bytes);
}

void BinarizedSqm::writeInt32(QByteArray& output, qint32 i) {
	QByteArray bytes(4, '\0');
	*reinterpret_cast<qint32*>(bytes.data()) = i;
	output.append(bytes);
}

int BinarizedSqm::writeUint32(QByteArray& output, quint32 u) {
	QByteArray bytes(4, '\0');
	*reinterpret_cast<quint32*>(bytes.data()) = u;
	output.append(bytes);
	return output.size() - 4;
}

void BinarizedSqm::writeUint8(QByteArray& output, quint8 u) {
	QByteArray bytes(1, '\0');
	*reinterpret_cast<quint8*>(bytes.data()) = u;
	output.append(bytes);
}

void BinarizedSqm::writeCompressedInteger(QByteArray& output, quint64 u) {
	quint8 v = u % 128;
	bool hasMore = false;
	if (u >= 0x80uLL) {
		hasMore = true;
		v += 0x80uLL;
		u -= v;
		u = u >> 7;
	}
	BinarizedSqm::writeUint8(output, v);

	while (hasMore) {
		u += 1;
		v = u % 128;
		hasMore = false;
		if (u >= 0x80uLL) {
			hasMore = true;
			v += 0x80uLL;
			u -= v;
		}
		BinarizedSqm::writeUint8(output, v);
	}
}

void BinarizedSqm::writeString(QByteArray& output, QString const& s) {
	output.append(s.toUtf8());
	output.append('\0');
}

void BinarizedSqm::overwriteOffset(QByteArray& output, int where, quint32 offset) {
	*reinterpret_cast<quint32*>(output.data() + where) = offset;
}
