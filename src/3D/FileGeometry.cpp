///////////////////////////////////////////////////////////////////////////////
// Name               : FileGeometry.cpp
// Purpose            : Abstract class for geometry file operations
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 11.01.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
// Licence            : GNU General Public License version 3.0 (GPLv3)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////////

#include "FileGeometry.h"

#include <cstring>
#include <exception>
#include <fstream>
#include <string>

#define UNUSED(var) [&var]{}()

FileGeometry::FileGeometry(const std::string &filename_) :
		filename(filename_) {
}

FileGeometry::FileGeometry(std::istream *stream) :
		inp(stream) {

}

FileGeometry::FileGeometry(std::ostream *stream) :
		outp(stream) {
}

void FileGeometry::Read(Geometry &geometry) {
	if (filename.empty()) {
		if (inp == nullptr)
			throw std::logic_error(
					std::string(__FUNCTION__)
							+ " - Missing input stream. Check the construction of this object.");
		if (!inp->good())
			throw std::runtime_error(
					std::string(__FUNCTION__) + "Input stream is not good.");
		ReadStream(geometry);
	} else {
		std::ifstream tempstream(filename);
		if (!tempstream.is_open()) {
			throw std::runtime_error(
					std::string(__FUNCTION__) + " : Geometry File " + filename
							+ ": File won't open: "
							+ std::string(strerror(errno)));
		}

		if (!tempstream.good())
			throw std::runtime_error(
					std::string(__FUNCTION__) + "Can't open " + filename
							+ " for reading.");

		inp = &tempstream;
		ReadStream(geometry);
		inp = nullptr;
		tempstream.close();
	}
}

void FileGeometry::Write(const Geometry &geometry) {
	if (filename.empty()) {
		if (outp == nullptr)
			throw std::logic_error(
					std::string(__FUNCTION__)
							+ " - Missing output stream. Check the construction of this object.");
		if (!outp->good())
			throw std::runtime_error(
					std::string(__FUNCTION__) + "Output stream is not good.");
		WriteStream(geometry);
		outp->flush();
	} else {
		std::ofstream tempstream(filename);
		if (!tempstream.is_open())
			throw std::runtime_error(
					std::string(__FUNCTION__) + " : STL File " + filename
							+ ": Could not open file for writing.");
		if (!tempstream.good())
			throw std::runtime_error(
					std::string(__FUNCTION__) + "Can't open " + filename
							+ " for writing.");
		outp = &tempstream;
		WriteStream(geometry);
		outp = nullptr;
		tempstream.flush();
		tempstream.close();
	}

}

size_t FileGeometry::GeometriesInFile() const {
	return geometryCount;
}

void FileGeometry::ReadStream(Geometry &geometry) {
	UNUSED(geometry);
	throw std::logic_error(
			std::string(__FUNCTION__)
					+ " - Not implemented for this type of file.");
}

void FileGeometry::WriteStream(const Geometry &geometry) {
	UNUSED(geometry);
	throw std::logic_error(
			std::string(__FUNCTION__)
					+ " - Not implemented for this type of file.");
}

std::string FileGeometry::StringTrim(const std::string &x) const {
	const size_t pos0 = x.find_first_not_of(" \t\r\n");
	const size_t pos1 = x.find_last_not_of(" \t\r\n");
	if (pos0 == std::string::npos)
		return std::string();
	return x.substr(pos0, pos1 - pos0 + 1);
}
