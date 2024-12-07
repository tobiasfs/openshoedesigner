///////////////////////////////////////////////////////////////////////////////
// Name               : FilePLY.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 10.09.2023
// Copyright          : (C) 2023 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_FILEPLY_H
#define L3D_FILEPLY_H

/*!\class FilePLY
 * \ingroup File3D
 * \brief Reading and writing PLY 3D files
 *
 * https://en.wikipedia.org/wiki/PLY_(file_format)
 *
 */

#include "FileGeometry.h"

#include <vector>

class FilePLY: public FileGeometry {
public:
	enum class DataType {
		NONE, ///< No type selected.
		INT8, ///< char in PLY, int8_t in C++
		UINT8, ///< uchar in PLY, uint8_t in C++
		INT16, ///< short in PLY, int16_t in C++
		UINT16, ///< ushort in PLY, uint16_t in C++
		INT32, ///< int in PLY, int32_t in C++
		UINT32, ///< uint in PLY, uint32_t in C++
		FLOAT32, ///< float in PLY, float in C++
		DOUBLE64 ///< double in PLY, double in C++
	};

	class Property {
	public:
		std::string name;
		DataType typeListSize = DataType::NONE; ///< If not NONE this property is a list.
		DataType type = DataType::NONE;
		bool IsList() const;
	};

	class Element {
	public:
		std::string name;
		size_t count = 0;
		std::vector<Property> properties;
		bool IsEmpty() const;
	};

	explicit FilePLY(const std::string &filename_);
	explicit FilePLY(std::istream *stream);
	explicit FilePLY(std::ostream *stream);
	virtual ~FilePLY() = default;

	virtual void ReadStream(Geometry &geometry) override;
//	virtual void WriteStream(const Geometry & geometry) override;

private:
	void ReadHeader(std::istream &stream);
	void ReadStreamAscii(std::istream &stream, Geometry &geometry);
	void ReadStreamBinaryLE(std::istream &stream, Geometry &geometry);
	void ReadStreamBinaryBE(std::istream &stream, Geometry &geometry);

	DataType StringToType(const std::string &name) const;
	std::string format;
	std::string version;

	std::vector<Element> elements;
};

#endif /* L3D_FILEPLY_H */
