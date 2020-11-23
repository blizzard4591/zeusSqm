/*
	Based on the non-Qt implementation by https://github.com/StidOfficial/libpbo
	Licensed under the GPL v3
*/

#ifndef ZEUSSQM_PBO_PBOENTRY_H_
#define ZEUSSQM_PBO_PBOENTRY_H_

#include <QMap>
#include <QString>

#define NULL_PROPERTY ""

class Properties : public QMap<QString, QString> {
public:
	Properties();
	void set_prefix(QString const& entry_name);
	QString get_prefix();
	void set_product(QString const& product);
	QString get_product();
	void set_version(QString const& version);
	QString get_version();
};

#endif
