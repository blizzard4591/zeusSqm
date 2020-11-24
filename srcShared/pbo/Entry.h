/*
	Based on the non-Qt implementation by https://github.com/StidOfficial/libpbo
	Licensed under the GPL v3
*/

#ifndef ZEUSSQM_PBO_ENTRY_H_
#define ZEUSSQM_PBO_ENTRY_H_

#include <QString>

#include "pbo/Properties.h"

#define ENTRY_PATH_LEN	128-20

namespace PBO {

	enum PackingMethod {
		Uncompressed = 0x0,
		Compressed = 0x43707273,
		Version = 0x56657273,
		Encrypted = 0x456e6372,
		Null = 0xffffffff
	};

	inline std::string to_string(PackingMethod packing_method) {
		switch (packing_method) {
			case Uncompressed:
				return "Uncompressed";
			case Compressed:
				return "Compressed";
			case Version:
				return "Version";
			case Encrypted:
				return "Encrypted";
			case Null:
				return "Null";
			default:
				return "Unknown";
		}
	}

	class Entry {
	public:
		Entry();
		~Entry();
		void set_file_path(QString const& file_path);
		QString const& get_file_path() const;
		void set_path(QByteArray const& path);
		QString const& get_path() const;
		QByteArray const& get_path_as_bytes() const;
		void set_packing_method(uint32_t packing_method);
		uint32_t get_packing_method() const;
		void set_original_size(uint32_t original_size);
		uint32_t get_original_size() const;
		void set_reserved(uint32_t reserved);
		uint32_t get_reserved() const;
		void set_timestamp(uint32_t timestamp);
		uint32_t get_timestamp() const;
		void set_data_size(uint32_t data_size);
		int get_data_size() const;
		void set_data_offset(std::streampos data_offset);
		std::streampos get_data_offset() const;
		Properties& get_properties();
		bool is_version_entry() const;
		bool is_file_entry() const;
		bool is_zero_entry() const;
	private:
		QString m_file_path;
		QString m_path;
		QByteArray m_pathAsBytes;
		uint32_t m_packing_method;
		uint32_t m_original_size;
		uint32_t m_reserved;
		uint32_t m_timestamp;
		uint32_t m_data_size;
		int m_data_offset;
		Properties m_properties;
	};

}

#endif
