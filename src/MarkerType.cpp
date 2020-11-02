#include "MarkerType.h"

QString MarkerTypeHelper::toString(MarkerType const& fillName) {
	switch (fillName) {
	case MarkerType::b_antiair: return QStringLiteral("b_antiair");
	case MarkerType::b_armor: return QStringLiteral("b_armor");
	case MarkerType::b_art: return QStringLiteral("b_art");
	case MarkerType::b_air: return QStringLiteral("b_air");
	case MarkerType::b_hq: return QStringLiteral("b_hq");
	case MarkerType::b_inf: return QStringLiteral("b_inf");
	case MarkerType::b_installation: return QStringLiteral("b_installation");
	case MarkerType::b_maint: return QStringLiteral("b_maint");
	case MarkerType::b_mech_inf: return QStringLiteral("b_mech_inf");
	case MarkerType::b_med: return QStringLiteral("b_med");
	case MarkerType::b_mortar: return QStringLiteral("b_mortar");
	case MarkerType::b_motor_inf: return QStringLiteral("b_motor_inf");
	case MarkerType::b_naval: return QStringLiteral("b_naval");
	case MarkerType::b_ordnance: return QStringLiteral("b_ordnance");
	case MarkerType::b_plane: return QStringLiteral("b_plane");
	case MarkerType::b_recon: return QStringLiteral("b_recon");
	case MarkerType::b_service: return QStringLiteral("b_service");
	case MarkerType::b_support: return QStringLiteral("b_support");
	case MarkerType::b_uav: return QStringLiteral("b_uav");
	case MarkerType::b_unknown: return QStringLiteral("b_unknown");
	case MarkerType::c_car: return QStringLiteral("c_car");
	case MarkerType::c_air: return QStringLiteral("c_air");
	case MarkerType::c_plane: return QStringLiteral("c_plane");
	case MarkerType::c_ship: return QStringLiteral("c_ship");
	case MarkerType::c_unknown: return QStringLiteral("c_unknown");
	case MarkerType::n_antiair: return QStringLiteral("n_antiair");
	case MarkerType::n_armor: return QStringLiteral("n_armor");
	case MarkerType::n_art: return QStringLiteral("n_art");
	case MarkerType::n_air: return QStringLiteral("n_air");
	case MarkerType::n_hq: return QStringLiteral("n_hq");
	case MarkerType::n_inf: return QStringLiteral("n_inf");
	case MarkerType::n_installation: return QStringLiteral("n_installation");
	case MarkerType::n_maint: return QStringLiteral("n_maint");
	case MarkerType::n_mech_inf: return QStringLiteral("n_mech_inf");
	case MarkerType::n_med: return QStringLiteral("n_med");
	case MarkerType::n_mortar: return QStringLiteral("n_mortar");
	case MarkerType::n_motor_inf: return QStringLiteral("n_motor_inf");
	case MarkerType::n_naval: return QStringLiteral("n_naval");
	case MarkerType::n_ordnance: return QStringLiteral("n_ordnance");
	case MarkerType::n_plane: return QStringLiteral("n_plane");
	case MarkerType::n_recon: return QStringLiteral("n_recon");
	case MarkerType::n_service: return QStringLiteral("n_service");
	case MarkerType::n_support: return QStringLiteral("n_support");
	case MarkerType::n_uav: return QStringLiteral("n_uav");
	case MarkerType::n_unknown: return QStringLiteral("n_unknown");
	case MarkerType::o_antiair: return QStringLiteral("o_antiair");
	case MarkerType::o_armor: return QStringLiteral("o_armor");
	case MarkerType::o_art: return QStringLiteral("o_art");
	case MarkerType::o_air: return QStringLiteral("o_air");
	case MarkerType::o_hq: return QStringLiteral("o_hq");
	case MarkerType::o_inf: return QStringLiteral("o_inf");
	case MarkerType::o_installation: return QStringLiteral("o_installation");
	case MarkerType::o_maint: return QStringLiteral("o_maint");
	case MarkerType::o_mech_inf: return QStringLiteral("o_mech_inf");
	case MarkerType::o_med: return QStringLiteral("o_med");
	case MarkerType::o_mortar: return QStringLiteral("o_mortar");
	case MarkerType::o_motor_inf: return QStringLiteral("o_motor_inf");
	case MarkerType::o_naval: return QStringLiteral("o_naval");
	case MarkerType::o_ordnance: return QStringLiteral("o_ordnance");
	case MarkerType::o_plane: return QStringLiteral("o_plane");
	case MarkerType::o_recon: return QStringLiteral("o_recon");
	case MarkerType::o_service: return QStringLiteral("o_service");
	case MarkerType::o_support: return QStringLiteral("o_support");
	case MarkerType::o_uav: return QStringLiteral("o_uav");
	case MarkerType::o_unknown: return QStringLiteral("o_unknown");
	case MarkerType::INVALID:
	default: return QStringLiteral("INVALID");
	}
}

QString MarkerTypeHelper::toDisplayString(MarkerType const& colorName) {
	switch (colorName) {
	case MarkerType::b_antiair: return QStringLiteral("BLUFOR Anti Air");
	case MarkerType::b_armor: return QStringLiteral("BLUFOR Armor");
	case MarkerType::b_art: return QStringLiteral("BLUFOR Artillery");
	case MarkerType::b_air: return QStringLiteral("BLUFOR Helicopter");
	case MarkerType::b_hq: return QStringLiteral("BLUFOR HQ");
	case MarkerType::b_inf: return QStringLiteral("BLUFOR Infantry");
	case MarkerType::b_installation: return QStringLiteral("BLUFOR Installation");
	case MarkerType::b_maint: return QStringLiteral("BLUFOR Maintenance");
	case MarkerType::b_mech_inf: return QStringLiteral("BLUFOR Mechanized");
	case MarkerType::b_med: return QStringLiteral("BLUFOR Medical");
	case MarkerType::b_mortar: return QStringLiteral("BLUFOR Mortar");
	case MarkerType::b_motor_inf: return QStringLiteral("BLUFOR Motorized");
	case MarkerType::b_naval: return QStringLiteral("BLUFOR Naval");
	case MarkerType::b_ordnance: return QStringLiteral("BLUFOR Ordnance");
	case MarkerType::b_plane: return QStringLiteral("BLUFOR Plane");
	case MarkerType::b_recon: return QStringLiteral("BLUFOR Recon");
	case MarkerType::b_service: return QStringLiteral("BLUFOR Service");
	case MarkerType::b_support: return QStringLiteral("BLUFOR Support");
	case MarkerType::b_uav: return QStringLiteral("BLUFOR UAV");
	case MarkerType::b_unknown: return QStringLiteral("BLUFOR Unknown");
	case MarkerType::c_car: return QStringLiteral("Civilian Car");
	case MarkerType::c_air: return QStringLiteral("Civilian Helicopter");
	case MarkerType::c_plane: return QStringLiteral("Civilian Plane");
	case MarkerType::c_ship: return QStringLiteral("Civilian Ship");
	case MarkerType::c_unknown: return QStringLiteral("Civilian Unknown");
	case MarkerType::n_antiair: return QStringLiteral("Independent Anti Air");
	case MarkerType::n_armor: return QStringLiteral("Independent Armor");
	case MarkerType::n_art: return QStringLiteral("Independent Artillery");
	case MarkerType::n_air: return QStringLiteral("Independent Helicopter");
	case MarkerType::n_hq: return QStringLiteral("Independent HQ");
	case MarkerType::n_inf: return QStringLiteral("Independent Infantry");
	case MarkerType::n_installation: return QStringLiteral("Independent Installation");
	case MarkerType::n_maint: return QStringLiteral("Independent Maintenance");
	case MarkerType::n_mech_inf: return QStringLiteral("Independent Mechanized");
	case MarkerType::n_med: return QStringLiteral("Independent Medical");
	case MarkerType::n_mortar: return QStringLiteral("Independent Mortar");
	case MarkerType::n_motor_inf: return QStringLiteral("Independent Motorized");
	case MarkerType::n_naval: return QStringLiteral("Independent Naval");
	case MarkerType::n_ordnance: return QStringLiteral("Independent Ordnance");
	case MarkerType::n_plane: return QStringLiteral("Independent Plane");
	case MarkerType::n_recon: return QStringLiteral("Independent Recon");
	case MarkerType::n_service: return QStringLiteral("Independent Service");
	case MarkerType::n_support: return QStringLiteral("Independent Support");
	case MarkerType::n_uav: return QStringLiteral("Independent UAV");
	case MarkerType::n_unknown: return QStringLiteral("Independent Unknown");
	case MarkerType::o_antiair: return QStringLiteral("OPFOR Anti Air");
	case MarkerType::o_armor: return QStringLiteral("OPFOR Armor");
	case MarkerType::o_art: return QStringLiteral("OPFOR Artillery");
	case MarkerType::o_air: return QStringLiteral("OPFOR Helicopter");
	case MarkerType::o_hq: return QStringLiteral("OPFOR HQ");
	case MarkerType::o_inf: return QStringLiteral("OPFOR Infantry");
	case MarkerType::o_installation: return QStringLiteral("OPFOR Installation");
	case MarkerType::o_maint: return QStringLiteral("OPFOR Maintenance");
	case MarkerType::o_mech_inf: return QStringLiteral("OPFOR Mechanized");
	case MarkerType::o_med: return QStringLiteral("OPFOR Medical");
	case MarkerType::o_mortar: return QStringLiteral("OPFOR Mortar");
	case MarkerType::o_motor_inf: return QStringLiteral("OPFOR Motorized");
	case MarkerType::o_naval: return QStringLiteral("OPFOR Naval");
	case MarkerType::o_ordnance: return QStringLiteral("OPFOR Ordnance");
	case MarkerType::o_plane: return QStringLiteral("OPFOR Plane");
	case MarkerType::o_recon: return QStringLiteral("OPFOR Recon");
	case MarkerType::o_service: return QStringLiteral("OPFOR Service");
	case MarkerType::o_support: return QStringLiteral("OPFOR Support");
	case MarkerType::o_uav: return QStringLiteral("OPFOR UAV");
	case MarkerType::o_unknown: return QStringLiteral("OPFOR Unknown");
	case MarkerType::INVALID:
	default: return QStringLiteral("INVALID");
	}
}

MarkerType MarkerTypeHelper::fromString(QString const& markerType) {
	if ((markerType.compare(QStringLiteral("b_antiair"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Anti Air"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_antiair;
	} else if ((markerType.compare(QStringLiteral("b_armor"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Armor"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_armor;
	} else if ((markerType.compare(QStringLiteral("b_art"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Artillery"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_art;
	} else if ((markerType.compare(QStringLiteral("b_air"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Helicopter"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_air;
	} else if ((markerType.compare(QStringLiteral("b_hq"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR HQ"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_hq;
	} else if ((markerType.compare(QStringLiteral("b_inf"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Infantry"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_inf;
	} else if ((markerType.compare(QStringLiteral("b_installation"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Installation"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_installation;
	} else if ((markerType.compare(QStringLiteral("b_maint"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Maintenance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_maint;
	} else if ((markerType.compare(QStringLiteral("b_mech_inf"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Mechanized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_mech_inf;
	} else if ((markerType.compare(QStringLiteral("b_med"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Medical"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_med;
	} else if ((markerType.compare(QStringLiteral("b_mortar"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Mortar"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_mortar;
	} else if ((markerType.compare(QStringLiteral("b_motor_inf"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Motorized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_motor_inf;
	} else if ((markerType.compare(QStringLiteral("b_naval"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Naval"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_naval;
	} else if ((markerType.compare(QStringLiteral("b_ordnance"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Ordnance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_ordnance;
	} else if ((markerType.compare(QStringLiteral("b_plane"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Plane"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_plane;
	} else if ((markerType.compare(QStringLiteral("b_recon"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Recon"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_recon;
	} else if ((markerType.compare(QStringLiteral("b_service"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Service"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_service;
	} else if ((markerType.compare(QStringLiteral("b_support"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Support"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_support;
	} else if ((markerType.compare(QStringLiteral("b_uav"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR UAV"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_uav;
	} else if ((markerType.compare(QStringLiteral("b_unknown"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("BLUFOR Unknown"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::b_unknown;
	} else if ((markerType.compare(QStringLiteral("c_car"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Civilian Car"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::c_car;
	} else if ((markerType.compare(QStringLiteral("c_air"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Civilian Helicopter"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::c_air;
	} else if ((markerType.compare(QStringLiteral("c_plane"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Civilian Plane"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::c_plane;
	} else if ((markerType.compare(QStringLiteral("c_ship"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Civilian Ship"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::c_ship;
	} else if ((markerType.compare(QStringLiteral("c_unknown"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Civilian Unknown"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::c_unknown;
	} else if ((markerType.compare(QStringLiteral("n_antiair"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Anti Air"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_antiair;
	} else if ((markerType.compare(QStringLiteral("n_armor"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Armor"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_armor;
	} else if ((markerType.compare(QStringLiteral("n_art"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Artillery"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_art;
	} else if ((markerType.compare(QStringLiteral("n_air"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Helicopter"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_air;
	} else if ((markerType.compare(QStringLiteral("n_hq"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent HQ"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_hq;
	} else if ((markerType.compare(QStringLiteral("n_inf"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Infantry"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_inf;
	} else if ((markerType.compare(QStringLiteral("n_installation"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Installation"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_installation;
	} else if ((markerType.compare(QStringLiteral("n_maint"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Maintenance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_maint;
	} else if ((markerType.compare(QStringLiteral("n_mech_inf"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Mechanized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_mech_inf;
	} else if ((markerType.compare(QStringLiteral("n_med"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Medical"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_med;
	} else if ((markerType.compare(QStringLiteral("n_mortar"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Mortar"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_mortar;
	} else if ((markerType.compare(QStringLiteral("n_motor_inf"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Motorized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_motor_inf;
	} else if ((markerType.compare(QStringLiteral("n_naval"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Naval"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_naval;
	} else if ((markerType.compare(QStringLiteral("n_ordnance"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Ordnance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_ordnance;
	} else if ((markerType.compare(QStringLiteral("n_plane"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Plane"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_plane;
	} else if ((markerType.compare(QStringLiteral("n_recon"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Recon"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_recon;
	} else if ((markerType.compare(QStringLiteral("n_service"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Service"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_service;
	} else if ((markerType.compare(QStringLiteral("n_support"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Support"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_support;
	} else if ((markerType.compare(QStringLiteral("n_uav"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent UAV"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_uav;
	} else if ((markerType.compare(QStringLiteral("n_unknown"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("Independent Unknown"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::n_unknown;
	} else if ((markerType.compare(QStringLiteral("o_antiair"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Anti Air"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_antiair;
	} else if ((markerType.compare(QStringLiteral("o_armor"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Armor"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_armor;
	} else if ((markerType.compare(QStringLiteral("o_art"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Artillery"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_art;
	} else if ((markerType.compare(QStringLiteral("o_air"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Helicopter"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_air;
	} else if ((markerType.compare(QStringLiteral("o_hq"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR HQ"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_hq;
	} else if ((markerType.compare(QStringLiteral("o_inf"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Infantry"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_inf;
	} else if ((markerType.compare(QStringLiteral("o_installation"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Installation"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_installation;
	} else if ((markerType.compare(QStringLiteral("o_maint"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Maintenance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_maint;
	} else if ((markerType.compare(QStringLiteral("o_mech_inf"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Mechanized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_mech_inf;
	} else if ((markerType.compare(QStringLiteral("o_med"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Medical"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_med;
	} else if ((markerType.compare(QStringLiteral("o_mortar"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Mortar"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_mortar;
	} else if ((markerType.compare(QStringLiteral("o_motor_inf"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Motorized"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_motor_inf;
	} else if ((markerType.compare(QStringLiteral("o_naval"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Naval"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_naval;
	} else if ((markerType.compare(QStringLiteral("o_ordnance"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Ordnance"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_ordnance;
	} else if ((markerType.compare(QStringLiteral("o_plane"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Plane"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_plane;
	} else if ((markerType.compare(QStringLiteral("o_recon"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Recon"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_recon;
	} else if ((markerType.compare(QStringLiteral("o_service"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Service"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_service;
	} else if ((markerType.compare(QStringLiteral("o_support"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Support"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_support;
	} else if ((markerType.compare(QStringLiteral("o_uav"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR UAV"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_uav;
	} else if ((markerType.compare(QStringLiteral("o_unknown"), Qt::CaseInsensitive) == 0) || (markerType.compare(QStringLiteral("OPFOR Unknown"), Qt::CaseInsensitive) == 0)) {
		return MarkerType::o_unknown;
	} else {
		return MarkerType::INVALID;
	}
}

MarkerTypeHelper::Side MarkerTypeHelper::getSide(MarkerType const& markerType) {
	switch (markerType) {
	case MarkerType::b_antiair:
	case MarkerType::b_armor:
	case MarkerType::b_art:
	case MarkerType::b_air:
	case MarkerType::b_hq:
	case MarkerType::b_inf:
	case MarkerType::b_installation:
	case MarkerType::b_maint:
	case MarkerType::b_mech_inf:
	case MarkerType::b_med:
	case MarkerType::b_mortar:
	case MarkerType::b_motor_inf:
	case MarkerType::b_naval:
	case MarkerType::b_ordnance:
	case MarkerType::b_plane:
	case MarkerType::b_recon:
	case MarkerType::b_service:
	case MarkerType::b_support:
	case MarkerType::b_uav:
	case MarkerType::b_unknown:
		return Side::BLUFOR;
	case MarkerType::c_car:
	case MarkerType::c_air:
	case MarkerType::c_plane:
	case MarkerType::c_ship:
	case MarkerType::c_unknown:
		return Side::CIVILIAN;
	case MarkerType::n_antiair:
	case MarkerType::n_armor:
	case MarkerType::n_art:
	case MarkerType::n_air:
	case MarkerType::n_hq:
	case MarkerType::n_inf:
	case MarkerType::n_installation:
	case MarkerType::n_maint:
	case MarkerType::n_mech_inf:
	case MarkerType::n_med:
	case MarkerType::n_mortar:
	case MarkerType::n_motor_inf:
	case MarkerType::n_naval:
	case MarkerType::n_ordnance:
	case MarkerType::n_plane:
	case MarkerType::n_recon:
	case MarkerType::n_service:
	case MarkerType::n_support:
	case MarkerType::n_uav:
	case MarkerType::n_unknown:
		return Side::INDEPENDENT;
	case MarkerType::o_antiair:
	case MarkerType::o_armor:
	case MarkerType::o_art:
	case MarkerType::o_air:
	case MarkerType::o_hq:
	case MarkerType::o_inf:
	case MarkerType::o_installation:
	case MarkerType::o_maint:
	case MarkerType::o_mech_inf:
	case MarkerType::o_med:
	case MarkerType::o_mortar:
	case MarkerType::o_motor_inf:
	case MarkerType::o_naval:
	case MarkerType::o_ordnance:
	case MarkerType::o_plane:
	case MarkerType::o_recon:
	case MarkerType::o_service:
	case MarkerType::o_support:
	case MarkerType::o_uav:
	case MarkerType::o_unknown:
		return Side::OPFOR;
	default:
		return Side::INVALID;
	}
}

