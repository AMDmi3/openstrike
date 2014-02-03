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

#include <algorithm>

#include <dat/buffer.hh>

#include <dat/unpacker.hh>

void Unpacker::ResetTable() {
	std::fill(table_, table_ + sizeof(table_), 0);
	for (int i = 0; i < 256; i++)
		table_[i * 4] = i;
}

void Unpacker::ProcessEscape(unsigned char in) {
	if (in == 0) {
		in_state_ = END_OF_FILE;
	} else if (in < 0x40) {
		counter_ = in;
		out_state_ = OUT_SINGLE;
	} else if (in < 0x80) {
		counter_ = (int)(in & 0x3f) << 8;
		out_state_ = OUT_LOW_SINGLE;
	} else if (in == 0x80) {
		in_state_ = READ_TABLE_SIZE;
	} else if (in < 0xc0) {
		counter_ = (in & 0x3f);
		out_state_ = OUT_RLE;
	} else {
		counter_ = (int)(in & 0x3f) << 8;
		out_state_ = OUT_LOW_RLE;
	}
}

void Unpacker::ProcessByte(unsigned char in, Buffer& out) {
	if (table_[in * 4 + 1] == 0) {
		OutputByte(table_[in * 4], out);
	} else {
		if (table_[in * 4 + 3] == 1)
			OutputByte(table_[in * 4 + 2], out);
		else
			ProcessByte(table_[in * 4 + 2], out);

		if (table_[in * 4 + 1] == 1)
			OutputByte(table_[in * 4], out);
		else
			ProcessByte(table_[in * 4], out);
	}
}

void Unpacker::OutputByte(unsigned char in, Buffer& out) {
	switch (out_state_) {
	case OUT_SINGLE:
		if (--counter_ == 0)
			out_state_ = OUT_ESCAPED;
		out.Append(in);
		break;
	case OUT_RLE:
		for (int i = 0; i < counter_; i++)
			out.Append(in);
		out_state_ = OUT_ESCAPED;
		break;
	case OUT_LOW_SINGLE:
		counter_ |= in;
		out_state_ = OUT_SINGLE;
		break;
	case OUT_LOW_RLE:
		counter_ |= in;
		out_state_ = OUT_RLE;
		break;
	case OUT_ESCAPED:
		ProcessEscape(in);
		break;
	}
}

Unpacker::Unpacker(): in_state_(READ_TABLE_SIZE), out_state_(OUT_ESCAPED), counter_(0) {
}

void Unpacker::Process(const MemRange& in, Buffer& out) {
	for (const unsigned char* i = in.GetData(); i != in.GetData() + in.GetSize(); i++) {
		switch (in_state_) {
		case READ_TABLE_SIZE:
			table_size_ = *i;
			ResetTable();
			in_state_ = READ_ESCAPE_CHAR;
			break;
		case READ_ESCAPE_CHAR:
			if (table_size_ == 0) {
				in_state_ = READ_BYTE_ONLY; // XXX: what happens with *i?
				break;
			}

			escape_char_ = *i;
			in_state_ = READ_TABLE_FIRST;
			break;

		case READ_TABLE_FIRST:
			table_first_ = *i;
			in_state_ = READ_TABLE_SECOND;
			break;
		case READ_TABLE_SECOND:
			table_second_ = *i;
			in_state_ = READ_TABLE_THIRD;
			break;
		case READ_TABLE_THIRD:
			table_[table_first_ * 4 + 3] = table_[table_second_ * 4 + 1] ? 2 : 1;
			table_[table_first_ * 4 + 2] = table_second_;
			table_[table_first_ * 4 + 1] = table_[*i * 4 + 1] ? 2 : 1;
			table_[table_first_ * 4] = *i;

			if (--table_size_ == 0)
				in_state_ = READ_BYTE;
			else
				in_state_ = READ_TABLE_FIRST;

			break;

		case READ_ESCAPED:
			in_state_ = READ_BYTE;

			OutputByte(*i, out);
			break;

		case READ_BYTE:
			if (*i == escape_char_)
				in_state_ = READ_ESCAPED;
			else
				ProcessByte(*i, out);

			break;

		case READ_BYTE_ONLY:
			OutputByte(*i, out);
			break;

		case END_OF_FILE:
			return;
		}
	}

	if (in_state_ != END_OF_FILE)
		throw std::logic_error("unexpected end of data");
}
