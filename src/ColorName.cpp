#include "ColorName.h"

QString ColorNameHelper::toString(ColorName const& colorName) {
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
	case ColorName::UnknownSide: return QStringLiteral("UnknownSide");
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
	} else if (color.compare(QStringLiteral("Black"), Qt::CaseInsensitive) == 0) {
		return ColorName::Black;
	} else if (color.compare(QStringLiteral("Grey"), Qt::CaseInsensitive) == 0) {
		return ColorName::Grey;
	} else if (color.compare(QStringLiteral("Red"), Qt::CaseInsensitive) == 0) {
		return ColorName::Red;
	} else if (color.compare(QStringLiteral("Brown"), Qt::CaseInsensitive) == 0) {
		return ColorName::Brown;
	} else if (color.compare(QStringLiteral("Orange"), Qt::CaseInsensitive) == 0) {
		return ColorName::Orange;
	} else if (color.compare(QStringLiteral("Yellow"), Qt::CaseInsensitive) == 0) {
		return ColorName::Yellow;
	} else if (color.compare(QStringLiteral("Khaki"), Qt::CaseInsensitive) == 0) {
		return ColorName::Khaki;
	} else if (color.compare(QStringLiteral("Green"), Qt::CaseInsensitive) == 0) {
		return ColorName::Green;
	} else if (color.compare(QStringLiteral("Blue"), Qt::CaseInsensitive) == 0) {
		return ColorName::Blue;
	} else if (color.compare(QStringLiteral("Pink"), Qt::CaseInsensitive) == 0) {
		return ColorName::Pink;
	} else if (color.compare(QStringLiteral("White"), Qt::CaseInsensitive) == 0) {
		return ColorName::White;
	} else if (color.compare(QStringLiteral("BLUFOR"), Qt::CaseInsensitive) == 0) {
		return ColorName::BLUFOR;
	} else if (color.compare(QStringLiteral("OPFOR"), Qt::CaseInsensitive) == 0) {
		return ColorName::OPFOR;
	} else if (color.compare(QStringLiteral("Independent"), Qt::CaseInsensitive) == 0) {
		return ColorName::Independent;
	} else if (color.compare(QStringLiteral("Civilian"), Qt::CaseInsensitive) == 0) {
		return ColorName::Civilian;
	} else if ((color.compare(QStringLiteral("Unknown Side"), Qt::CaseInsensitive) == 0) || (color.compare(QStringLiteral("UnknownSide"), Qt::CaseInsensitive) == 0)) {
		return ColorName::UnknownSide;
	} else {
		return ColorName::INVALID;
	}
}
