#ifndef ZEUSSQM_CHECKMODULE_H_
#define ZEUSSQM_CHECKMODULE_H_

#include <QCommandLineParser>
#include <QString>

#include <memory>

#include "SqmObjectList.h"

class CheckModule {
public:
	virtual ~CheckModule() {}

	virtual bool checkArguments(QCommandLineParser& parser) = 0;
	virtual std::shared_ptr<SqmObjectList<SqmStructure>> perform(std::shared_ptr<SqmObjectList<SqmStructure>> const& sqmObjects) = 0;
	virtual void registerOptions(QCommandLineParser& parser) = 0;
};

#endif
