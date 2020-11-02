#ifndef ZEUSSQM_STATISTICSCHECKMODULE_H_
#define ZEUSSQM_STATISTICSCHECKMODULE_H_

#include "CheckModule.h"

class StatisticsCheckModule : public CheckModule {
public:
	StatisticsCheckModule();
	virtual ~StatisticsCheckModule() {}

	virtual bool checkArguments(QCommandLineParser& parser) override;
	virtual std::shared_ptr<SqmObjectList<SqmStructure>> perform(std::shared_ptr<SqmObjectList<SqmStructure>> const& sqmObjects) override;
	virtual void registerOptions(QCommandLineParser& parser) override;
private:
	QCommandLineOption setBluforRelationOption;
	QCommandLineOption setOpforRelationOption;

	bool changeBluforRelation;
	bool bluforAtWar;
	bool changeOpforRelation;
	bool opforAtWar;
};

#endif
