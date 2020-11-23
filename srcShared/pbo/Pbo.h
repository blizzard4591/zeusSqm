/*
	Based on the non-Qt implementation by https://github.com/StidOfficial/libpbo
	Licensed under the GPL v3
*/

#ifndef ZEUSSQM_PBO_PBO_H_
#define ZEUSSQM_PBO_PBO_H_

#include <QCryptographicHash>
#include <QFile>
#include <QString>

#include <memory>
#include <vector>

#include "pbo/Entry.h"

namespace PBO {

	class PBO : public std::vector<std::shared_ptr<Entry>> {
	public:
		PBO(QString const& filePath, bool signed_file = true);
		PBO(bool signed_file = true);
		virtual ~PBO();

		QString const& get_path();
		void signed_file(bool signed_file);
		bool is_signed();
		QString& signature();
		QString& file_signature();
		void open(QString const& file_path);

		void pack();
		void unpack();
	private:
		QCryptographicHash m_hashContext;
		QString m_path;
		bool m_signed;
		QFile m_file;
		QString m_checksum;
		QString m_file_checksum;

		void read(Entry*& entry);
		void read(uint32_t& value);
		void read(QString& text);
		void read(char* s, qint64 n);
		void write(Entry* entry);
		void write(uint32_t value);
		void write(QString const& text);
		void write(const char* s, qint64 n);
		void set_signature(QByteArray const& digest);
		void set_file_signature(QByteArray const& digest);
	};

}

#endif
