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

#include <iostream>

#include <getopt.h>

#include <dat/datfile.hh>

void usage(const char* progname) {
	std::cerr << "Usage: " << progname << " [-hlv] <filename.dat>" << std::endl;
	std::cerr << std::endl;
	std::cerr << "    -l    List datfile entries" << std::endl;
	std::cerr << "    -x    Extract data files into current durectory" << std::endl;
	std::cerr << "    -h    Display this help" << std::endl;
	std::cerr << std::endl;
}

int realmain(int argc, char** argv) {
	int c;
	bool dolist = false, doextract = false;
	const char* progname = argv[0];

	while ((c = getopt(argc, argv, "lx")) != -1) {
		switch (c) {
		case 'l': dolist = true; break;
		case 'x': doextract = true; break;
		case 'h': usage(progname); return 0;
		default:  usage(progname); return 1;
		}
	}

	argc -= optind;
	argv += optind;

	if (argc != 1) {
		usage(progname);
		return 1;
	}

	DatFile datfile(argv[0]);

	if (dolist)
		std::cerr << datfile.GetCount() << " entries" << std::endl;

	for (int i = 0; i < datfile.GetCount(); i++) {
		Buffer unpacked = datfile.GetData(i);
		if (dolist)
			std::cout << "Entry #" << i << " \"" << datfile.GetName(i) << "\", " << unpacked.GetSize() << " bytes" << std::endl;
		if (doextract) {
			std::ofstream of(datfile.GetName(i), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
			of.write(reinterpret_cast<const char*>(unpacked.GetData()), unpacked.GetSize());
		}
	}

	return 0;
}

int main(int argc, char** argv) {
	try {
		return realmain(argc, argv);
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 1;
}

