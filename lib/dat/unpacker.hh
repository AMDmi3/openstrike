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

#ifndef UNPACKER_HH
#define UNPACKER_HH

class Buffer;
class MemRange;

class Unpacker {
private:
	enum InState {
		READ_TABLE_SIZE,
		READ_ESCAPE_CHAR,

		READ_TABLE_FIRST,
		READ_TABLE_SECOND,
		READ_TABLE_THIRD,

		READ_ESCAPED,
		READ_BYTE,

		READ_BYTE_ONLY,

		END_OF_FILE,
	};

	enum OutState {
		OUT_SINGLE,
		OUT_RLE,

		OUT_LOW_SINGLE,
		OUT_LOW_RLE,

		OUT_ESCAPED,
	};

	InState in_state_;
	OutState out_state_;

	unsigned char table_[256 * 4];
	unsigned char table_size_;
	unsigned char escape_char_;
	unsigned char table_first_;
	unsigned char table_second_;
	int counter_;

private:
	void ResetTable();
	void ProcessEscape(unsigned char in);
	void ProcessByte(unsigned char in, Buffer& out);
	void OutputByte(unsigned char in, Buffer& out);

public:
	Unpacker();
	void Process(const MemRange& in, Buffer& out);
};

#endif // UNPACKER_HH
