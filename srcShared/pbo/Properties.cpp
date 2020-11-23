#include "pbo/Properties.h"

Properties::Properties() : QMap<QString, QString>() {
}

void Properties::set_prefix(QString const& prefix) {
	insert("prefix", prefix);
}

QString Properties::get_prefix() {
	auto it = find("prefix");
	if (it != end())
		return *it;

	return QString();
}

void Properties::set_product(QString const& product) {
	insert("product", product);
}

QString Properties::get_product() {
	auto it = find("product");
	if (it != end())
		return *it;

	return QString();
}

void Properties::set_version(QString const& version) {
	insert("version", version);
}

QString Properties::get_version() {
	auto it = find("version");
	if (it != end())
		return *it;

	return QString();
}
