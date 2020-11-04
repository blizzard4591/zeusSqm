#ifndef ZEUSSQM_OBJECTBUILDERMODULE_H_
#define ZEUSSQM_OBJECTBUILDERMODULE_H_

#include "CheckModule.h"

#include <QImage>

class ObjectBuilderModule : public CheckModule {
public:
	ObjectBuilderModule();
	virtual ~ObjectBuilderModule() {}

	virtual bool checkArguments(QCommandLineParser& parser) override;
	virtual std::shared_ptr<SqmObjectList<SqmStructure>> perform(std::shared_ptr<SqmObjectList<SqmStructure>> const& sqmObjects) override;
	virtual void registerOptions(QCommandLineParser& parser) override;
private:
	QCommandLineOption buildFromImageOption;
	QCommandLineOption buildMinAlphaValueOption;
	QCommandLineOption buildPlaneOption;
	QCommandLineOption buildStartingPointOption;

	enum class BuildPlane {
		XY, XZ, YZ
	};

	bool buildFromImage;
	int minAlphaValue;
	QImage image;
	BuildPlane buildPlane;
	int startingPosition[3];
};

#endif
