#include "FillName.h"

QString FillNameHelper::toString(FillName const& fillName) {
	switch (fillName) {
	case FillName::Solid: return QStringLiteral("Solid");
	case FillName::SolidFull: return QStringLiteral("SolidFull");
	case FillName::SolidBorder: return QStringLiteral("SolidBorder");
	case FillName::Border: return QStringLiteral("Border");
	case FillName::Horizontal: return QStringLiteral("Horizontal");
	case FillName::Vertical: return QStringLiteral("Vertical");
	case FillName::Grid: return QStringLiteral("Grid");
	case FillName::FDiagonal: return QStringLiteral("FDiagonal");
	case FillName::BDiagonal: return QStringLiteral("BDiagonal");
	case FillName::DiagGrid: return QStringLiteral("DiagGrid");
	case FillName::Cross: return QStringLiteral("Cross");
	case FillName::INVALID:
	default: return QStringLiteral("INVALID");
	}
}

QString FillNameHelper::toDisplayString(FillName const& colorName) {
	switch (colorName) {
	case FillName::Solid: return QStringLiteral("Solid");
	case FillName::SolidFull: return QStringLiteral("Solid Full");
	case FillName::SolidBorder: return QStringLiteral("Solid Border");
	case FillName::Border: return QStringLiteral("Border");
	case FillName::Horizontal: return QStringLiteral("Horizontal");
	case FillName::Vertical: return QStringLiteral("Vertical");
	case FillName::Grid: return QStringLiteral("Grid");
	case FillName::FDiagonal: return QStringLiteral("Forward Diagonal");
	case FillName::BDiagonal: return QStringLiteral("Backwards Diagonal");
	case FillName::DiagGrid: return QStringLiteral("Diagonal Grid");
	case FillName::Cross: return QStringLiteral("Cross");
	case FillName::INVALID:
	default: return QStringLiteral("INVALID");
	}
}

FillName FillNameHelper::fromString(QString const& color) {
	if (color.compare(QStringLiteral("Solid"), Qt::CaseInsensitive) == 0) {
		return FillName::Solid;
	} else if ((color.compare(QStringLiteral("SolidFull"), Qt::CaseInsensitive) == 0) || (color.compare(QStringLiteral("SolidFull"), Qt::CaseInsensitive) == 0)) {
		return FillName::SolidFull;
	} else if ((color.compare(QStringLiteral("SolidBorder"), Qt::CaseInsensitive) == 0) || (color.compare(QStringLiteral("Solid Border"), Qt::CaseInsensitive) == 0)) {
		return FillName::SolidBorder;
	} else if (color.compare(QStringLiteral("Border"), Qt::CaseInsensitive) == 0) {
		return FillName::Border;
	} else if (color.compare(QStringLiteral("Horizontal"), Qt::CaseInsensitive) == 0) {
		return FillName::Horizontal;
	} else if (color.compare(QStringLiteral("Vertical"), Qt::CaseInsensitive) == 0) {
		return FillName::Vertical;
	} else if (color.compare(QStringLiteral("Grid"), Qt::CaseInsensitive) == 0) {
		return FillName::Grid;
	} else if ((color.compare(QStringLiteral("FDiagonal"), Qt::CaseInsensitive) == 0) || (color.compare(QStringLiteral("Forward Diagonal"), Qt::CaseInsensitive) == 0)) {
		return FillName::FDiagonal;
	} else if ((color.compare(QStringLiteral("BDiagonal"), Qt::CaseInsensitive) == 0) || (color.compare(QStringLiteral("Backwards Diagonal"), Qt::CaseInsensitive) == 0)) {
		return FillName::BDiagonal;
	} else if ((color.compare(QStringLiteral("DiagGrid"), Qt::CaseInsensitive) == 0) || (color.compare(QStringLiteral("Diagonal Grid"), Qt::CaseInsensitive) == 0)) {
		return FillName::DiagGrid;
	} else if (color.compare(QStringLiteral("Cross"), Qt::CaseInsensitive) == 0) {
		return FillName::Cross;
	} else {
		return FillName::INVALID;
	}
}
