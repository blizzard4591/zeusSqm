#include "pbo/Pbo.h"

#include "exceptions/FormatErrorException.h"

#include <QByteArray>
#include <QDir>
#include <QFileInfo>

#include <iostream>
#include <stdexcept>

#define PACKING_BUFFER_SIZE			4096
#define SIGNATURE_BUFFER_SIZE		1024
#define HEADER_ENTRY_DEFAULT_SIZE	21

#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
#define SHA_DIGEST_LENGTH 20
#else
#define SHA_DIGEST_LENGTH QCryptographicHash::hashLength(QCryptographicHash::Sha1)
#endif

//#define DEBUG_TIMERS 1
//#define DEBUG_SAVE_FILES_AROUND_COMPRESSION

#ifdef DEBUG_TIMERS
#include <chrono>
#endif

namespace PBO {
	PBO::PBO(QString const& file_path, bool signed_file) : PBO(signed_file) {
		open(file_path);
	}

	PBO::PBO(bool signed_file) : std::vector<std::shared_ptr<Entry>>(), m_hash_context(QCryptographicHash::Sha1), m_file(), m_skip_hash_check(false) {
		m_signed = signed_file;
	}

	void PBO::set_skip_hash_check(bool skip_hash_check) {
		m_skip_hash_check = skip_hash_check;
	}

	QString const& PBO::get_path() const {
		return m_path;
	}

	void PBO::signed_file(bool signed_file) {
		m_signed = signed_file;
	}

	bool PBO::is_signed() const {
		return m_signed;
	}

	void PBO::set_signature(QByteArray const& digest) {
		m_checksum = digest.toHex();
	}

	QString const& PBO::signature() const {
		return m_checksum;
	}

	void PBO::set_file_signature(QByteArray const& digest) {
		m_file_checksum = digest.toHex();
	}

	QString const& PBO::file_signature() const {
		return m_file_checksum;
	}

	void PBO::open(QString const& file_path) {
		m_path = file_path;

		if (m_path.endsWith(QStringLiteral(".ebo"))) {
			//std::cout << "WARNING : .ebo is encrypted file" << std::endl;
		}

		m_file.close();

		clear();
		m_path_to_entry_map.clear();
		m_path_to_lowerCaseEntry_map.clear();

		if (is_signed())
			m_hash_context.reset();

		QFileInfo fileInfo(m_path);
		if (fileInfo.isDir())
			throw std::logic_error("Path is a directory!");
	}

	void PBO::read(std::shared_ptr<Entry> const& entry) {
		QByteArray entry_path;
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
				if (entry_original_size == entry_data_size) {
					std::cerr << "WARNING: Entry is marked as compressed (" << entry_original_size << " vs. " << entry_data_size << "), but its not really compressed?!" << std::endl;
				}
				break;
			case PackingMethod::Encrypted:
				std::cerr << "WARNING: Encrypted method is unavailable for entry " << entry->get_path().toStdString() << " !" << std::endl;
				break;
			default:
				std::cerr << "WARNING: Unsupported packing method (" << entry->get_packing_method() << ") !" << std::endl;
				break;
		}
	}

	QByteArray PBO::uncompress(QByteArray const& data, std::size_t const original_size) const {
		QByteArray result;
		result.reserve(static_cast<int>(original_size));
		int position = 0;

		uint8_t formatByte = (uint8_t)data.at(position++);
		int bitNumber = 0;

		while (static_cast<std::size_t>(result.size()) < original_size) {
			if (position >= data.size()) {
#ifdef DEBUG_SAVE_FILES_AROUND_COMPRESSION
				QFile out("entry_orig.bin");
				out.open(QFile::WriteOnly);
				out.write(data);
				out.close();
				out.setFileName("entry_uncom.bin");
				out.open(QFile::WriteOnly);
				out.write(result);
				out.close();
#endif
				throw zeusops::exceptions::FormatErrorException() << "Input data too short, expected another byte!";
			}

			if (bitNumber >= 8) {
				formatByte = data.at(position++);
				bitNumber = 0;
			}
			if (formatByte & 0x1) { // Bit is 1: Append Byte directly
				if (position >= data.size()) {
					throw zeusops::exceptions::FormatErrorException() << "Input data too short, expected another byte!";
				}
				
				result.append(data.at(position++));
			} else { // Bit is 0: Two-Byte Pointer to past data
				if ((position + 1) >= data.size()) {
					throw zeusops::exceptions::FormatErrorException() << "Input data too short, expected another two bytes!";
				}

				uint16_t const B2B1 = (((uint8_t)data.at(position + 1)) << 8) | ((uint8_t)data.at(position));
				int const rpos = result.size() - ((B2B1 & 0x00FFu) | ((B2B1 & 0xF000u) >> 4));
				int const rlen = ((B2B1 & 0x0F00u) >> 8) + 3;

				if ((rpos >= 0) && ((rpos + rlen) <= result.size())) {
					// bytes to copy are within the existing reconstructed data
					for (int i = 0; i < rlen; ++i) {
						result.append(result.at(rpos + i));
					}
				} else if ((rpos >= 0) && ((rpos + rlen) > result.size())) {
					// data to copy exceeds what's available
					int const cappedRlen = result.size() - rpos;
					int added = 0;
					while ((cappedRlen > 0) && (added < rlen)) {
						for (int i = 0; (i < cappedRlen) && (added < rlen); ++i) {
							result.append(result.at(rpos + i));
							++added;
						}
					}
				} else if ((rpos < 0) || ((rpos + rlen) < 0)) {
					// special case
					for (int i = 0; i < rlen; ++i) {
						result.append(' ');
					}
				}
				position += 2;
			}

			formatByte >>= 1;
			++bitNumber;
		}

		return result;
	}

	void PBO::read(uint32_t& value) {
		QByteArray const bytes = m_file.read(4);
		value = *reinterpret_cast<uint32_t const*>(bytes.constData());
		if (is_signed() && !m_skip_hash_check) {
			m_hash_context.addData(bytes);
		}
	}

	void PBO::read(QString& text) {
		QByteArray data;
		while (true) {
			QByteArray const chunk = m_file.peek(4096);
			qint64 const pos = chunk.indexOf('\0');
			if (pos == -1) {
				data.append(m_file.read(4096));
			} else {
				data.append(m_file.read(pos + 1));
				break;
			}
		}

		text = QString::fromUtf8(data);

		if (is_signed() && !m_skip_hash_check) {
			m_hash_context.addData(data);
		}
	}

	void PBO::read(QByteArray& target) {
		QByteArray data;
		while (true) {
			QByteArray const chunk = m_file.peek(4096);
			qint64 const pos = chunk.indexOf('\0');
			if (pos == -1) {
				data.append(m_file.read(4096));
			} else {
				data.append(m_file.read(pos + 1));
				break;
			}
		}

		if (is_signed() && !m_skip_hash_check) {
			m_hash_context.addData(data);
		}
		target = data;
	}

	void PBO::read(char* s, qint64 n) {
		m_file.read(s, n);

		if (is_signed() && !m_skip_hash_check) {
			m_hash_context.addData(s, n);
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
			m_hash_context.addData(s, n);
		}
	}

	void PBO::pack() {
		m_file.setFileName(m_path);
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
		QByteArray const digest = m_hash_context.result();
		
		m_file.write("\0", 1);
		m_file.write(digest.data(), digest.size());

		set_signature(digest);
		set_file_signature(digest);
	}

	void PBO::unpack() {
#ifdef DEBUG_TIMERS
		auto t1 = std::chrono::high_resolution_clock::now();
#endif
		m_file.setFileName(m_path);
		if (!m_file.open(QFile::ReadOnly))
			throw std::logic_error("Failed to open input file for reading!");

		qint64 const file_size = m_file.size();
		while (!m_file.atEnd()) {
			if ((m_file.pos() + HEADER_ENTRY_DEFAULT_SIZE) > file_size)
				throw std::logic_error("Header entry is too small");

			std::shared_ptr<Entry> const entry = std::make_shared<Entry>();
			read(entry);
			if (entry->is_zero_entry()) {
				break;
			}

			m_path_to_entry_map.insert(entry->get_path_as_bytes(), entry);
			m_path_to_lowerCaseEntry_map.insert(QString::fromUtf8(entry->get_path_as_bytes()).toLower(), entry);
			push_back(entry);

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

			QByteArray const file_digest = m_hash_context.result();
			set_file_signature(file_digest);
		} else if (is_signed()) {
			throw std::logic_error("Signature not found");
		}

		if (data_offset > file_size) {
			throw std::logic_error("Is too small");
		}
#ifdef DEBUG_TIMERS
		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		std::cout << "Unpacking PBO took " << duration << "ms." << std::endl;
#endif
	}

	QList<QByteArray> PBO::getAllPaths() const {
		return m_path_to_entry_map.keys();
	}

	bool PBO::extract_all(QString const& targetLocation) const {
		QString dir = targetLocation;
		if (!dir.endsWith('/')) {
			dir.append('/');
		}
		QMapIterator<QByteArray, std::shared_ptr<Entry>> i(m_path_to_entry_map);
		std::size_t counter = 0;
		while (i.hasNext()) {
			i.next();
			QString pathname = QString::fromUtf8(i.key());
			if (pathname.isEmpty() || pathname == QStringLiteral("*.*") || pathname == QStringLiteral("\t\t")) {
				continue;
			}

			std::cout << "Looking at '" << pathname.toStdString() << "' (" << QString(pathname.toUtf8().toBase64()).toStdString() << ")." << std::endl;
			bool foundUnicode = false;
			for (int i = 0; i < pathname.length(); ++i) {
				QChar c = pathname.at(i);
				bool isUnicode = c.unicode() > 127;
				if (isUnicode || !c.isPrint()) {
					foundUnicode = true;
					pathname[i] = '_';
				}
			}
			if (foundUnicode) {
				std::cout << "Sanitized name from '" << QString::fromUtf8(i.key()).toStdString() << "' to '" << pathname.toStdString() << "'." << std::endl;
			}

			int const pos = pathname.lastIndexOf('\\');
			QString filename = pathname;
			QDir d(dir);
			if (pos != -1) {
				QString const path = pathname.left(pos);
				d.mkpath(path);
				d = QDir(dir + path);
				filename = pathname.mid(pos + 1);
			}
			
			try {
				QByteArray const data = read_file_ignore_case(i.key(), true);
				QFile f(d.absoluteFilePath(filename));
				if (!f.open(QFile::WriteOnly)) {
					std::cerr << "Failed to open '" << f.fileName().toStdString() << "' for writing." << std::endl;
					continue;
				}
				f.write(data);
				f.close();
				++counter;
			} catch (std::exception const& ex) {
				std::cerr << "Failed to extract file '" << QString::fromUtf8(i.key()).toStdString() << "': " << ex.what() << std::endl;
			}
		}
		std::cout << "Extracted " << counter << " files from PBO to '" << targetLocation.toStdString() << "'." << std::endl;
		return true;
	}

	bool PBO::has_file(QByteArray const& path) const {
		return m_path_to_entry_map.contains(path);
	}

	bool PBO::has_file_ignore_case(QByteArray const& path) const {
		if (has_file(path)) {
			return true;
		}

		return m_path_to_lowerCaseEntry_map.contains(QString::fromUtf8(path).toLower());
	}

	Entry const& PBO::get_file(QByteArray const& path) const {
		return **m_path_to_entry_map.constFind(path);
	}

	Entry const& PBO::get_file_ignore_case(QByteArray const& path) const {
		if (has_file(path)) {
			return get_file(path);
		}

		return **m_path_to_lowerCaseEntry_map.constFind(QString::fromUtf8(path).toLower());
	}

	QByteArray PBO::read_file_from_entry(Entry const& entry, QByteArray const& path, bool quiet) const {
#ifdef DEBUG_TIMERS
		auto const t1 = std::chrono::high_resolution_clock::now();
#endif
		QByteArray result;

		auto const size = entry.get_data_size();
		auto const originalSize = entry.get_original_size();
		auto const offset = entry.get_data_offset();

		if (size > 0) {
			QFile input(m_path);
			if (!input.open(QFile::ReadOnly)) {
				throw zeusops::exceptions::FormatErrorException() << "Failed to open input PBO archive for entry extraction, is the file still there?";
			}
			input.skip(offset);
			result = input.read(size);
			input.close();
#ifdef DEBUG_SAVE_FILES_AROUND_COMPRESSION
			// Debug
			QByteArray const debug = result;
#endif

			if (entry.get_packing_method() == PackingMethod::Compressed || ((originalSize != 0) && (originalSize != size))) {
#ifdef DEBUG_TIMERS
				auto const t1B = std::chrono::high_resolution_clock::now();
#endif
				QByteArray const fileChecksumData = result.right(4);
				uint32_t const fileChecksum = *reinterpret_cast<uint32_t const*>(fileChecksumData.constData());
				result = uncompress(result, originalSize);

				uint32_t checksum = 0;
				for (int i = 0; i < result.size(); ++i) {
					checksum += (unsigned char)result.at(i);
				}

				if (checksum != fileChecksum) {
					throw zeusops::exceptions::FormatErrorException() << "Failed to properly unpack file: Signature error!";
#ifdef DEBUG_SAVE_FILES_AROUND_COMPRESSION
					QFile out(QString(path).replace('/', "") + "_orig.bin");
					out.open(QFile::WriteOnly);
					out.write(debug);
					out.close();
					out.setFileName(QString(path).replace('/', "") + "_uncom.bin");
					out.open(QFile::WriteOnly);
					out.write(result);
					out.close();
#endif
				}

#ifdef DEBUG_TIMERS
				auto const t2B = std::chrono::high_resolution_clock::now();
				auto const duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2B - t1B).count();
				std::cout << "Uncompressing file took " << duration << "ms." << std::endl;
#endif
			}

			if (!quiet) {
				std::cout << "Loaded '" << path.toStdString() << "' (" << result.size() << " Bytes) from PBO '" << m_path.toStdString() << "'." << std::endl;
			}
		}

#ifdef DEBUG_TIMERS
		auto const t2 = std::chrono::high_resolution_clock::now();
		auto const duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
		std::cout << "Getting file from PBO took " << duration << "ms." << std::endl;
#endif

		return result;
	}

	QByteArray PBO::read_file(QByteArray const& path, bool quiet) const {
		QByteArray result;
		Entry const& entry = get_file(path);

		return read_file_from_entry(entry, path, quiet);
	}

	QByteArray PBO::read_file_ignore_case(QByteArray const& path, bool quiet) const {
		QByteArray result;
		Entry const& entry = get_file_ignore_case(path);

		return read_file_from_entry(entry, path, quiet);
	}

	PBO::~PBO() {
		if (is_signed()) {
			m_hash_context.reset();
		}

		m_file.close();
	}
}
