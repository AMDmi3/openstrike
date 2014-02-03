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

#include <cassert>

#include <dat/unpacker.hh>

#include <dat/datfile.hh>

DatFile::DatFile(const std::string& path) : file_(path, std::ios_base::in | std::ios_base::binary) {
	// open file & determine size
	file_.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	file_.seekg(0, std::ifstream::end);
	size_t file_size = file_.tellg();
	file_.seekg(0);

	// read header
	Buffer header(file_, datfile_header_size_);
	int num_entries = header.GetDWord(datfile_header_toc_legth_offset_);

	// read table of contents
	Buffer toc(file_, datfile_toc_entry_size_ * num_entries);

	for (int i = 0; i < num_entries; i++) {
		Slice this_entry = toc.GetSlice(i * datfile_toc_entry_size_, datfile_toc_entry_size_);

		// clear zero filling from name
		std::string name = this_entry.GetString(datfile_toc_entry_name_offset_, datfile_toc_entry_name_size_);
		while (name.size() > 0 && name.back() == ' ')
			name.pop_back();

		// calculate start end end of data for this entry
		TocEntry entry_info;
		entry_info.datfile_offset = this_entry.GetDWord(datfile_toc_entry_data_offset_offset_);
		if (i < num_entries - 1) {
			Slice next_entry = toc.GetSlice((i + 1) * datfile_toc_entry_size_, datfile_toc_entry_size_);
			entry_info.packed_size = next_entry.GetDWord(datfile_toc_entry_data_offset_offset_) - entry_info.datfile_offset;
		} else {
			entry_info.packed_size = file_size - entry_info.datfile_offset;
		}

		entry_info.name = name;
		entry_info.unpacked_size_hint = this_entry.GetWord(datfile_toc_entry_data_unpacked_size_offset_) * datfile_paragraph_size_;

		toc_by_num_.push_back(entry_info);
		toc_by_name_.emplace(name, entry_info);
	}
}

const DatFile::TocEntry& DatFile::GetTocEntry(int num) const {
	return toc_by_num_.at(num);
}

const DatFile::TocEntry& DatFile::GetTocEntry(const std::string& name) const {
	TocMap::const_iterator entry = toc_by_name_.find(name);
	if (entry == toc_by_name_.end())
		throw std::logic_error("cannot find requested data in datfile");

	return entry->second;
}

Buffer DatFile::GetData(const DatFile::TocEntry& entry) const {
	file_.seekg(entry.datfile_offset);
	Buffer packed(file_, entry.packed_size);

	Buffer unpacked;
	unpacked.Reserve(entry.unpacked_size_hint);

	Unpacker().Process(packed, unpacked);

	if (unpacked.GetSize() > entry.unpacked_size_hint)
		throw std::logic_error("data is larger than expected unpacked size");

	return unpacked;
}

int DatFile::GetCount() const {
	return toc_by_num_.size();
}

std::string DatFile::GetName(int num) const {
	return toc_by_num_.at(num).name;
}

Buffer DatFile::GetData(int num) const {
	return GetData(GetTocEntry(num));
}

Buffer DatFile::GetData(const std::string& name) const {
	return GetData(GetTocEntry(name));
}
