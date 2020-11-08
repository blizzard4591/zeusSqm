#ifndef ZEUSSQM_PARSER_BINARIZEDSQM_H_
#define ZEUSSQM_PARSER_BINARIZEDSQM_H_

#include <QByteArray>
#include <QString>


class BinarizedSqm {
public:
	static void writeFloat(QByteArray& output, float f);
	static void writeInt32(QByteArray& output, qint32 i);
	static int writeUint32(QByteArray& output, quint32 u);
	static void writeUint8(QByteArray& output, quint8 u);
	static void writeCompressedInteger(QByteArray& output, quint64 u);
	static void writeString(QByteArray& output, QString const& s);
private:
	//
};

#endif
