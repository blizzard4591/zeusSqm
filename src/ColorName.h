#ifndef ZEUSSQM_SQM_COLORNAME_H_
#define ZEUSSQM_SQM_COLORNAME_H_

#include <QString>

enum class ColorName {
	INVALID,
	Default,
	Black,
	Grey,
	Red,
	Brown,
	Orange,
	Yellow,
	Khaki,
	Green,
	Blue,
	Pink,
	White,
	BLUFOR,
	OPFOR,
	Independent,
	Civilian,
	UnknownSide
};

class ColorNameHelper {
public:
	static QString toString(ColorName const& colorName);
	static QString toDisplayString(ColorName const& colorName);
	static ColorName fromString(QString const& color);
};

#endif
