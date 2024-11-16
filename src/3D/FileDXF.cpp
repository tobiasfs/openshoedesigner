///////////////////////////////////////////////////////////////////////////////
// Name               : FileDXF.cpp
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

#include "FileDXF.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

FileDXF::FileDXF(const std::string &filename_) :
		FileGeometry(filename_) {
}

void FileDXF::ReadStream(Geometry &geometry) {

	state = StateType::idle;

	int codeNumber;
	*inp >> codeNumber;
	while (inp->good()) {
		std::string line;
		std::getline(*inp, line); // Here the leftovers from the ">> codeNumber" are read.
		std::getline(*inp, line);
		ProcessCode(codeNumber, line, geometry);
		*inp >> codeNumber;
	}
	geometry.Finish();
// Calculate normals to get the lighting right.
//	size_t i;
//	for(i = 0; i < geometry.GetCount(); i++){
//		geometry[i].CalculateNormals();
//	}
}

void FileDXF::ProcessCode(int codeNr, const std::string &code, Geometry &geo) {

	if (codeNr == 0 && !entityType.empty()) {
		// Process available data

		if (entityType.compare("SECTION") == 0) {
			state = StateType::inSection;
			if (entityName.compare("HEADER") == 0)
				state = StateType::inHeader;
			if (entityName.compare("ENTITIES") == 0)
				state = StateType::inEntities;
		}
		if (entityType.compare("ENDSEC") == 0) {
			if (!geo.IsEmpty()) {

			}
			entityName.clear();
			state = StateType::idle;
		}

		if (state == StateType::inEntities && entityType == "3DFACE") {
			if (points[2] == points[3]) {
				geo.AddTriangle(points[0], points[1], points[2]);
			} else {
				geo.AddQuad(points[0], points[1], points[2], points[3]);
			}
		}
	}

	foundCodeNr.insert(codeNr);

// The code numbers are ranging from -5 to 1071.
	switch ((CodeNr) codeNr) {

	case CodeNr::ENTITY_TYPE:
		entityType = code;
		break;

	case CodeNr::ENTITY_VALUE:
		entityValue = code;
		break;

	case CodeNr::ENTITY_NAME:
		entityName = code;
		break;

	case CodeNr::ENTITY_NAME1:
		entityName1 = code;
		break;

	case CodeNr::ENTITY_NAME2:
		entityName2 = code;
		break;

	case CodeNr::ENTITY_HANDLE:
		entityHandle = code;
		break;

	case CodeNr::LINE_TYPE:
		lineType = code;
		break;

	case CodeNr::LAYER_NAME:
		layerName = code;
		break;

	case CodeNr::VARIABLE_NAME:
		variableName = code;
		break;

	case CodeNr::ENTITY_THICKNESS:
		thickness = std::stof(code);
		break;

	case CodeNr::VALUE_REOCCURING:
		value = std::stod(code);
		break;

	case CodeNr::COLOR_NUMBER:
		colorNumber = std::stoi(code);
		break;

	default:
		if (codeNr >= 10 && codeNr <= 18)
			points[(size_t) codeNr - 10].x = std::stof(code);
		if (codeNr >= 20 && codeNr <= 28)
			points[(size_t) codeNr - 20].y = std::stof(code);
		if (codeNr >= 30 && codeNr <= 38)
			points[(size_t) codeNr - 30].z = std::stof(code);
		if (codeNr >= 40 && codeNr <= 48)
			values[(size_t) codeNr - 40] = std::stod(code);
		if (codeNr >= 50 && codeNr <= 58)
			angles[(size_t) codeNr - 50] = std::stod(code);
		if (codeNr >= 70 && codeNr <= 78)
			integers[(size_t) codeNr - 70] = std::stoi(code);
	}

//	if (objectType.compare("INSERT") == 0) {
//		switch (codeNr) {
//		case 10:
//			x = std::stof(code);
//			break;
//		case 20:
//			y = std::stof(code);
//			break;
//		case 30:
//			z = std::stof(code);
//			break;
//		case 41:
//			sx = std::stof(code);
//			break;
//		case 42:
//			sy = std::stof(code);
//			break;
//		case 43:
//			sz = std::stof(code);
//			break;
//		}
//	}
//
//	if (objectType.compare("VERTEX") == 0) {
//		switch (codeNr) {
//		case 10:
//			x = std::stof(code);
//			break;
//		case 20:
//			y = std::stof(code);
//			break;
//		case 30:
//			z = std::stof(code);
//			break;
//		case 71:
//			v0 = std::stol(code);
//			break;
//		case 72:
//			v1 = std::stol(code);
//			break;
//		case 73:
//			v2 = std::stol(code);
//			break;
//		case 74:
//			v3 = std::stol(code);
//			break;
//		}
//	}
//
//	if (codeNr == 0) {
//		// Generate Objects and such
//
//		if (objectType.compare("BLOCK") == 0) {
//			v.clear();
//			TriangleCollection g;
//			g.matrix.TranslateGlobal(x, y, z);
//			g.name = blockName;
////			geometry.Add(g);
////			lastGeometry = geometry.Count() - 1;
//		}
//		if (objectType.compare("VERTEX") == 0) {
//			if (objectFlag == 192) {
//				Vector3 vector(x, y, z);
//				v.push_back(vector);
//			}
//			if (objectFlag == 128) {
//				if (lastGeometry >= 0) {
//					if (v0 > 0 && v1 > 0 && v2 > 0 && v3 < 0) {
////						geometry[lastGeometry].AddTriangle(v[v0 - 1], v[v1 - 1],
////								v[v2 - 1]);
//					}
//					if (v0 > 0 && v1 > 0 && v2 > 0 && v3 > 0)
//
//					{
////						geometry[lastGeometry].AddQuad(v[v0 - 1], v[v1 - 1],
////								v[v2 - 1], v[v3 - 1]);
//					}
//				}
//			}
//		}
//
//		if (objectType.compare("INSERT") == 0) {
//			std::cout << "?\n";
////			for(size_t i = 0; i < geometry.GetCount(); i++){
////				if(geometry[i].name.compare(blockName) == 0){
////					geometry[i].matrix.ScaleGlobal(sx, sy, sz);
////					geometry[i].matrix.TranslateGlobal(x, y, z);
////				}
////			}
//		}
//
//		objectName = "";
//		x = y = z = 0.0;
//		sx = sy = sz = 1.0;
//		v0 = v1 = v2 = v3 = -1;
//		objectType = code;

//	}
}

