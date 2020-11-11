#ifndef ZEUSSQM_ALLOWEDMODSCHECKMODULE_H_
#define ZEUSSQM_ALLOWEDMODSCHECKMODULE_H_

#include <QSet>

#include "CheckModule.h"

class AllowedModsCheckModule : public CheckModule {
public:
	AllowedModsCheckModule();
	virtual ~AllowedModsCheckModule() {}

	virtual bool checkArguments(QCommandLineParser& parser) override;
	virtual std::shared_ptr<SqmObjectList<SqmStructure>> perform(std::shared_ptr<SqmObjectList<SqmStructure>> const& sqmObjects) override;
	virtual void registerOptions(QCommandLineParser& parser) override;
private:
	QCommandLineOption checkModsOption;
	QCommandLineOption dropUnlistedModsOption;

	bool checkMods;
	QSet<QString> allowedMods;
	bool dropUnlistedMods;
};

#endif
