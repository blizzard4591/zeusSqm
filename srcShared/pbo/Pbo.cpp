#include "pbo/Pbo.h"

#include <QByteArray>
#include <QFileInfo>

#include <iostream>
#include <stdexcept>

#define PACKING_BUFFER_SIZE			4096
#define SIGNATURE_BUFFER_SIZE		1024
#define HEADER_ENTRY_DEFAULT_SIZE	21

#define SHA_DIGEST_LENGTH QCryptographicHash::hashLength(QCryptographicHash::Sha1)

namespace PBO {
	PBO::PBO(QString const& file_path, bool signed_file) : PBO(signed_file) {
		open(file_path);
	}

	PBO::PBO(bool signed_file) : std::vector<std::shared_ptr<Entry>>(), m_hashContext(QCryptographicHash::Sha1), m_file() {
		m_signed = signed_file;
	}

	QString const& PBO::get_path() {
		return m_path;
	}

	void PBO::signed_file(bool signed_file) {
		m_signed = signed_file;
	}

	bool PBO::is_signed() {
		return m_signed;
	}

	void PBO::set_signature(QByteArray const& digest) {
		m_checksum = digest.toHex();
	}

	QString& PBO::signature() {
		return m_checksum;
	}

	void PBO::set_file_signature(QByteArray const& digest) {
		m_file_checksum = digest.toHex();
	}

	QString& PBO::file_signature() {
		return m_file_checksum;
	}

	void PBO::open(QString const& file_path) {
		m_path = file_path;

		if (m_path.endsWith(QStringLiteral(".ebo"))) {
			//std::cout << "WARNING : .ebo is encrypted file" << std::endl;
		}

		m_file.close();

		clear();

		if (is_signed())
			m_hashContext.reset();

		QFileInfo fileInfo(m_path);
		if (fileInfo.isDir())
			throw std::logic_error("Path is a directory!");
	}

	void PBO::read(Entry*& entry) {
		QString entry_path;
		uint32_t entry_packing_method;
		uint32_t entry_original_size;
		uint32_t entry_reserved;
		uint32_t entry_timestamp;
		uint32_t entry_data_size;

		QString property_key;
		QString property_value;

		read(entry_path);
		read(entry_packing_method);
		read(entry_original_size);
		read(entry_reserved);
		read(entry_timestamp);
		read(entry_data_size);

		entry = new Entry();

		entry->set_path(entry_path);
		entry->set_packing_method(entry_packing_method);
		entry->set_original_size(entry_original_size);
		entry->set_reserved(entry_reserved);
		entry->set_timestamp(entry_timestamp);
		entry->set_data_size(entry_data_size);

		if (entry->is_zero_entry())
			return;

		switch (entry->get_packing_method()) {
			case PackingMethod::Version:
			{
				do {
					read(property_key);
					if (property_key.isEmpty())
						break;

					read(property_value);

					entry->get_properties().insert(property_key, property_value);

					property_key = "";
					property_value = "";
				} while (true);

				if (!property_key.isEmpty())
					entry->get_properties().insert(property_key, property_value);

				break;
			}
			case PackingMethod::Uncompressed:
				break;
			case PackingMethod::Compressed:
				break;
			case PackingMethod::Encrypted:
				std::cerr << "WARNING : Encrypted method is unavailable for entry " << entry->get_path().toStdString() << " !" << std::endl;
				break;
			default:
				std::cerr << "WARNING : Unsupported packing method (" << entry->get_packing_method() << ") !" << std::endl;
				break;
		}
	}

	void PBO::read(uint32_t& value) {
		read(reinterpret_cast<char*>(&value), sizeof(value));
	}

	void PBO::read(QString& text) {
		QByteArray data;
		while (true) {
			QByteArray const chunk = m_file.peek(4096);
			int const pos = chunk.indexOf('\0');
			if (pos == -1) {
				data.append(m_file.read(4096));
			} else {
				data.append(m_file.read(pos + 1));
				break;
			}
		}

		text = QString::fromUtf8(data);

		if (is_signed()) {
			m_hashContext.addData(data);
		}
	}

	void PBO::read(char* s, qint64 n) {
		m_file.read(s, n);

		if (is_signed()) {
			m_hashContext.addData(s, n);
		}
	}

	void PBO::write(Entry* entry) {
		write(entry->get_path());
		write(entry->get_packing_method());
		write(entry->get_original_size());
		write(entry->get_reserved());
		write(entry->get_timestamp());
		write(entry->get_data_size());

		switch (entry->get_packing_method()) {
			case PackingMethod::Version:
			{
				QMap<QString, QString>::const_iterator it = entry->get_properties().constBegin();
				while (it != entry->get_properties().constEnd()) {
					if (it.key().isEmpty())
						break;

					write(it.key());

					if (it.value().isEmpty())
						break;

					write(it.value());
				}

				write(NULL_PROPERTY);
				break;
			}
			case PackingMethod::Uncompressed:
				break;
			case PackingMethod::Compressed:
			case PackingMethod::Encrypted:
				throw std::logic_error("Packed method is unavailable !");
				break;
			default:
				throw std::logic_error("Unsupported packing method");
				break;
		}
	}

	void PBO::write(uint32_t value) {
		write(reinterpret_cast<const char*>(&value), sizeof(uint32_t));
	}

	void PBO::write(QString const& text) {
		QByteArray const data = text.toUtf8();

		write(data.constData(), data.length());
	}

	void PBO::write(const char* s, qint64 n) {
		m_file.write(s, n);

		if (is_signed()) {
			m_hashContext.addData(s, n);
		}
	}

	void PBO::pack() {
		if (!m_file.open(QFile::WriteOnly))
			throw std::logic_error("Failed to open output for writing!");

		// Write all entries
		for (auto& entry : *this)
			write(entry.get());

		// Write zero entry to end the headers
		char zeroEntry[HEADER_ENTRY_DEFAULT_SIZE] = { 0 };
		write(zeroEntry, sizeof(zeroEntry));

		// Write all data entries
		char entryFileBuffer[PACKING_BUFFER_SIZE];
		for (auto& entry : *this) {
			switch (entry->get_packing_method()) {
				case PackingMethod::Uncompressed:
				{
					QFile entryFile(entry->get_file_path());
					if (!entryFile.open(QFile::ReadOnly))
						throw std::logic_error("Failed to open input file for reading!");

					while (!entryFile.atEnd()) {
						int const bytesRead = entryFile.read(entryFileBuffer, sizeof(entryFileBuffer));
						write(entryFileBuffer, bytesRead);
					}

					entryFile.close();
					break;
				}
				default:
					break;
			}
		}

		// Write data signature
		QByteArray const digest = m_hashContext.result();
		
		m_file.write("\0", 1);
		m_file.write(digest.data(), digest.size());

		set_signature(digest);
		set_file_signature(digest);
	}

	void PBO::unpack() {
		if (!m_file.open(QFile::ReadOnly))
			throw std::logic_error("Failed to open input file for reading!");

		qint64 const file_size = m_file.size();
		
		Entry* entry = nullptr;
		while (!m_file.atEnd()) {
			if ((m_file.pos() + HEADER_ENTRY_DEFAULT_SIZE) > file_size)
				throw std::logic_error("Header entry is too small");

			read(entry);
			if (entry->is_zero_entry()) {
				delete entry;
				break;
			}

			emplace_back(entry);

			if (m_file.atEnd())
				throw std::logic_error("No zero entry found");
		}

		// Check entry data integrity
		qint64 data_offset = m_file.pos();
		int leftDataLength;

		for (auto& entry : *this) {
			entry->set_data_offset(data_offset);

			data_offset = data_offset + std::streampos(entry->get_data_size());
			leftDataLength = entry->get_data_size();

			char signatureData[SIGNATURE_BUFFER_SIZE];
			while (leftDataLength > 0) {
				if (leftDataLength > SIGNATURE_BUFFER_SIZE)
					read(signatureData, SIGNATURE_BUFFER_SIZE);
				else
					read(signatureData, leftDataLength);

				leftDataLength -= SIGNATURE_BUFFER_SIZE;
			}
		}

		// Get and check signatures
		if ((file_size - data_offset) == HEADER_ENTRY_DEFAULT_SIZE) {
			m_file.read(1); // entry path always empty, skip

			QByteArray const pbo_digest = m_file.read(SHA_DIGEST_LENGTH);
			set_signature(pbo_digest);

			QByteArray const file_digest = m_hashContext.result();
			set_file_signature(file_digest);
		} else if (is_signed()) {
			throw std::logic_error("Signature not found");
		}

		if (data_offset > file_size) {
			throw std::logic_error("Is too small");
		}
	}

	PBO::~PBO() {
		if (is_signed()) {
			m_hashContext.reset();
		}

		m_file.close();
	}
}
