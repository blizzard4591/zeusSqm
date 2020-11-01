#ifndef ZEUSSQM_MARKERCHECK_H_
#define ZEUSSQM_MARKERCHECK_H_

#include <QCommandLineParser>
#include <QString>

#include <memory>

#include "SqmObjectList.h"

class MarkerCheck {
public:
	MarkerCheck(QCommandLineParser& parser);
	virtual ~MarkerCheck() {}

	bool checkArguments(QCommandLineParser& parser);
	std::shared_ptr<SqmObjectList<SqmStructure>> perform(std::shared_ptr<SqmObjectList<SqmStructure>> const& sqmObjects);
private:
	QCommandLineOption checkMarkerOption;
	QCommandLineOption markerMaxDistanceOption;
	QCommandLineOption markerAskConfirmationOption;
	QCommandLineOption markerGridOption;

	bool checkMarkers;
	int markerGrid;
	int markerMaxDistance;
	bool markerAskConfirmation;
};

#endif
