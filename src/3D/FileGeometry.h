///////////////////////////////////////////////////////////////////////////////
// Name               : FileGeometry.h
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

#ifndef L3D_FILEGEOMETRY_H
#define L3D_FILEGEOMETRY_H

/*!\class FileGeometry
 * \ingroup File3D
 * \brief Abstract file for different 3D formats
 *
 * This file is the abstract basis for the other 3D read and write classes.
 * It provides the storage for the data.
 *
 * 3D formats to look into:
 *
 *  - DAE
 *  - DXF - implemented, need some bugfixing
 *  - FBX
 *  - FLT
 *  - GLTF - very small files <- this is next to implement
 *  - GTS - implemented, but not used anywhere
 *  - OBJ - implemented
 *  - PLY - implemented
 *  - STL - implemented
 *  - USD
 *
 *
 *
 *
 */

#include "Geometry.h"
#include "Vector3.h"

#include <iostream>
#include <string>
#include <vector>

class FileGeometry {
public:
	FileGeometry() = delete;
	explicit FileGeometry(const std::string &filename_);
	/**\brief Constructor to read from a stream
	 *
	 * The stream is stored as a raw pointer. Therefore the Read operation
	 * should follow shortly after creating the object.
	 */
	explicit FileGeometry(std::istream *stream);
	/**\brief Constructor to write to a stream
	 *
	 * The stream is stored as a raw pointer in the object. Therefore the Write
	 * operation should follow shortly after creating the object.
	 */
	explicit FileGeometry(std::ostream *stream);
	virtual ~FileGeometry() = default;

	void Read(Geometry &geometry);
	void Write(const Geometry &geometry);

	virtual void ReadStream(Geometry &geometry);
	virtual void WriteStream(const Geometry &geometry);

	size_t GeometriesInFile() const;

protected:
	std::string StringTrim(const std::string &x) const;

	std::string filename; ///< Last file read / last file written to
	std::istream *inp = nullptr;
	std::ostream *outp = nullptr;

	size_t geometryCount = 0;

public:
	bool joinGeometries = true; ///< If a file contains multiple geometries, these are joint into the same geometry object.
	std::vector<Geometry> geometries; ///< Other geometries, if not joined.

	/**\brief Color for new objects
	 *
	 * Color of object, if no color is provided by the geometry itself.
	 */
	Vector3 color = { 0.8, 0.8, 0.8 };
};

#endif /* L3D_FILEGEOMETRY_H */
