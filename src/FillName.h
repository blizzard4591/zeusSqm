#ifndef ZEUSSQM_SQM_FILLNAME_H_
#define ZEUSSQM_SQM_FILLNAME_H_

#include <QString>

enum class FillName {
	INVALID,
	Solid,
	SolidFull,
	SolidBorder,
	Border,
	Horizontal,
	Vertical,
	Grid,
	FDiagonal,
	BDiagonal,
	DiagGrid,
	Cross
};

class FillNameHelper {
public:
	static QString toString(FillName const& colorName);
	static QString toDisplayString(FillName const& colorName);
	static FillName fromString(QString const& color);
};

#endif
