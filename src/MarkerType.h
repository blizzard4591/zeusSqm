#ifndef ZEUSSQM_SQM_MARKERTYPE_H_
#define ZEUSSQM_SQM_MARKERTYPE_H_

#include <QString>

enum class MarkerType {
	INVALID,
	
	b_antiair,
	b_armor,
	b_art,
	b_air,
	b_hq,
	b_inf,
	b_installation,
	b_maint,
	b_mech_inf,
	b_med,
	b_mortar,
	b_motor_inf,
	b_naval,
	b_ordnance,
	b_plane,
	b_recon,
	b_service,
	b_support,
	b_uav,
	b_unknown,
	c_car,
	c_air,
	c_plane,
	c_ship,
	c_unknown,
	n_antiair,
	n_armor,
	n_art,
	n_air,
	n_hq,
	n_inf,
	n_installation,
	n_maint,
	n_mech_inf,
	n_med,
	n_mortar,
	n_motor_inf,
	n_naval,
	n_ordnance,
	n_plane,
	n_recon,
	n_service,
	n_support,
	n_uav,
	n_unknown,
	o_antiair,
	o_armor,
	o_art,
	o_air,
	o_hq,
	o_inf,
	o_installation,
	o_maint,
	o_mech_inf,
	o_med,
	o_mortar,
	o_motor_inf,
	o_naval,
	o_ordnance,
	o_plane,
	o_recon,
	o_service,
	o_support,
	o_uav,
	o_unknown
};

class MarkerTypeHelper {
public:
	static QString toString(MarkerType const& markerType);
	static QString toDisplayString(MarkerType const& markerType);
	static MarkerType fromString(QString const& marker);

	enum class Side {
		INVALID,
		BLUFOR, OPFOR, INDEPENDENT, CIVILIAN
	};
	static Side getSide(MarkerType const& markerType);
};

#endif
