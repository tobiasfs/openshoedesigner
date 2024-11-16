///////////////////////////////////////////////////////////////////////////////
// Name               : FileDXF.h
// Purpose            : Reads a DXF File.
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 07.08.2010
// Copyright          : (C) 2010 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_FILEDXF_H
#define L3D_FILEDXF_H

/*!\class FileDXF
 * \ingroup File3D
 * \brief DXF file
 *
 * Class for reading in DXF files
 *
 * https://images.autodesk.com/adsk/files/autocad_2012_pdf_dxf-reference_enu.pdf
 *
 */

#include "FileGeometry.h"
#include "Vector3.h"

#include <string.h>
#include <vector>
#include <set>

class FileDXF: public FileGeometry {
public:
	FileDXF() = delete;
	explicit FileDXF(const std::string &filename_);
	explicit FileDXF(std::istream *stream);
	virtual ~FileDXF() = default;

	virtual void ReadStream(Geometry &geometry) override;

private:
	void ProcessCode(int codeNr, const std::string &code, Geometry &geometry);

	enum class StateType {
		idle, inSection, inBlock, inEntities, inHeader
	} state = StateType::idle;

	enum class CodeNr : int {
		ENTITY_TYPE = 0,
		ENTITY_VALUE = 1,
		ENTITY_NAME = 2,
		ENTITY_NAME1 = 3,
		ENTITY_NAME2 = 4,
		ENTITY_HANDLE = 5,
		LINE_TYPE = 6,
		LAYER_NAME = 8,
		VARIABLE_NAME = 9,
		ENTITY_THICKNESS = 38,
		VALUE_REOCCURING = 49,
		COLOR_NUMBER = 62
	};

	std::string entityType;
	std::string entityValue;
	std::string entityName;
	std::string entityName1;
	std::string entityName2;
	std::string entityHandle;
	std::string lineType;
	std::string layerName;
	std::string variableName;
	int colorNumber = 0;

	std::array<Vector3, 9> points;
	std::array<double, 9> values;
	std::array<double, 9> angles;
	std::array<int, 9> integers;

	double value = 0.0;
	float thickness = 0.0;

//	long objectFlag = 0;
//	float x = 0.0;
//	float y = 0.0;
//	float z = 0.0;
//	float sx = 0.0;
//	float sy = 0.0;
//	float sz = 0.0;
//	long v0 = 0;
//	long v1 = 0;
//	long v2 = 0;
//	long v3 = 0;
//	long lastGeometry = 0;
//	std::vector<Vector3> v;

	std::set<int> foundCodeNr; ///< For debugging: gives an overview of used IDs.

};

#endif /* L3D_FILEDXF_H */

