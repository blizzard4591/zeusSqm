#ifndef ZEUSSQM_MARKERCHECKMODULE_H_
#define ZEUSSQM_MARKERCHECKMODULE_H_

#include "CheckModule.h"

class MarkerCheckModule : public CheckModule {
public:
	MarkerCheckModule();
	virtual ~MarkerCheckModule() {}

	virtual bool checkArguments(QCommandLineParser& parser) override;
	virtual std::shared_ptr<SqmObjectList<SqmStructure>> perform(std::shared_ptr<SqmObjectList<SqmStructure>> const& sqmObjects) override;
	virtual void registerOptions(QCommandLineParser& parser) override;
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
