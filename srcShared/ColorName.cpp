#include "ColorName.h"

QString ColorNameHelper::toString(ColorName const& colorName) {
	switch (colorName) {
	case ColorName::Default: return QStringLiteral("Default");
	case ColorName::Black: return QStringLiteral("ColorBlack");
	case ColorName::Grey: return QStringLiteral("ColorGrey");
	case ColorName::Red: return QStringLiteral("ColorRed");
	case ColorName::Brown: return QStringLiteral("ColorBrown");
	case ColorName::Orange: return QStringLiteral("ColorOrange");
	case ColorName::Yellow: return QStringLiteral("ColorYellow");
	case ColorName::Khaki: return QStringLiteral("ColorKhaki");
	case ColorName::Green: return QStringLiteral("ColorGreen");
	case ColorName::Blue: return QStringLiteral("ColorBlue");
	case ColorName::Pink: return QStringLiteral("ColorPink");
	case ColorName::White: return QStringLiteral("ColorWhite");
	case ColorName::BLUFOR: return QStringLiteral("ColorWEST");
	case ColorName::OPFOR: return QStringLiteral("ColorEAST");
	case ColorName::Independent: return QStringLiteral("ColorGUER");
	case ColorName::Civilian: return QStringLiteral("ColorCIV");
	case ColorName::UnknownSide: return QStringLiteral("ColorUNKNOWN");
	case ColorName::INVALID:
	default: return QStringLiteral("INVALID");
	}
}

QString ColorNameHelper::toDisplayString(ColorName const& colorName) {
	switch (colorName) {
	case ColorName::Default: return QStringLiteral("Default");
	case ColorName::Black: return QStringLiteral("Black");
	case ColorName::Grey: return QStringLiteral("Grey");
	case ColorName::Red: return QStringLiteral("Red");
	case ColorName::Brown: return QStringLiteral("Brown");
	case ColorName::Orange: return QStringLiteral("Orange");
	case ColorName::Yellow: return QStringLiteral("Yellow");
	case ColorName::Khaki: return QStringLiteral("Khaki");
	case ColorName::Green: return QStringLiteral("Green");
	case ColorName::Blue: return QStringLiteral("Blue");
	case ColorName::Pink: return QStringLiteral("Pink");
	case ColorName::White: return QStringLiteral("White");
	case ColorName::BLUFOR: return QStringLiteral("BLUFOR");
	case ColorName::OPFOR: return QStringLiteral("OPFOR");
	case ColorName::Independent: return QStringLiteral("Independent");
	case ColorName::Civilian: return QStringLiteral("Civilian");
	case ColorName::UnknownSide: return QStringLiteral("Unknown Side");
	case ColorName::INVALID:
	default: return QStringLiteral("INVALID");
	}
}

ColorName ColorNameHelper::fromString(QString const& color) {
	if (color.compare(QStringLiteral("Default"), Qt::CaseInsensitive) == 0) {
		return ColorName::Default;
	} else if (color.compare(QStringLiteral("ColorBlack"), Qt::CaseInsensitive) == 0) {
		return ColorName::Black;
	} else if (color.compare(QStringLiteral("ColorGrey"), Qt::CaseInsensitive) == 0) {
		return ColorName::Grey;
	} else if (color.compare(QStringLiteral("ColorRed"), Qt::CaseInsensitive) == 0) {
		return ColorName::Red;
	} else if (color.compare(QStringLiteral("ColorBrown"), Qt::CaseInsensitive) == 0) {
		return ColorName::Brown;
	} else if (color.compare(QStringLiteral("ColorOrange"), Qt::CaseInsensitive) == 0) {
		return ColorName::Orange;
	} else if (color.compare(QStringLiteral("ColorYellow"), Qt::CaseInsensitive) == 0) {
		return ColorName::Yellow;
	} else if (color.compare(QStringLiteral("ColorKhaki"), Qt::CaseInsensitive) == 0) {
		return ColorName::Khaki;
	} else if (color.compare(QStringLiteral("ColorGreen"), Qt::CaseInsensitive) == 0) {
		return ColorName::Green;
	} else if (color.compare(QStringLiteral("ColorBlue"), Qt::CaseInsensitive) == 0) {
		return ColorName::Blue;
	} else if (color.compare(QStringLiteral("ColorPink"), Qt::CaseInsensitive) == 0) {
		return ColorName::Pink;
	} else if (color.compare(QStringLiteral("ColorWhite"), Qt::CaseInsensitive) == 0) {
		return ColorName::White;
	} else if (color.compare(QStringLiteral("ColorWEST"), Qt::CaseInsensitive) == 0) {
		return ColorName::BLUFOR;
	} else if (color.compare(QStringLiteral("ColorEAST"), Qt::CaseInsensitive) == 0) {
		return ColorName::OPFOR;
	} else if (color.compare(QStringLiteral("ColorGUER"), Qt::CaseInsensitive) == 0) {
		return ColorName::Independent;
	} else if (color.compare(QStringLiteral("ColorCIV"), Qt::CaseInsensitive) == 0) {
		return ColorName::Civilian;
	} else if ((color.compare(QStringLiteral("Unknown Side"), Qt::CaseInsensitive) == 0) || (color.compare(QStringLiteral("ColorUNKNOWN"), Qt::CaseInsensitive) == 0)) {
		return ColorName::UnknownSide;
	} else {
		return ColorName::INVALID;
	}
}
