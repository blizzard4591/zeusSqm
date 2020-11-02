#include "MarkerType.h"

QString MarkerTypeHelper::toString(MarkerType const& fillName) {
	switch (fillName) {
	case MarkerType::BLUFOR_ANTIAIR: return QStringLiteral("BLUFOR_ANTIAIR");
	case MarkerType::BLUFOR_ARMOR: return QStringLiteral("BLUFOR_ARMOR");
	case MarkerType::BLUFOR_ARTILLERY: return QStringLiteral("BLUFOR_ARTILLERY");
	case MarkerType::BLUFOR_HELICOPTER: return QStringLiteral("BLUFOR_HELICOPTER");
	case MarkerType::BLUFOR_HQ: return QStringLiteral("BLUFOR_HQ");
	case MarkerType::BLUFOR_INFANTRY: return QStringLiteral("BLUFOR_INFANTRY");
	case MarkerType::BLUFOR_INSTALLATION: return QStringLiteral("BLUFOR_INSTALLATION");
	case MarkerType::BLUFOR_MAINTENANCE: return QStringLiteral("BLUFOR_MAINTENANCE");
	case MarkerType::BLUFOR_MECHANIZED: return QStringLiteral("BLUFOR_MECHANIZED");
	case MarkerType::BLUFOR_MEDICAL: return QStringLiteral("BLUFOR_MEDICAL");
	case MarkerType::BLUFOR_MORTAR: return QStringLiteral("BLUFOR_MORTAR");
	case MarkerType::BLUFOR_MOTORIZED: return QStringLiteral("BLUFOR_MOTORIZED");
	case MarkerType::BLUFOR_NAVAL: return QStringLiteral("BLUFOR_NAVAL");
	case MarkerType::BLUFOR_ORDNANCE: return QStringLiteral("BLUFOR_ORDNANCE");
	case MarkerType::BLUFOR_PLANE: return QStringLiteral("BLUFOR_PLANE");
	case MarkerType::BLUFOR_RECON: return QStringLiteral("BLUFOR_RECON");
	case MarkerType::BLUFOR_SERVICE: return QStringLiteral("BLUFOR_SERVICE");
	case MarkerType::BLUFOR_SUPPORT: return QStringLiteral("BLUFOR_SUPPORT");
	case MarkerType::BLUFOR_UAV: return QStringLiteral("BLUFOR_UAV");
	case MarkerType::BLUFOR_UNKNOWN: return QStringLiteral("BLUFOR_UNKNOWN");
	case MarkerType::CIVILIAN_CAR: return QStringLiteral("CIVILIAN_CAR");
	case MarkerType::CIVILIAN_HELICOPTER: return QStringLiteral("CIVILIAN_HELICOPTER");
	case MarkerType::CIVILIAN_PLANE: return QStringLiteral("CIVILIAN_PLANE");
	case MarkerType::CIVILIAN_SHIP: return QStringLiteral("CIVILIAN_SHIP");
	case MarkerType::CIVILIAN_UNKNOWN: return QStringLiteral("CIVILIAN_UNKNOWN");
	case MarkerType::INDEPENDENT_ANTIAIR: return QStringLiteral("INDEPENDENT_ANTIAIR");
	case MarkerType::INDEPENDENT_ARMOR: return QStringLiteral("INDEPENDENT_ARMOR");
	case MarkerType::INDEPENDENT_ARTILLERY: return QStringLiteral("INDEPENDENT_ARTILLERY");
	case MarkerType::INDEPENDENT_HELICOPTER: return QStringLiteral("INDEPENDENT_HELICOPTER");
	case MarkerType::INDEPENDENT_HQ: return QStringLiteral("INDEPENDENT_HQ");
	case MarkerType::INDEPENDENT_INFANTRY: return QStringLiteral("INDEPENDENT_INFANTRY");
	case MarkerType::INDEPENDENT_INSTALLATION: return QStringLiteral("INDEPENDENT_INSTALLATION");
	case MarkerType::INDEPENDENT_MAINTENANCE: return QStringLiteral("INDEPENDENT_MAINTENANCE");
	case MarkerType::INDEPENDENT_MECHANIZED: return QStringLiteral("INDEPENDENT_MECHANIZED");
	case MarkerType::INDEPENDENT_MEDICAL: return QStringLiteral("INDEPENDENT_MEDICAL");
	case MarkerType::INDEPENDENT_MORTAR: return QStringLiteral("INDEPENDENT_MORTAR");
	case MarkerType::INDEPENDENT_MOTORIZED: return QStringLiteral("INDEPENDENT_MOTORIZED");
	case MarkerType::INDEPENDENT_NAVAL: return QStringLiteral("INDEPENDENT_NAVAL");
	case MarkerType::INDEPENDENT_ORDNANCE: return QStringLiteral("INDEPENDENT_ORDNANCE");
	case MarkerType::INDEPENDENT_PLANE: return QStringLiteral("INDEPENDENT_PLANE");
	case MarkerType::INDEPENDENT_RECON: return QStringLiteral("INDEPENDENT_RECON");
	case MarkerType::INDEPENDENT_SERVICE: return QStringLiteral("INDEPENDENT_SERVICE");
	case MarkerType::INDEPENDENT_SUPPORT: return QStringLiteral("INDEPENDENT_SUPPORT");
	case MarkerType::INDEPENDENT_UAV: return QStringLiteral("INDEPENDENT_UAV");
	case MarkerType::INDEPENDENT_UNKNOWN: return QStringLiteral("INDEPENDENT_UNKNOWN");
	case MarkerType::OPFOR_ANTIAIR: return QStringLiteral("OPFOR_ANTIAIR");
	case MarkerType::OPFOR_ARMOR: return QStringLiteral("OPFOR_ARMOR");
	case MarkerType::OPFOR_ARTILLERY: return QStringLiteral("OPFOR_ARTILLERY");
	case MarkerType::OPFOR_HELICOPTER: return QStringLiteral("OPFOR_HELICOPTER");
	case MarkerType::OPFOR_HQ: return QStringLiteral("OPFOR_HQ");
	case MarkerType::OPFOR_INFANTRY: return QStringLiteral("OPFOR_INFANTRY");
	case MarkerType::OPFOR_INSTALLATION: return QStringLiteral("OPFOR_INSTALLATION");
	case MarkerType::OPFOR_MAINTENANCE: return QStringLiteral("OPFOR_MAINTENANCE");
	case MarkerType::OPFOR_MECHANIZED: return QStringLiteral("OPFOR_MECHANIZED");
	case MarkerType::OPFOR_MEDICAL: return QStringLiteral("OPFOR_MEDICAL");
	case MarkerType::OPFOR_MORTAR: return QStringLiteral("OPFOR_MORTAR");
	case MarkerType::OPFOR_MOTORIZED: return QStringLiteral("OPFOR_MOTORIZED");
	case MarkerType::OPFOR_NAVAL: return QStringLiteral("OPFOR_NAVAL");
	case MarkerType::OPFOR_ORDNANCE: return QStringLiteral("OPFOR_ORDNANCE");
	case MarkerType::OPFOR_PLANE: return QStringLiteral("OPFOR_PLANE");
	case MarkerType::OPFOR_RECON: return QStringLiteral("OPFOR_RECON");
	case MarkerType::OPFOR_SERVICE: return QStringLiteral("OPFOR_SERVICE");
	case MarkerType::OPFOR_SUPPORT: return QStringLiteral("OPFOR_SUPPORT");
	case MarkerType::OPFOR_UAV: return QStringLiteral("OPFOR_UAV");
	case MarkerType::OPFOR_UNKNOWN: return QStringLiteral("OPFOR_UNKNOWN");
	case MarkerType::INVALID:
	default: return QStringLiteral("INVALID");
	}
}

QString MarkerTypeHelper::toDisplayString(MarkerType const& colorName) {
	switch (colorName) {
	case MarkerType::BLUFOR_ANTIAIR: return QStringLiteral("BLUFOR Anti Air");
	case MarkerType::BLUFOR_ARMOR: return QStringLiteral("BLUFOR Armor");
	case MarkerType::BLUFOR_ARTILLERY: return QStringLiteral("BLUFOR Artillery");
	case MarkerType::BLUFOR_HELICOPTER: return QStringLiteral("BLUFOR Helicopter");
	case MarkerType::BLUFOR_HQ: return QStringLiteral("BLUFOR HQ");
	case MarkerType::BLUFOR_INFANTRY: return QStringLiteral("BLUFOR Infantry");
	case MarkerType::BLUFOR_INSTALLATION: return QStringLiteral("BLUFOR Installation");
	case MarkerType::BLUFOR_MAINTENANCE: return QStringLiteral("BLUFOR Maintenance");
	case MarkerType::BLUFOR_MECHANIZED: return QStringLiteral("BLUFOR Mechanized");
	case MarkerType::BLUFOR_MEDICAL: return QStringLiteral("BLUFOR Medical");
	case MarkerType::BLUFOR_MORTAR: return QStringLiteral("BLUFOR Mortar");
	case MarkerType::BLUFOR_MOTORIZED: return QStringLiteral("BLUFOR Motorized");
	case MarkerType::BLUFOR_NAVAL: return QStringLiteral("BLUFOR Naval");
	case MarkerType::BLUFOR_ORDNANCE: return QStringLiteral("BLUFOR Ordnance");
	case MarkerType::BLUFOR_PLANE: return QStringLiteral("BLUFOR Plane");
	case MarkerType::BLUFOR_RECON: return QStringLiteral("BLUFOR Recon");
	case MarkerType::BLUFOR_SERVICE: return QStringLiteral("BLUFOR Service");
	case MarkerType::BLUFOR_SUPPORT: return QStringLiteral("BLUFOR Support");
	case MarkerType::BLUFOR_UAV: return QStringLiteral("BLUFOR UAV");
	case MarkerType::BLUFOR_UNKNOWN: return QStringLiteral("BLUFOR Unknown");
	case MarkerType::CIVILIAN_CAR: return QStringLiteral("Civilian Car");
	case MarkerType::CIVILIAN_HELICOPTER: return QStringLiteral("Civilian Helicopter");
	case MarkerType::CIVILIAN_PLANE: return QStringLiteral("Civilian Plane");
	case MarkerType::CIVILIAN_SHIP: return QStringLiteral("Civilian Ship");
	case MarkerType::CIVILIAN_UNKNOWN: return QStringLiteral("Civilian Unknown");
	case MarkerType::INDEPENDENT_ANTIAIR: return QStringLiteral("Independent Anti Air");
	case MarkerType::INDEPENDENT_ARMOR: return QStringLiteral("Independent Armor");
	case MarkerType::INDEPENDENT_ARTILLERY: return QStringLiteral("Independent Artillery");
	case MarkerType::INDEPENDENT_HELICOPTER: return QStringLiteral("Independent Helicopter");
	case MarkerType::INDEPENDENT_HQ: return QStringLiteral("Independent HQ");
	case MarkerType::INDEPENDENT_INFANTRY: return QStringLiteral("Independent Infantry");
	case MarkerType::INDEPENDENT_INSTALLATION: return QStringLiteral("Independent Installation");
	case MarkerType::INDEPENDENT_MAINTENANCE: return QStringLiteral("Independent Maintenance");
	case MarkerType::INDEPENDENT_MECHANIZED: return QStringLiteral("Independent Mechanized");
	case MarkerType::INDEPENDENT_MEDICAL: return QStringLiteral("Independent Medical");
	case MarkerType::INDEPENDENT_MORTAR: return QStringLiteral("Independent Mortar");
	case MarkerType::INDEPENDENT_MOTORIZED: return QStringLiteral("Independent Motorized");
	case MarkerType::INDEPENDENT_NAVAL: return QStringLiteral("Independent Naval");
	case MarkerType::INDEPENDENT_ORDNANCE: return QStringLiteral("Independent Ordnance");
	case MarkerType::INDEPENDENT_PLANE: return QStringLiteral("Independent Plane");
	case MarkerType::INDEPENDENT_RECON: return QStringLiteral("Independent Recon");
	case MarkerType::INDEPENDENT_SERVICE: return QStringLiteral("Independent Service");
	case MarkerType::INDEPENDENT_SUPPORT: return QStringLiteral("Independent Support");
	case MarkerType::INDEPENDENT_UAV: return QStringLiteral("Independent UAV");
	case MarkerType::INDEPENDENT_UNKNOWN: return QStringLiteral("Independent Unknown");
	case MarkerType::OPFOR_ANTIAIR: return QStringLiteral("OPFOR Anti Air");
	case MarkerType::OPFOR_ARMOR: return QStringLiteral("OPFOR Armor");
	case MarkerType::OPFOR_ARTILLERY: return QStringLiteral("OPFOR Artillery");
	case MarkerType::OPFOR_HELICOPTER: return QStringLiteral("OPFOR Helicopter");
	case MarkerType::OPFOR_HQ: return QStringLiteral("OPFOR HQ");
	case MarkerType::OPFOR_INFANTRY: return QStringLiteral("OPFOR Infantry");
	case MarkerType::OPFOR_INSTALLATION: return QStringLiteral("OPFOR Installation");
	case MarkerType::OPFOR_MAINTENANCE: return QStringLiteral("OPFOR Maintenance");
	case MarkerType::OPFOR_MECHANIZED: return QStringLiteral("OPFOR Mechanized");
	case MarkerType::OPFOR_MEDICAL: return QStringLiteral("OPFOR Medical");
	case MarkerType::OPFOR_MORTAR: return QStringLiteral("OPFOR Mortar");
	case MarkerType::OPFOR_MOTORIZED: return QStringLiteral("OPFOR Motorized");
	case MarkerType::OPFOR_NAVAL: return QStringLiteral("OPFOR Naval");
	case MarkerType::OPFOR_ORDNANCE: return QStringLiteral("OPFOR Ordnance");
	case MarkerType::OPFOR_PLANE: return QStringLiteral("OPFOR Plane");
	case MarkerType::OPFOR_RECON: return QStringLiteral("OPFOR Recon");
	case MarkerType::OPFOR_SERVICE: return QStringLiteral("OPFOR Service");
	case MarkerType::OPFOR_SUPPORT: return QStringLiteral("OPFOR Support");
	case MarkerType::OPFOR_UAV: return QStringLiteral("OPFOR UAV");
	case MarkerType::OPFOR_UNKNOWN: return QStringLiteral("OPFOR Unknown");
	case MarkerType::INVALID:
	default: return QStringLiteral("INVALID");
	}
}

MarkerType MarkerTypeHelper::fromString(QString const& markerType) {
	if ((markerType.compare(QStringLiteral("BLUFOR_ANTIAIR"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Anti Air"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_ANTIAIR;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_ARMOR"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Armor"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_ARMOR;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_ARTILLERY"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Artillery"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_ARTILLERY;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_HELICOPTER"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Helicopter"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_HELICOPTER;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_HQ"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR HQ"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_HQ;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_INFANTRY"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Infantry"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_INFANTRY;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_INSTALLATION"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Installation"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_INSTALLATION;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_MAINTENANCE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Maintenance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_MAINTENANCE;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_MECHANIZED"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Mechanized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_MECHANIZED;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_MEDICAL"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Medical"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_MEDICAL;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_MORTAR"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Mortar"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_MORTAR;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_MOTORIZED"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Motorized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_MOTORIZED;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_NAVAL"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Naval"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_NAVAL;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_ORDNANCE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Ordnance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_ORDNANCE;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_PLANE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Plane"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_PLANE;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_RECON"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Recon"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_RECON;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_SERVICE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Service"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_SERVICE;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_SUPPORT"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Support"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_SUPPORT;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_UAV"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR UAV"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_UAV;
	} else if ((markerType.compare(QStringLiteral("BLUFOR_UNKNOWN"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Unknown"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::BLUFOR_UNKNOWN;
	} else if ((markerType.compare(QStringLiteral("CIVILIAN_CAR"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Civilian Car"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::CIVILIAN_CAR;
	} else if ((markerType.compare(QStringLiteral("CIVILIAN_HELICOPTER"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Civilian Helicopter"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::CIVILIAN_HELICOPTER;
	} else if ((markerType.compare(QStringLiteral("CIVILIAN_PLANE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Civilian Plane"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::CIVILIAN_PLANE;
	} else if ((markerType.compare(QStringLiteral("CIVILIAN_SHIP"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Civilian Ship"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::CIVILIAN_SHIP;
	} else if ((markerType.compare(QStringLiteral("CIVILIAN_UNKNOWN"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Civilian Unknown"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::CIVILIAN_UNKNOWN;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_ANTIAIR"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Anti Air"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_ANTIAIR;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_ARMOR"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Armor"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_ARMOR;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_ARTILLERY"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Artillery"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_ARTILLERY;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_HELICOPTER"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Helicopter"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_HELICOPTER;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_HQ"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent HQ"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_HQ;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_INFANTRY"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Infantry"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_INFANTRY;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_INSTALLATION"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Installation"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_INSTALLATION;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_MAINTENANCE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Maintenance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_MAINTENANCE;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_MECHANIZED"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Mechanized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_MECHANIZED;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_MEDICAL"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Medical"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_MEDICAL;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_MORTAR"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Mortar"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_MORTAR;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_MOTORIZED"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Motorized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_MOTORIZED;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_NAVAL"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Naval"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_NAVAL;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_ORDNANCE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Ordnance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_ORDNANCE;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_PLANE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Plane"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_PLANE;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_RECON"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Recon"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_RECON;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_SERVICE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Service"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_SERVICE;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_SUPPORT"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Support"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_SUPPORT;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_UAV"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent UAV"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_UAV;
	} else if ((markerType.compare(QStringLiteral("INDEPENDENT_UNKNOWN"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Unknown"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::INDEPENDENT_UNKNOWN;
	} else if ((markerType.compare(QStringLiteral("OPFOR_ANTIAIR"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Anti Air"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_ANTIAIR;
	} else if ((markerType.compare(QStringLiteral("OPFOR_ARMOR"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Armor"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_ARMOR;
	} else if ((markerType.compare(QStringLiteral("OPFOR_ARTILLERY"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Artillery"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_ARTILLERY;
	} else if ((markerType.compare(QStringLiteral("OPFOR_HELICOPTER"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Helicopter"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_HELICOPTER;
	} else if ((markerType.compare(QStringLiteral("OPFOR_HQ"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR HQ"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_HQ;
	} else if ((markerType.compare(QStringLiteral("OPFOR_INFANTRY"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Infantry"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_INFANTRY;
	} else if ((markerType.compare(QStringLiteral("OPFOR_INSTALLATION"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Installation"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_INSTALLATION;
	} else if ((markerType.compare(QStringLiteral("OPFOR_MAINTENANCE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Maintenance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_MAINTENANCE;
	} else if ((markerType.compare(QStringLiteral("OPFOR_MECHANIZED"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Mechanized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_MECHANIZED;
	} else if ((markerType.compare(QStringLiteral("OPFOR_MEDICAL"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Medical"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_MEDICAL;
	} else if ((markerType.compare(QStringLiteral("OPFOR_MORTAR"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Mortar"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_MORTAR;
	} else if ((markerType.compare(QStringLiteral("OPFOR_MOTORIZED"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Motorized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_MOTORIZED;
	} else if ((markerType.compare(QStringLiteral("OPFOR_NAVAL"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Naval"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_NAVAL;
	} else if ((markerType.compare(QStringLiteral("OPFOR_ORDNANCE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Ordnance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_ORDNANCE;
	} else if ((markerType.compare(QStringLiteral("OPFOR_PLANE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Plane"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_PLANE;
	} else if ((markerType.compare(QStringLiteral("OPFOR_RECON"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Recon"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_RECON;
	} else if ((markerType.compare(QStringLiteral("OPFOR_SERVICE"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Service"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_SERVICE;
	} else if ((markerType.compare(QStringLiteral("OPFOR_SUPPORT"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Support"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_SUPPORT;
	} else if ((markerType.compare(QStringLiteral("OPFOR_UAV"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR UAV"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_UAV;
	} else if ((markerType.compare(QStringLiteral("OPFOR_UNKNOWN"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Unknown"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::OPFOR_UNKNOWN;
	} else {
		return MarkerType::INVALID;
	}
}

MarkerTypeHelper::Side MarkerTypeHelper::getSide(MarkerType const& markerType) {
	switch (markerType) {
	case MarkerType::BLUFOR_ANTIAIR:
	case MarkerType::BLUFOR_ARMOR:
	case MarkerType::BLUFOR_ARTILLERY:
	case MarkerType::BLUFOR_HELICOPTER:
	case MarkerType::BLUFOR_HQ:
	case MarkerType::BLUFOR_INFANTRY:
	case MarkerType::BLUFOR_INSTALLATION:
	case MarkerType::BLUFOR_MAINTENANCE:
	case MarkerType::BLUFOR_MECHANIZED:
	case MarkerType::BLUFOR_MEDICAL:
	case MarkerType::BLUFOR_MORTAR:
	case MarkerType::BLUFOR_MOTORIZED:
	case MarkerType::BLUFOR_NAVAL:
	case MarkerType::BLUFOR_ORDNANCE:
	case MarkerType::BLUFOR_PLANE:
	case MarkerType::BLUFOR_RECON:
	case MarkerType::BLUFOR_SERVICE:
	case MarkerType::BLUFOR_SUPPORT:
	case MarkerType::BLUFOR_UAV:
	case MarkerType::BLUFOR_UNKNOWN:
		return Side::BLUFOR;
	case MarkerType::CIVILIAN_CAR:
	case MarkerType::CIVILIAN_HELICOPTER:
	case MarkerType::CIVILIAN_PLANE:
	case MarkerType::CIVILIAN_SHIP:
	case MarkerType::CIVILIAN_UNKNOWN:
		return Side::CIVILIAN;
	case MarkerType::INDEPENDENT_ANTIAIR:
	case MarkerType::INDEPENDENT_ARMOR:
	case MarkerType::INDEPENDENT_ARTILLERY:
	case MarkerType::INDEPENDENT_HELICOPTER:
	case MarkerType::INDEPENDENT_HQ:
	case MarkerType::INDEPENDENT_INFANTRY:
	case MarkerType::INDEPENDENT_INSTALLATION:
	case MarkerType::INDEPENDENT_MAINTENANCE:
	case MarkerType::INDEPENDENT_MECHANIZED:
	case MarkerType::INDEPENDENT_MEDICAL:
	case MarkerType::INDEPENDENT_MORTAR:
	case MarkerType::INDEPENDENT_MOTORIZED:
	case MarkerType::INDEPENDENT_NAVAL:
	case MarkerType::INDEPENDENT_ORDNANCE:
	case MarkerType::INDEPENDENT_PLANE:
	case MarkerType::INDEPENDENT_RECON:
	case MarkerType::INDEPENDENT_SERVICE:
	case MarkerType::INDEPENDENT_SUPPORT:
	case MarkerType::INDEPENDENT_UAV:
	case MarkerType::INDEPENDENT_UNKNOWN:
		return Side::INDEPENDENT;
	case MarkerType::OPFOR_ANTIAIR:
	case MarkerType::OPFOR_ARMOR:
	case MarkerType::OPFOR_ARTILLERY:
	case MarkerType::OPFOR_HELICOPTER:
	case MarkerType::OPFOR_HQ:
	case MarkerType::OPFOR_INFANTRY:
	case MarkerType::OPFOR_INSTALLATION:
	case MarkerType::OPFOR_MAINTENANCE:
	case MarkerType::OPFOR_MECHANIZED:
	case MarkerType::OPFOR_MEDICAL:
	case MarkerType::OPFOR_MORTAR:
	case MarkerType::OPFOR_MOTORIZED:
	case MarkerType::OPFOR_NAVAL:
	case MarkerType::OPFOR_ORDNANCE:
	case MarkerType::OPFOR_PLANE:
	case MarkerType::OPFOR_RECON:
	case MarkerType::OPFOR_SERVICE:
	case MarkerType::OPFOR_SUPPORT:
	case MarkerType::OPFOR_UAV:
	case MarkerType::OPFOR_UNKNOWN:
		return Side::OPFOR;
	default:
		return Side::INVALID;
	}
}

