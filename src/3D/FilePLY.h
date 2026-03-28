///////////////////////////////////////////////////////////////////////////////
// Name               : FilePLY.h
// Purpose            : Reading and writing PLY files.
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : C++ >= 17
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
 * https://paulbourke.net/dataformats/ply/
 *
 */

#include "FileGeometry.h"

#include <vector>
#include <variant>

class FilePLY: public FileGeometry {
private:
	enum struct DataType : size_t {
		INT8 = 0, ///< char in PLY, int8_t in C++
		UINT8 = 1, ///< uchar in PLY, uint8_t in C++
		INT16 = 2, ///< short in PLY, int16_t in C++
		UINT16 = 3, ///< ushort in PLY, uint16_t in C++
		INT32 = 4, ///< int in PLY, int32_t in C++
		UINT32 = 5, ///< uint in PLY, uint32_t in C++
		FLOAT32 = 6, ///< float in PLY, float in C++
		DOUBLE64 = 7, ///< double in PLY, double in C++
		NONE = 100, ///< No type selected.
	};
	// The typedef holds the alternatives in the same order as the DataType
	// enum, so that the index() function on the alternatives reflects the
	// value of the DataType enum. (index() returns a size_t.)
	typedef std::variant<int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t,
			float, double> VarValue;

	enum struct ElementType {
		UNKNOWN, VERTEX, EDGE, TRIANGLE
	};

	enum struct Target {
		UNINTERESTING,
		V_X,
		V_Y,
		V_Z,
		V_NX,
		V_NY,
		V_NZ,
		V_R,
		V_G,
		V_B,
		V_A,
		V_U,
		V_V,
		V_GROUP,
		E_VA,
		E_VB,
		E_TA,
		E_TB,
		E_NX,
		E_NY,
		E_NZ,
		E_R,
		E_G,
		E_B,
		E_A,
		E_GROUP,
		E_TRIANGLECOUNT,
		E_SHARP,
		E_FLIP,
		T_VERTEXINDEX,
		T_EDGEINDEX,
		T_NX,
		T_NY,
		T_NZ,
		T_TX,
		T_TY,
		T_TZ,
		T_BX,
		T_BY,
		T_BZ,
		T_R,
		T_G,
		T_B,
		T_A,
		T_TUA,
		T_TVA,
		T_TUB,
		T_TVB,
		T_TUC,
		T_TVC,
		T_GROUP,
		T_FLIP
	};

	class Property {
	public:
		std::string name;
		DataType typeListSize = DataType::NONE; ///< If not NONE this property is a list.
		DataType type = DataType::NONE;
		Target target = Target::UNINTERESTING;
		bool IsList() const;
	};

	class Element {
	public:
		std::string name;
		ElementType type = ElementType::UNKNOWN;
		size_t count = 0;
		std::vector<Property> properties;
		bool IsEmpty() const;
	};
public:
	explicit FilePLY(const std::string &filename_);
	explicit FilePLY(std::istream *stream);
	explicit FilePLY(std::ostream *stream);

	void ReadStream(Geometry &geometry) override;
	void WriteStream(const Geometry &geometry) override;

	/**\brief Modify the exported file to be readable by Meshlab.
	 *
	 * Meshlabs PLY importer is broken since forever.
	 *
	 * E.g. Indices for triangle corners have to be uint, but the vertices
	 * in an edge have to be referenced by int. If uint is used here, Meshlab
	 * crashes.
	 *
	 * This mode modifies the generated file to be compatible with Meshlab.
	 * The file generated does not contain as much information as normally,
	 * but works with Meshlab.
	 */
	bool meshlabCompatibilityMode = false;

private:
	void ReadHeader(std::istream &stream, Geometry &geometry);
	void ReadStreamAscii(std::istream &stream, Geometry &geometry);
	void ReadStreamBinaryLE(std::istream &stream, Geometry &geometry);
	void ReadStreamBinaryBE(std::istream &stream, Geometry &geometry);

	DataType StringToType(const std::string &name) const;

	VarValue ReadValue(std::istream &stream, DataType datatype);

	static bool ToBool(const VarValue value);
	static size_t ToSize(const VarValue value);
	static uint8_t ToUint8(const VarValue value);
	static float ToFloat(const VarValue value);
	static double ToDouble(const VarValue value);

	std::string format;
	std::string version;

	std::vector<Element> elements;
};

#endif /* L3D_FILEPLY_H */
