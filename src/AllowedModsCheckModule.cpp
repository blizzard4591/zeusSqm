#include "AllowedModsCheckModule.h"

#include <iostream>
#include <QFile>
#include <QTextStream>

#include <QtGlobal>

#include "SqmArray.h"
#include "SqmHandling.h"
#include "exceptions/FormatErrorException.h"

AllowedModsCheckModule::AllowedModsCheckModule() :
	CheckModule(),
	checkModsOption(QStringList() << "checkMods", QCoreApplication::translate("main", "Check mods list against a whitelist. Tells you if your mission will work with the allowed base set. The whitelist should contain one allowed mod per line."), QCoreApplication::translate("main", "whitelist.txt")),
	dropUnlistedModsOption(QStringList() << "dropUnlistedMods", QCoreApplication::translate("main", "Remove all mods not on the whitelist.")),
	checkMods(false), allowedMods(), dropUnlistedMods(false)
{
	//
}

void AllowedModsCheckModule::registerOptions(QCommandLineParser& parser) {
	parser.addOption(checkModsOption);
	parser.addOption(dropUnlistedModsOption);
}

bool AllowedModsCheckModule::checkArguments(QCommandLineParser& parser) {
	if (parser.isSet(checkModsOption)) {
		checkMods = true;
		QString const whiteListFilename = parser.value(checkModsOption);
		QFile whiteListFile(whiteListFilename);
		if (!whiteListFile.open(QFile::ReadOnly)) {
			std::cerr << "Failed to open/read mod whitelist file '" << whiteListFilename.toStdString() << "'!" << std::endl;
			return false;
		}
		
		QTextStream stream(&whiteListFile);
		QString const whitelist = stream.readAll();
		whiteListFile.close();
		bool const isSimpleNewline = whitelist.count("\r\n") == 0;
		QStringList modList;
		if (isSimpleNewline) {
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
			modList = whitelist.split('\n', QString::SkipEmptyParts);
#else
			modList = whitelist.split('\n', Qt::SplitBehaviorFlags::SkipEmptyParts);
#endif
		} else {
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
			modList = whitelist.split("\r\n", QString::SkipEmptyParts);
#else
			modList = whitelist.split("\r\n", Qt::SplitBehaviorFlags::SkipEmptyParts);
#endif
		}

		for (int i = 0; i < modList.size(); ++i) {
			allowedMods.insert(modList.at(i).toLower());
		}

		std::cout << "Found " << allowedMods.size() << " mods in mod-whitelist file." << std::endl;
	}

	if (parser.isSet(dropUnlistedModsOption)) {
		dropUnlistedMods = true;
	}

	return true;
}

std::shared_ptr<SqmObjectList<SqmStructure>> AllowedModsCheckModule::perform(std::shared_ptr<SqmObjectList<SqmStructure>> const& sqmObjects) {
	std::shared_ptr<SqmObjectList<SqmStructure>> result = sqmObjects;

	std::cout << std::endl << "Checking used mods against whitelist..." << std::endl;
	
	SqmArray* addons = dynamic_cast<SqmArray*>(sqmObjects->getByName(QStringLiteral("addons")));
	if (addons == nullptr) {
		LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "FORMAT ERROR: SQM does not contain array 'addons', is it complete/the correct file?");
	}

	std::vector<SqmArray::ArrayEntry> const& values = addons->getValues();
	std::vector<SqmArray::ArrayEntry> newValues;
	newValues.reserve(values.size());

	QSet<QString> modsToDrop;
	for (std::size_t i = 0; i < values.size(); ++i) {
		bool dropEntry = false;
		SqmArray::ArrayEntry const& entry = values.at(i);
		if (entry.type == SqmArray::ArrayEntryType::STRING) {
			QString const mod = std::get<QString>(entry.content).toLower();
			if (!allowedMods.contains(mod)) {
				std::cout << "Found mod '" << mod.toStdString() << "' which is NOT on the mod whitelist." << std::endl;
				if (dropUnlistedMods) {
					dropEntry = true;
					modsToDrop.insert(mod);
				}
			}

			if (!dropEntry) {
				newValues.push_back(entry);
			}
		} else {
			LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "FORMAT ERROR: SQM contains non-string entry in array 'addons', is it complete/the correct file?");
		}
	}

	if (modsToDrop.size() > 0) {
		result = result->replace(*addons, std::make_shared<SqmArray>(addons->getName(), newValues), result);

		SqmClass* addonsMetaData = dynamic_cast<SqmClass*>(sqmObjects->getByName(QStringLiteral("AddonsMetaData")));
		if (addonsMetaData == nullptr) {
			LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "FORMAT ERROR: SQM does not contain class 'AddonsMetaData', is it complete/the correct file?");
		}

		SqmClass* addonsMetaDataList = dynamic_cast<SqmClass*>(addonsMetaData->getByName(QStringLiteral("List")));
		if (addonsMetaDataList == nullptr) {
			LOG_AND_THROW(zeusops::exceptions::FormatErrorException, "FORMAT ERROR: SQM does not contain class 'AddonsMetaData->List', is it complete/the correct file?");
		}

		QSet<SqmStructure const*> objectsToBeRemoved;
		SqmObjectList<SqmClass> listClasses = addonsMetaDataList->onlyClasses();
		std::vector<std::shared_ptr<SqmClass>> const& listObjects = listClasses.getObjects();
		for (std::size_t i = 0; i < listObjects.size(); ++i) {
			QString const className = listObjects.at(i)->getPropertyValueAsString(QStringLiteral("className")).toLower();
			if (modsToDrop.contains(className)) {
				objectsToBeRemoved.insert(listObjects.at(i).get());
			}
		}

		result = addonsMetaDataList->removeClassItemsWithItemCountDecrement(objectsToBeRemoved, result);
	}

	return result;
}
