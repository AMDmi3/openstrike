/*
 * Copyright (C) 2013-2014 Dmitry Marakasov
 *
 * This file is part of openstrike.
 *
 * openstrike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * openstrike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with openstrike.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DATFILE_HH
#define DATFILE_HH

#include <map>
#include <fstream>

#include "buffer.hh"

class DatFile {
protected:
	struct TocEntry {
		std::string name;
		off_t datfile_offset;
		size_t packed_size;
		size_t unpacked_size_hint;
	};
	typedef std::vector<TocEntry> TocVector;
	typedef std::map<std::string, TocEntry> TocMap;

protected:
	mutable std::ifstream file_;
	TocVector toc_by_num_;
	TocMap toc_by_name_;

protected:
	static const int datfile_header_size_ = 16;
	static const int datfile_header_toc_legth_offset_ = 0;

	static const int datfile_toc_entry_size_ = 16;
	static const int datfile_toc_entry_name_offset_ = 0;
	static const int datfile_toc_entry_name_size_ = 8;
	static const int datfile_toc_entry_data_offset_offset_ = 8;
	static const int datfile_toc_entry_data_packed_size_offset_ = 12;
	static const int datfile_toc_entry_data_unpacked_size_offset_ = 14;

	static const int datfile_paragraph_size_ = 16;

protected:
	const TocEntry& GetTocEntry(int num) const;
	const TocEntry& GetTocEntry(const std::string& name) const;

	Buffer GetData(const TocEntry& entry) const;

public:
	DatFile(const std::string& path);

	int GetCount() const;
	std::string GetName(int num) const;

	bool Exists(const std::string& name) const;

	Buffer GetData(int num) const;
	Buffer GetData(const std::string& name) const;
};

#endif // DATFILE_HH
