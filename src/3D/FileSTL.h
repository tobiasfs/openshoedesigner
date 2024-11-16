///////////////////////////////////////////////////////////////////////////////
// Name               : FileSTL.h
// Purpose            : Reads a STL File
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 11.06.2011
// Copyright          : (C) 2011 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_FILESTL_H
#define L3D_FILESTL_H

/*!\class FileSTL
 * \ingroup File3D
 * \brief Stereolithography files
 *
 * STL is a very widespread file format to exchange geometry data
 * between CAD programs. This class reads a STL file and stores its
 * triangles.
 *
 * https://en.wikipedia.org/wiki/STL_(file_format)
 *
 * Color is represented in the VisCAM / SolidView schema.
 *
 * \todo Check header for Materialise Magics color tags. Also this software uses
 *       BGR instead of RGB.
 *
 */

#include "FileGeometry.h"

#include <iostream>
#include <string>

class FileSTL: public FileGeometry {
public:
	explicit FileSTL(const std::string &filename_);
	explicit FileSTL(std::istream *stream);
	virtual ~FileSTL() = default;

	virtual void ReadStream(Geometry &geometry) override;
	virtual void WriteStream(const Geometry &geometry) override;

private:
	void ReadStreamBinary(std::istream &stream, std::string &header,
			Geometry &geometry);
	void ReadStreamAscii(std::istream &stream, std::string &header,
			Geometry &geometry);

//	void TriangleToStream(std::ostream &stream, const Triangle &tri) const;
};

#endif /* L3D_FILESTL_H */
