/*
	Based on the non-Qt implementation by https://github.com/StidOfficial/libpbo
	Licensed under the GPL v3
*/

#ifndef ZEUSSQM_PBO_PBO_H_
#define ZEUSSQM_PBO_PBO_H_

#include <QCryptographicHash>
#include <QFile>
#include <QMap>
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

		QString const& get_path() const;
		void signed_file(bool signed_file);
		bool is_signed()  const;
		QString const& signature() const;
		QString const& file_signature()  const;
		void open(QString const& file_path);

		bool has_file(QByteArray const& path) const;
		bool has_file_ignore_case(QByteArray const& path) const;
		Entry const& get_file(QByteArray const& path) const;
		Entry const& get_file_ignore_case(QByteArray const& path) const;
		QByteArray read_file(QByteArray const& path, bool quiet = false) const;
		QByteArray read_file_ignore_case(QByteArray const& path, bool quiet = false) const;
		QList<QByteArray> getAllPaths() const;
		bool extract_all(QString const& targetLocation) const;

		void pack();
		void unpack();

		void set_skip_hash_check(bool skip_hash_check);
	private:
		QCryptographicHash m_hash_context;
		QString m_path;
		bool m_signed;
		QFile m_file;
		QString m_checksum;
		QString m_file_checksum;
		bool m_skip_hash_check;

		QMap<QByteArray, std::shared_ptr<Entry>> m_path_to_entry_map;
		QMap<QString, std::shared_ptr<Entry>> m_path_to_lowerCaseEntry_map;

		QByteArray uncompress(QByteArray const& data, std::size_t const original_size) const;
		void read(std::shared_ptr<Entry> const& entry);
		void read(uint32_t& value);
		void read(QString& text);
		void read(QByteArray& data);
		void read(char* s, qint64 n);
		void write(Entry* entry);
		void write(uint32_t value);
		void write(QString const& text);
		void write(const char* s, qint64 n);
		void set_signature(QByteArray const& digest);
		void set_file_signature(QByteArray const& digest);
		QByteArray read_file_from_entry(Entry const& entry, QByteArray const& path, bool quiet = false) const;
	};

}

#endif
