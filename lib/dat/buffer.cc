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

#include <stdexcept>

#include <boost/detail/endian.hpp>

#include <dat/buffer.hh>

// MemRange
uint8_t MemRange::operator[](size_t offset) const {
	return GetData()[offset];
}

uint8_t MemRange::GetByte(size_t offset) const {
	if (offset + 1 > GetSize())
		throw std::out_of_range("GetByte out of range");
	return GetData()[offset];
}

uint16_t MemRange::GetWord(size_t offset) const {
	if (offset + 2 > GetSize())
		throw std::out_of_range("GetWord out of range");
#if defined BOOST_LITTLE_ENDIAN
	return ((uint16_t)(*this)[offset + 1] << 8) | (uint16_t)(*this)[offset];
#else
	return ((uint16_t)(*this)[offset] << 8) | (uint16_t)(*this)[offset + 1];
#endif
}

uint32_t MemRange::GetDWord(size_t offset) const {
	if (offset + 4 > GetSize())
		throw std::out_of_range("GetDWord out of range");
#if defined BOOST_LITTLE_ENDIAN
	return ((uint32_t)(*this)[offset + 3] << 24) | ((uint32_t)(*this)[offset + 2] << 16) | ((uint32_t)(*this)[offset + 1] << 8) | (uint32_t)(*this)[offset];
#else
	return ((uint32_t)(*this)[offset] << 24) | ((uint32_t)(*this)[offset + 1] << 16) | ((uint32_t)(*this)[offset + 2] << 8) | (uint32_t)(*this)[offset + 3];
#endif
}

int8_t MemRange::GetSByte(size_t offset) const {
	return static_cast<int8_t>(GetByte(offset));
}

int16_t MemRange::GetSWord(size_t offset) const {
	return static_cast<int16_t>(GetWord(offset));
}

int32_t MemRange::GetSDWord(size_t offset) const {
	return static_cast<int32_t>(GetDWord(offset));
}

std::string MemRange::GetString(size_t offset, size_t length) const {
	if (offset + length > GetSize())
		throw std::out_of_range("Get out of range");
	return std::string(reinterpret_cast<const char*>(GetData() + offset), length);
}

Slice MemRange::GetSlice(size_t offset) const {
	return Slice(*this, offset);
}

Slice MemRange::GetSlice(size_t offset, size_t length) const {
	return Slice(*this, offset, length);
}

// Buffer
Buffer::Buffer() {
}

Buffer::Buffer(std::istream& stream, size_t size) : data_(size) {
	stream.read(reinterpret_cast<char*>(data_.data()), size);
}

const unsigned char* Buffer::GetData() const {
	return data_.data();
}

size_t Buffer::GetSize() const {
	return data_.size();
}

void Buffer::Append(unsigned char c) {
	data_.push_back(c);
}

void Buffer::Reserve(size_t size) {
	data_.reserve(size);
}

// Slice
Slice::Slice() : start_(nullptr), size_(0) {
}

Slice::Slice(const MemRange& source, size_t offset = 0) : start_(source.GetData() + offset), size_(source.GetSize() - offset) {
	if (offset > source.GetSize())
		throw std::out_of_range("Slice out of range");
}

Slice::Slice(const MemRange& source, size_t offset, size_t length) : start_(source.GetData() + offset), size_(length) {
	if (offset + length > source.GetSize())
		throw std::out_of_range("Slice out of range");
}

const unsigned char* Slice::GetData() const {
	return start_;
}

size_t Slice::GetSize() const {
	return size_;
}
