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

#ifndef BUFFER_HH
#define BUFFER_HH

#include <vector>
#include <iostream>
#include <cstddef>
#include <string>

class Slice;

class MemRange {
protected:
	virtual ~MemRange() {}

public:
	virtual const unsigned char* GetData() const = 0;
	virtual size_t GetSize() const = 0;

	uint8_t operator[](size_t offset) const;

	uint8_t GetByte(size_t offset) const;
	uint16_t GetWord(size_t offset) const;
	uint32_t GetDWord(size_t offset) const;
	int8_t GetSByte(size_t offset) const;
	int16_t GetSWord(size_t offset) const;
	int32_t GetSDWord(size_t offset) const;

	std::string GetString(size_t offset, size_t length) const;

	Slice GetSlice(size_t offset) const;
	Slice GetSlice(size_t offset, size_t length) const;
};

class Buffer : public MemRange {
protected:
	std::vector<unsigned char> data_;

public:
	Buffer();
	Buffer(std::istream& stream, size_t size);
	virtual ~Buffer() {}

	/* It's better to have these explicit declarations, however
	 * currently only gcc-4.9 and clang-3.4 are able to compile them
	Buffer(const Buffer&) = default;
	Buffer(Buffer&&) noexcept = default;
	Buffer& operator=(const Buffer&) = default;
	Buffer& operator=(Buffer&&) noexcept = default;
	*/

	virtual const unsigned char* GetData() const;
	virtual size_t GetSize() const;

	void Append(unsigned char c);
	void Reserve(size_t size);
};

class Slice : public MemRange {
protected:
	const unsigned char* start_;
	size_t size_;

public:
	Slice();
	Slice(const MemRange& source, size_t offset);
	Slice(const MemRange& source, size_t offset, size_t length);

	virtual ~Slice() {}

	/* It's better to have these explicit declarations, however
	 * currently only gcc-4.9 and clang-3.4 are able to compile them
	Slice(const Slice&) = default;
	Slice(Slice&&) noexcept = default;
	Slice& operator=(const Slice&) = default;
	Slice& operator=(Slice&&) noexcept = default;
	*/

	virtual const unsigned char* GetData() const;
	virtual size_t GetSize() const;
};

#endif // BUFFER_HH
