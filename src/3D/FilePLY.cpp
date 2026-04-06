///////////////////////////////////////////////////////////////////////////////
// Name               : FilePLY.cpp
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

#include "FilePLY.h"

#include "Geometry.h"

#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stddef.h>
#include <stdexcept>
#include <string>

FilePLY::FilePLY(const std::string &filename_) :
		FileGeometry(filename_) {
}

FilePLY::FilePLY(std::istream *stream) :
		FileGeometry(stream) {
}

FilePLY::FilePLY(std::ostream *stream) :
		FileGeometry(stream) {
}

void FilePLY::ReadStream(Geometry &geo) {

	std::string word;
	*inp >> word;
	if (word != "ply")
		throw std::runtime_error(
				"FilePLY::ReadFile : The file " + filename
						+ " is not a valid PLY file.");
	geo.Clear();
	ReadHeader(*inp, geo);

	if (format == "ascii") {
		ReadStreamAscii(*inp, geo);
	} else {
		if (format == "binary_little_endian") {
			ReadStreamBinaryLE(*inp, geo);
		} else {
			if (format != "binary_big_endian")
				throw std::runtime_error(
						"FilePLY::ReadFile - The format '" + format
								+ "' is not recognized.");
			ReadStreamBinaryBE(*inp, geo);
		}
	}
}

void FilePLY::ReadHeader(std::istream &stream, Geometry &geo) {

	std::string word;
	std::string rest;
	Element e;
	stream >> word;

	while (word != "end_header") {

		if (word == "comment")
			std::getline(stream, rest);
		if (word == "format") {
			stream >> format;
			stream >> version;
			std::getline(stream, rest);
		}
		if (word == "element") {
			if (!e.IsEmpty()) {
				elements.push_back(e);
				e = Element();
			}
			stream >> e.name;
			stream >> e.count;
			std::getline(stream, rest);
		}
		if (word == "property") {
			Property p;
			stream >> word;
			if (word == "list") {
				stream >> word;
				p.typeListSize = StringToType(word);
				stream >> word;
			}
			p.type = StringToType(word);
			stream >> p.name;
			std::getline(stream, rest);

			if (e.name == "vertex" || e.name == "vertices" || e.name == "point"
					|| e.name == "points") {
				e.type = ElementType::VERTEX;
				if (p.name == "x") {
					p.target = Target::V_X;
				} else if (p.name == "y") {
					p.target = Target::V_Y;
				} else if (p.name == "z") {
					p.target = Target::V_Z;
				} else if (p.name == "nx") {
					p.target = Target::V_NX;
					geo.verticesHaveNormal = true;
				} else if (p.name == "ny") {
					p.target = Target::V_NY;
					geo.verticesHaveNormal = true;
				} else if (p.name == "nz") {
					p.target = Target::V_NZ;
					geo.verticesHaveNormal = true;
				} else if (p.name == "red" || p.name == "r") {
					p.target = Target::V_R;
					geo.verticesHaveColor = true;
				} else if (p.name == "green" || p.name == "g") {
					p.target = Target::V_G;
					geo.verticesHaveColor = true;
				} else if (p.name == "blue" || p.name == "b") {
					p.target = Target::V_B;
					geo.verticesHaveColor = true;
				} else if (p.name == "alpha" || p.name == "a"
						|| p.name == "opacity") {
					p.target = Target::V_A;
					geo.verticesHaveColor = true;
				} else if (p.name == "s" || p.name == "u") {
					p.target = Target::V_U;
					geo.verticesHaveTexture = true;
				} else if (p.name == "t" || p.name == "v") {
					p.target = Target::V_V;
					geo.verticesHaveTexture = true;
				} else if (p.name == "gr" || p.name == "group") {
					p.target = Target::V_GROUP;
				}
			} else if (e.name == "edge" || e.name == "edges" || e.name == "line"
					|| e.name == "lines") {
				e.type = ElementType::EDGE;
				if (p.name == "vertex1" || p.name == "va") {
					p.target = Target::E_VA;
				} else if (p.name == "vertex2" || p.name == "vb") {
					p.target = Target::E_VB;
				} else if (p.name == "triangle1" || p.name == "ta") {
					p.target = Target::E_TA;
				} else if (p.name == "triangle2" || p.name == "tb") {
					p.target = Target::E_TB;
				} else if (p.name == "nx") {
					p.target = Target::E_NX;
					geo.edgesHaveNormal = true;
				} else if (p.name == "ny") {
					p.target = Target::E_NY;
					geo.edgesHaveNormal = true;
				} else if (p.name == "nz") {
					p.target = Target::E_NZ;
					geo.edgesHaveNormal = true;
				} else if (p.name == "red" || p.name == "r") {
					p.target = Target::E_R;
					geo.edgesHaveColor = true;
				} else if (p.name == "green" || p.name == "g") {
					p.target = Target::E_G;
					geo.edgesHaveColor = true;
				} else if (p.name == "blue" || p.name == "b") {
					p.target = Target::E_B;
					geo.edgesHaveColor = true;
				} else if (p.name == "alpha" || p.name == "a"
						|| p.name == "opacity") {
					p.target = Target::E_A;
					geo.edgesHaveColor = true;
				} else if (p.name == "gr" || p.name == "group") {
					p.target = Target::E_GROUP;
				} else if (p.name == "triangle_count" || p.name == "tc") {
					p.target = Target::E_TRIANGLECOUNT;
				} else if (p.name == "sharp") {
					p.target = Target::E_SHARP;
				} else if (p.name == "flip") {
					p.target = Target::E_FLIP;
				}

			} else if (e.name == "face" || e.name == "faces"
					|| e.name == "triangle" || e.name == "triangles") {
				e.type = ElementType::TRIANGLE;
				if (p.name == "vertex_indices" || p.name == "vertex_index") {
					p.target = Target::T_VERTEXINDEX;
				} else if (p.name == "edge_indices" || p.name == "edge_index") {
					p.target = Target::T_EDGEINDEX;
				} else if (p.name == "nx") {
					p.target = Target::T_NX;
					geo.trianglesHaveNormal = true;
				} else if (p.name == "ny") {
					p.target = Target::T_NY;
					geo.trianglesHaveNormal = true;
				} else if (p.name == "nz") {
					p.target = Target::T_NZ;
					geo.trianglesHaveNormal = true;
				} else if (p.name == "tx") {
					p.target = Target::T_TX;
					geo.trianglesHaveNormal = true;
				} else if (p.name == "ty") {
					p.target = Target::T_TY;
					geo.trianglesHaveNormal = true;
				} else if (p.name == "tz") {
					p.target = Target::T_TZ;
					geo.trianglesHaveNormal = true;
				} else if (p.name == "bx") {
					p.target = Target::T_BX;
					geo.trianglesHaveNormal = true;
				} else if (p.name == "by") {
					p.target = Target::T_BY;
					geo.trianglesHaveNormal = true;
				} else if (p.name == "bz") {
					p.target = Target::T_BZ;
					geo.trianglesHaveNormal = true;
				} else if (p.name == "red" || p.name == "r") {
					p.target = Target::T_R;
					geo.trianglesHaveColor = true;
				} else if (p.name == "green" || p.name == "g") {
					p.target = Target::T_G;
					geo.trianglesHaveColor = true;
				} else if (p.name == "blue" || p.name == "b") {
					p.target = Target::T_B;
					geo.trianglesHaveColor = true;
				} else if (p.name == "alpha" || p.name == "a"
						|| p.name == "opacity") {
					p.target = Target::T_A;
					geo.trianglesHaveColor = true;

				} else if (p.name == "tua" || p.name == "ua") {
					p.target = Target::T_TUA;
					geo.trianglesHaveTexture = true;
				} else if (p.name == "tva" || p.name == "va") {
					p.target = Target::T_TVA;
					geo.trianglesHaveTexture = true;
				} else if (p.name == "tub" || p.name == "ub") {
					p.target = Target::T_TUB;
					geo.trianglesHaveTexture = true;
				} else if (p.name == "tvb" || p.name == "vb") {
					p.target = Target::T_TVB;
					geo.trianglesHaveTexture = true;
				} else if (p.name == "tuc" || p.name == "uc") {
					p.target = Target::T_TUC;
					geo.trianglesHaveTexture = true;
				} else if (p.name == "tvc" || p.name == "vc") {
					p.target = Target::T_TVC;
					geo.trianglesHaveTexture = true;
				} else if (p.name == "gr" || p.name == "group") {
					p.target = Target::T_GROUP;
				} else if (p.name == "flip") {
					p.target = Target::T_FLIP;
				}
			}
			e.properties.push_back(p);
		}
		stream >> word;
		if (stream.eof())
			throw std::runtime_error(
					"FilePLY::ReadHeader : Unexpected end of file while reading the header.");
	}
	if (!e.IsEmpty())
		elements.push_back(e);
	std::getline(stream, rest); // After end_header follows a return (char(10)).
}

bool FilePLY::Property::IsList() const {
	return typeListSize != DataType::NONE;
}

bool FilePLY::Element::IsEmpty() const {
	return properties.empty();
}

void FilePLY::ReadStreamAscii(std::istream &stream, Geometry &geo) {
	std::vector<Geometry::Vertex> v;
	std::vector<Vector3> vn;
	std::vector<Vector3> vc;
	for (const Element &e : elements) {
		if (e.type == ElementType::VERTEX) {
			v.resize(e.count);
			vn.resize(e.count);
			vc.resize(e.count);
			for (size_t i = 0; i < e.count; i++) {
				for (const Property &p : e.properties) {
					if (p.IsList())
						throw std::runtime_error(
								"FilePLY::ReadStreamAscii - A list is not expected in the vertex definition.");
					float val_float;
					stream >> val_float;

					if (p.type == DataType::UINT8)
						val_float /= 255.0f;

					if (p.type == DataType::NONE || p.type == DataType::UINT16
							|| p.type == DataType::UINT32
							|| p.type == DataType::INT8
							|| p.type == DataType::INT16
							|| p.type == DataType::INT32)
						throw std::runtime_error(
								"FilePLY::ReadStreamAscii - Unexpected datatype.");

					switch (p.target) {
					case Target::V_X:
						v[i].x = val_float;
						break;
					case Target::V_Y:
						v[i].y = val_float;
						break;
					case Target::V_Z:
						v[i].z = val_float;
						break;
					case Target::V_NX:
						v[i].n.x = val_float;
						break;
					case Target::V_NY:
						v[i].n.y = val_float;
						break;
					case Target::V_NZ:
						v[i].n.z = val_float;
						break;
					case Target::V_R:
						v[i].c.r = val_float;
						break;
					case Target::V_G:
						v[i].c.g = val_float;
						break;
					case Target::V_B:
						v[i].c.b = val_float;
						break;
					case Target::V_A:
						v[i].c.a = val_float;
						break;
					}
				}
			}
		}
		if (e.type == ElementType::TRIANGLE) {
			if (e.properties.size() != 1)
				throw std::runtime_error(
						"FilePLY::ReadStreamAscii - Expected exactly one property for '"
								+ e.name + "'.");
			auto p = e.properties[0];
			if (p.name != "vertex_index" && p.name != "vertex_indices")
				throw std::runtime_error(
						"FilePLY::ReadStreamAscii - Expected the name 'vertex_index' or 'vertex_indices' but got '"
								+ p.name + "'.");
			if (!p.IsList())
				throw std::runtime_error(
						"FilePLY::ReadStreamAscii - Expected a list.");
			if (p.typeListSize == DataType::FLOAT32
					|| p.typeListSize == DataType::DOUBLE64)
				throw std::runtime_error(
						"FilePLY::ReadStreamAscii - The list size has to be an integer.");
			if (p.type == DataType::FLOAT32 || p.type == DataType::DOUBLE64)
				throw std::runtime_error(
						"FilePLY::ReadStreamAscii - The index in the list has to be an integer.");

			for (size_t i = 0; i < e.count; i++) {
				int N;
				stream >> N;
				std::vector<int> idx(N, 0);
				for (int j = 0; j < N; j++)
					stream >> idx[j];

				if (geo.verticesHaveNormal)
					geo.SetAddNormal(vn[idx[0]]);
				else
					geo.ResetAddNormal();
				if (geo.verticesHaveColor)
					geo.SetAddColor(vc[idx[0]]);
				else
					geo.ResetAddColor();
				geo.AddVertex(v[idx[0]]);
				size_t idx0 = geo.CountVertices() - 1;
				if (geo.verticesHaveNormal)
					geo.SetAddNormal(vn[idx[1]]);
				else
					geo.ResetAddNormal();
				if (geo.verticesHaveColor)
					geo.SetAddColor(vc[idx[1]]);
				else
					geo.ResetAddColor();
				geo.AddVertex(v[idx[1]]);
				size_t idx1 = geo.CountVertices() - 1;
				for (int j = 2; j < N; j++) {
					if (geo.verticesHaveNormal)
						geo.SetAddNormal(vn[idx[j]]);
					else
						geo.ResetAddNormal();
					if (geo.verticesHaveColor)
						geo.SetAddColor(vc[idx[j]]);
					else
						geo.ResetAddColor();
					geo.AddVertex(v[idx[j]]);
					geo.ResetAddNormal();
					size_t idx2 = geo.CountVertices() - 1;
					geo.AddTriangle(idx0, idx1, idx2);

					idx1 = idx2;
				}
			}
			geo.Finish();
			geo.Sort();
			if (!geo.PassedSelfCheck(true)) {
				std::cerr << __FILE__ << " (" << __LINE__ << "): "
						<< "The file seems to contain data that breaks the Finish() method.\n";
			}
		}
	}
}

void FilePLY::ReadStreamBinaryLE(std::istream &stream, Geometry &geo) {
	for (const Element &e : elements) {
		if (e.type == ElementType::VERTEX) {
			std::vector<Geometry::Vertex> vertices;
			vertices.reserve(e.count);
			for (size_t i = 0; i < e.count; i++) {
				Geometry::Vertex vert;
				for (const Property &p : e.properties) {
					if (p.IsList()) {
						VarValue listSize = ReadValue(stream, p.typeListSize);
						uint32_t N = ToUint8(listSize);
						for (uint32_t n = 0; n < N; ++n) {
							VarValue value = ReadValue(stream, p.type);
							// no receivers for lists in the vertices; just eat
							// the values.
						}
					} else {
						VarValue value = ReadValue(stream, p.type);
						switch (p.target) {
						case Target::V_X:
							vert.x = ToDouble(value);
							break;
						case Target::V_Y:
							vert.y = ToDouble(value);
							break;
						case Target::V_Z:
							vert.z = ToDouble(value);
							break;
						case Target::V_NX:
							vert.n.x = ToDouble(value);
							break;
						case Target::V_NY:
							vert.n.y = ToDouble(value);
							break;
						case Target::V_NZ:
							vert.n.z = ToDouble(value);
							break;
						case Target::V_R:
							vert.c.r = ToFloat(value);
							break;
						case Target::V_G:
							vert.c.g = ToFloat(value);
							break;
						case Target::V_B:
							vert.c.b = ToFloat(value);
							break;
						case Target::V_A:
							vert.c.a = ToFloat(value);
							break;
						case Target::V_U:
							vert.u = ToDouble(value);
							break;
						case Target::V_V:
							vert.v = ToDouble(value);
							break;
						case Target::V_GROUP:
							vert.group = ToSize(value);
							break;
						}
					}
				}
				vertices.push_back(vert);
			}
			geo.AddVertex(vertices);

		} else if (e.type == ElementType::EDGE) {
			geo.SetEdgeCount(e.count);
			for (size_t i = 0; i < e.count; i++) {
				Geometry::Edge &ed = geo.GetEdge(i);
				for (const Property &p : e.properties) {
					if (p.IsList()) {
						VarValue listSize = ReadValue(stream, p.typeListSize);
						uint32_t N = ToUint8(listSize);
						for (uint32_t n = 0; n < N; ++n) {
							VarValue value = ReadValue(stream, p.type);
							// no receivers for lists in the edges; just eat
							// the values.
						}
					} else {
						VarValue value = ReadValue(stream, p.type);
						switch (p.target) {
						case Target::E_VA:
							ed.va = ToSize(value);
							break;
						case Target::E_VB:
							ed.vb = ToSize(value);
							break;
						case Target::E_TA:
							ed.ta = ToSize(value);
							break;
						case Target::E_TB:
							ed.tb = ToSize(value);
							break;
						case Target::E_NX:
							ed.n.x = ToDouble(value);
							break;
						case Target::E_NY:
							ed.n.y = ToDouble(value);
							break;
						case Target::E_NZ:
							ed.n.z = ToDouble(value);
							break;
						case Target::E_R:
							ed.c.r = ToFloat(value);
							break;
						case Target::E_G:
							ed.c.g = ToFloat(value);
							break;
						case Target::E_B:
							ed.c.b = ToFloat(value);
							break;
						case Target::E_A:
							ed.c.a = ToFloat(value);
							break;
						case Target::E_GROUP:
							ed.group = ToSize(value);
							break;
						case Target::E_TRIANGLECOUNT:
							ed.trianglecount = ToUint8(value);
							break;
						case Target::E_SHARP:
							ed.sharp = ToBool(value);
							break;
						case Target::E_FLIP:
							ed.flip = ToBool(value);
							break;
						}
					}
				}
			}

		} else if (e.type == ElementType::TRIANGLE) {
			geo.SetTriangleCount(e.count);
			for (size_t i = 0; i < e.count; i++) {
				Geometry::Triangle &tri = geo.GetTriangle(i);
				for (const Property &p : e.properties) {
					if (p.IsList()) {
						VarValue listSize = ReadValue(stream, p.typeListSize);
						uint32_t N = ToUint8(listSize);
						for (uint32_t n = 0; n < N; ++n) {
							VarValue value = ReadValue(stream, p.type);
							if (p.target == Target::T_VERTEXINDEX) {
								switch (n) {
								case 0:
									tri.va = ToSize(value);
									break;
								case 1:
									tri.vb = ToSize(value);
									break;
								case 2:
									tri.vc = ToSize(value);
									break;
								}
							} else if (p.target == Target::T_EDGEINDEX) {
								switch (n) {
								case 0:
									tri.ea = ToSize(value);
									break;
								case 1:
									tri.eb = ToSize(value);
									break;
								case 2:
									tri.ec = ToSize(value);
									break;
								}
							}
						}
					} else {
						VarValue value = ReadValue(stream, p.type);
						switch (p.target) {
						case Target::T_NX:
							tri.n.x = ToDouble(value);
							break;
						case Target::T_NY:
							tri.n.y = ToDouble(value);
							break;
						case Target::T_NZ:
							tri.n.z = ToDouble(value);
							break;
						case Target::T_TX:
							tri.t.x = ToDouble(value);
							break;
						case Target::T_TY:
							tri.t.y = ToDouble(value);
							break;
						case Target::T_TZ:
							tri.t.z = ToDouble(value);
							break;
						case Target::T_BX:
							tri.b.x = ToDouble(value);
							break;
						case Target::T_BY:
							tri.b.y = ToDouble(value);
							break;
						case Target::T_BZ:
							tri.b.z = ToDouble(value);
							break;
						case Target::T_R:
							tri.c.r = ToFloat(value);
							break;
						case Target::T_G:
							tri.c.g = ToFloat(value);
							break;
						case Target::T_B:
							tri.c.b = ToFloat(value);
							break;
						case Target::T_A:
							tri.c.a = ToFloat(value);
							break;
						case Target::T_TUA:
							tri.tua = ToDouble(value);
							break;
						case Target::T_TVA:
							tri.tva = ToDouble(value);
							break;
						case Target::T_TUB:
							tri.tub = ToDouble(value);
							break;
						case Target::T_TVB:
							tri.tvb = ToDouble(value);
							break;
						case Target::T_TUC:
							tri.tuc = ToDouble(value);
							break;
						case Target::T_TVC:
							tri.tvc = ToDouble(value);
							break;
						case Target::T_GROUP:
							tri.group = ToSize(value);
							break;
						case Target::T_FLIP:
							tri.flip = ToBool(value);
							break;
						}
					}
				}
			}
		}
	}
}

void FilePLY::ReadStreamBinaryBE(std::istream &stream, Geometry &geometry) {
	std::ostringstream out;
	out << __FILE__ << ":" << __LINE__ << ": " << __FUNCTION__ << ": ";
	out << "Not implemented.";
	throw std::runtime_error(out.str());
}

void FilePLY::WriteStream(const Geometry &geometry) {

	if (outp == nullptr)
		throw std::logic_error(
				std::string(__FUNCTION__)
						+ " - Missing output stream. Check the construction of this object.");
	if (!outp->good())
		throw std::runtime_error(
				std::string(__FUNCTION__) + "Output stream is not good.");

	*outp << "ply\n";
	*outp << "format binary_little_endian 1.0\n";

	*outp << "element vertex " << geometry.CountVertices() << '\n';
	*outp << "property double x\n";
	*outp << "property double y\n";
	*outp << "property double z\n";
	if (geometry.verticesHaveNormal) {
		*outp << "property double nx\n";
		*outp << "property double ny\n";
		*outp << "property double nz\n";
	}
	if (geometry.verticesHaveColor) {
		*outp << "property float red\n";
		*outp << "property float green\n";
		*outp << "property float blue\n";
		*outp << "property float alpha\n";
	}
	if (geometry.verticesHaveTexture) {
		*outp << "property double s\n";
		*outp << "property double t\n";
	}
	*outp << "property uint group\n";

	*outp << "element edge " << geometry.CountEdges() << '\n';
	if (meshlabCompatibilityMode) {
		*outp << "property int vertex1\n";
		*outp << "property int vertex2\n";
	} else {
		*outp << "property uint vertex1\n";
		*outp << "property uint vertex2\n";
	}
	*outp << "property uint triangle1\n";
	*outp << "property uint triangle2\n";

	if (geometry.edgesHaveNormal) {
		*outp << "property double nx\n";
		*outp << "property double ny\n";
		*outp << "property double nz\n";
	}
	if (geometry.edgesHaveColor) {
		*outp << "property float red\n";
		*outp << "property float green\n";
		*outp << "property float blue\n";
		*outp << "property float alpha\n";
	}
	*outp << "property uint group\n";
	*outp << "property uchar triangle_count\n";
	*outp << "property char sharp\n";
	*outp << "property char flip\n";

	*outp << "element face " << geometry.CountTriangles() << '\n';
	*outp << "property list uchar uint vertex_indices\n";
	*outp << "property list uchar uint edge_indices\n";

	if (geometry.trianglesHaveNormal) {
		*outp << "property double nx\n";
		*outp << "property double ny\n";
		*outp << "property double nz\n";
		*outp << "property double tx\n";
		*outp << "property double ty\n";
		*outp << "property double tz\n";
		*outp << "property double bx\n";
		*outp << "property double by\n";
		*outp << "property double bz\n";
	}
	if (geometry.trianglesHaveColor) {
		*outp << "property float red\n";
		*outp << "property float green\n";
		*outp << "property float blue\n";
		*outp << "property float alpha\n";
	}
	if (geometry.trianglesHaveTexture) {
		*outp << "property double tau\n";
		*outp << "property double tav\n";
		*outp << "property double tbu\n";
		*outp << "property double tbv\n";
		*outp << "property double tcu\n";
		*outp << "property double tcv\n";
	}
	*outp << "property uint group\n";
	*outp << "property char flip\n";
	*outp << "end_header\n";

//	std::fwrite(header.c_str(), sizeof(char), header.size(), fhd);
	for (size_t n = 0; n < geometry.CountVertices(); n++) {
		const Geometry::Vertex &v = geometry.GetVertex(n);

		outp->write(reinterpret_cast<const char*>(&(v.x)), sizeof(v.x));
		outp->write(reinterpret_cast<const char*>(&(v.y)), sizeof(v.y));
		outp->write(reinterpret_cast<const char*>(&(v.z)), sizeof(v.z));
		if (geometry.verticesHaveNormal) {
			outp->write(reinterpret_cast<const char*>(&(v.n.x)), sizeof(v.n.x));
			outp->write(reinterpret_cast<const char*>(&(v.n.y)), sizeof(v.n.y));
			outp->write(reinterpret_cast<const char*>(&(v.n.z)), sizeof(v.n.z));
		}
		if (geometry.verticesHaveColor) {
			outp->write(reinterpret_cast<const char*>(&(v.c.r)), sizeof(v.c.r));
			outp->write(reinterpret_cast<const char*>(&(v.c.g)), sizeof(v.c.g));
			outp->write(reinterpret_cast<const char*>(&(v.c.b)), sizeof(v.c.b));
			outp->write(reinterpret_cast<const char*>(&(v.c.a)), sizeof(v.c.a));
		}
		if (geometry.verticesHaveTexture) {
			outp->write(reinterpret_cast<const char*>(&(v.u)), sizeof(v.u));
			outp->write(reinterpret_cast<const char*>(&(v.v)), sizeof(v.v));
		}
		uint32_t group = v.group;
		outp->write(reinterpret_cast<char*>(&(group)), sizeof(group));
	}

	for (size_t n = 0; n < geometry.CountEdges(); n++) {
		const Geometry::Edge &e = geometry.GetEdge(n);

		if (meshlabCompatibilityMode) {
			uint32_t vertex1 = e.va;
			uint32_t vertex2 = e.vb;
			outp->write(reinterpret_cast<char*>(&(vertex1)), sizeof(vertex1));
			outp->write(reinterpret_cast<char*>(&(vertex2)), sizeof(vertex2));
		} else {
			uint32_t vertex1 = e.va;
			uint32_t vertex2 = e.vb;
			outp->write(reinterpret_cast<char*>(&(vertex1)), sizeof(vertex1));
			outp->write(reinterpret_cast<char*>(&(vertex2)), sizeof(vertex2));
		}
		{
			uint32_t triangle1 = e.ta;
			uint32_t triangle2 = e.tb;
			outp->write(reinterpret_cast<char*>(&(triangle1)),
					sizeof(triangle1));
			outp->write(reinterpret_cast<char*>(&(triangle2)),
					sizeof(triangle2));
		}
		if (geometry.edgesHaveNormal) {
			outp->write(reinterpret_cast<const char*>(&(e.n.x)), sizeof(e.n.x));
			outp->write(reinterpret_cast<const char*>(&(e.n.y)), sizeof(e.n.y));
			outp->write(reinterpret_cast<const char*>(&(e.n.z)), sizeof(e.n.z));
		}
		if (geometry.edgesHaveColor) {
			outp->write(reinterpret_cast<const char*>(&(e.c.r)), sizeof(e.c.r));
			outp->write(reinterpret_cast<const char*>(&(e.c.g)), sizeof(e.c.g));
			outp->write(reinterpret_cast<const char*>(&(e.c.b)), sizeof(e.c.b));
			outp->write(reinterpret_cast<const char*>(&(e.c.a)), sizeof(e.c.a));
		}

		uint32_t group = e.group;
		outp->write(reinterpret_cast<char*>(&(group)), sizeof(group));

		uint8_t triangleCount = e.trianglecount;
		outp->write(reinterpret_cast<char*>(&(triangleCount)),
				sizeof(triangleCount));

		int8_t sharp = e.sharp ? 1 : 0;
		outp->write(reinterpret_cast<char*>(&(sharp)), sizeof(sharp));

		int8_t flip = e.flip ? 1 : 0;
		outp->write(reinterpret_cast<char*>(&(flip)), sizeof(flip));
	}
	const uint8_t vertex_count = 3;
	const uint8_t edge_count = 3;

	for (size_t n = 0; n < geometry.CountTriangles(); n++) {
		const Geometry::Triangle &t = geometry.GetTriangle(n);

		outp->write(reinterpret_cast<const char*>(&(vertex_count)),
				sizeof(vertex_count));

		uint32_t vertex1 = t.va;
		uint32_t vertex2 = t.vb;
		uint32_t vertex3 = t.vc;
		outp->write(reinterpret_cast<char*>(&(vertex1)), sizeof(vertex1));
		outp->write(reinterpret_cast<char*>(&(vertex2)), sizeof(vertex2));
		outp->write(reinterpret_cast<char*>(&(vertex3)), sizeof(vertex3));

		outp->write(reinterpret_cast<const char*>(&(edge_count)),
				sizeof(edge_count));

		uint32_t edge1 = t.ea;
		uint32_t edge2 = t.eb;
		uint32_t edge3 = t.ec;
		outp->write(reinterpret_cast<char*>(&(edge1)), sizeof(edge1));
		outp->write(reinterpret_cast<char*>(&(edge2)), sizeof(edge2));
		outp->write(reinterpret_cast<char*>(&(edge3)), sizeof(edge3));

		if (geometry.verticesHaveNormal) {
			outp->write(reinterpret_cast<const char*>(&(t.n.x)), sizeof(t.n.x));
			outp->write(reinterpret_cast<const char*>(&(t.n.y)), sizeof(t.n.y));
			outp->write(reinterpret_cast<const char*>(&(t.n.z)), sizeof(t.n.z));
			outp->write(reinterpret_cast<const char*>(&(t.t.x)), sizeof(t.t.x));
			outp->write(reinterpret_cast<const char*>(&(t.t.y)), sizeof(t.t.y));
			outp->write(reinterpret_cast<const char*>(&(t.t.z)), sizeof(t.t.z));
			outp->write(reinterpret_cast<const char*>(&(t.b.x)), sizeof(t.b.x));
			outp->write(reinterpret_cast<const char*>(&(t.b.y)), sizeof(t.b.y));
			outp->write(reinterpret_cast<const char*>(&(t.b.z)), sizeof(t.b.z));
		}
		if (geometry.verticesHaveColor) {
			outp->write(reinterpret_cast<const char*>(&(t.c.r)), sizeof(t.c.r));
			outp->write(reinterpret_cast<const char*>(&(t.c.g)), sizeof(t.c.g));
			outp->write(reinterpret_cast<const char*>(&(t.c.b)), sizeof(t.c.b));
			outp->write(reinterpret_cast<const char*>(&(t.c.a)), sizeof(t.c.a));
		}
		if (geometry.verticesHaveTexture) {
			outp->write(reinterpret_cast<const char*>(&(t.tua)), sizeof(t.tua));
			outp->write(reinterpret_cast<const char*>(&(t.tva)), sizeof(t.tva));
			outp->write(reinterpret_cast<const char*>(&(t.tub)), sizeof(t.tub));
			outp->write(reinterpret_cast<const char*>(&(t.tvb)), sizeof(t.tvb));
			outp->write(reinterpret_cast<const char*>(&(t.tuc)), sizeof(t.tuc));
			outp->write(reinterpret_cast<const char*>(&(t.tvc)), sizeof(t.tvc));
		}
		uint32_t group = t.group;
		outp->write(reinterpret_cast<char*>(&(group)), sizeof(group));

		int8_t flip = t.flip ? 1 : 0;
		outp->write(reinterpret_cast<char*>(&(flip)), sizeof(flip));
	}
}

FilePLY::DataType FilePLY::StringToType(const std::string &name) const {
	if (name == "char" || name == "int8")
		return DataType::INT8;
	if (name == "uchar" || name == "uint8")
		return DataType::UINT8;
	if (name == "short" || name == "int16")
		return DataType::INT16;
	if (name == "ushort" || name == "uint16")
		return DataType::UINT16;
	if (name == "int" || name == "int32")
		return DataType::INT32;
	if (name == "uint" || name == "uint32")
		return DataType::UINT32;
	if (name == "float" || name == "float32")
		return DataType::FLOAT32;
	if (name == "double" || name == "float64")
		return DataType::DOUBLE64;
	throw std::runtime_error(
			"FilePLY::StringToType : Unknown datatype found in file: " + name
					+ ".");
}

FilePLY::VarValue FilePLY::ReadValue(std::istream &stream, DataType datatype) {
	FilePLY::VarValue ret;
	switch (datatype) {
	case DataType::INT8: {
		int8_t temp;
		stream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		ret = temp;
		break;
	}
	case DataType::UINT8: {
		uint8_t temp;
		stream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		ret = temp;
		break;
	}
	case DataType::INT16: {
		int16_t temp;
		stream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		ret = temp;
		break;
	}
	case DataType::UINT16: {
		uint16_t temp;
		stream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		ret = temp;
		break;
	}
	case DataType::INT32: {
		int32_t temp;
		stream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		ret = temp;
		break;
	}
	case DataType::UINT32: {
		uint32_t temp;
		stream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		ret = temp;
		break;
	}
	case DataType::FLOAT32: {
		float temp;
		stream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		ret = temp;
		break;
	}
	case DataType::DOUBLE64: {
		double temp;
		stream.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		ret = temp;
		break;
	}
	default:
		throw std::runtime_error(
				"FilePLY::ReadValue - Unhandled datatype while reading.");
	}
	return ret;
}

bool FilePLY::ToBool(const VarValue value) {
	switch ((DataType) value.index()) {
	case DataType::INT8:
		return std::get<int8_t>(value) != 0;
	case DataType::UINT8:
		return std::get<uint8_t>(value) != 0;
	case DataType::INT16:
		return std::get<int16_t>(value) != 0;
	case DataType::UINT16:
		return std::get<uint16_t>(value) != 0;
	case DataType::INT32:
		return std::get<int32_t>(value) != 0;
	case DataType::UINT32:
		return std::get<uint32_t>(value) != 0;
	case DataType::FLOAT32:
		return std::get<float>(value) >= 0.5;
	case DataType::DOUBLE64:
		return std::get<double>(value) >= 0.5;
	default:
		return false;
	}
}

size_t FilePLY::ToSize(const VarValue value) {
	switch ((DataType) value.index()) {
	case DataType::INT8: {
		const int8_t value_int8 = std::get<int8_t>(value);
		if (value_int8 < 0)
			return (size_t) -1;
		return value_int8;
	}
	case DataType::UINT8: {
		const uint8_t value_uint8 = std::get<uint8_t>(value);
		if (value_uint8 == (uint8_t) -1)
			return (size_t) -1;
		return value_uint8;
	}
	case DataType::INT16: {
		const int16_t value_int16 = std::get<int16_t>(value);
		if (value_int16 <= 0)
			return (size_t) -1;
		return value_int16;
	}
	case DataType::UINT16: {
		const uint16_t value_uint16 = std::get<uint16_t>(value);
		if (value_uint16 == (uint16_t) -1)
			return (size_t) -1;
		return value_uint16;
	}
	case DataType::INT32: {
		const int32_t value_int32 = std::get<int32_t>(value);
		if (value_int32 < 0)
			return (size_t) -1;
		return value_int32;
	}
	case DataType::UINT32: {
		const uint32_t value_uint32 = std::get<uint32_t>(value);
		if (value_uint32 == (uint32_t) -1)
			return (size_t) -1;
		return value_uint32;
	}
	case DataType::FLOAT32: {
		return std::round(std::get<float>(value));
	}
	case DataType::DOUBLE64: {
		return std::round(std::get<double>(value));
	}
	default:
		return 0;
	}
}

uint8_t FilePLY::ToUint8(const VarValue value) {
	switch ((DataType) value.index()) {
	case DataType::INT8:
		return std::get<int8_t>(value);
	case DataType::UINT8:
		return std::get<uint8_t>(value);
	case DataType::INT16:
		return std::get<int16_t>(value);
	case DataType::UINT16:
		return std::get<uint16_t>(value);
	case DataType::INT32:
		return std::get<int32_t>(value);
	case DataType::UINT32:
		return std::get<uint32_t>(value);
	case DataType::FLOAT32:
		return round(std::get<float>(value));
	case DataType::DOUBLE64:
		return round(std::get<double>(value));
	default:
		return 0;
	}
}

float FilePLY::ToFloat(const VarValue value) {
	switch ((DataType) value.index()) {
	case DataType::INT8:
		return std::get<int8_t>(value);
	case DataType::UINT8:
		return std::get<uint8_t>(value);
	case DataType::INT16:
		return std::get<int16_t>(value);
	case DataType::UINT16:
		return std::get<uint16_t>(value);
	case DataType::INT32:
		return std::get<int32_t>(value);
	case DataType::UINT32:
		return std::get<uint32_t>(value);
	case DataType::FLOAT32:
		return std::get<float>(value);
	case DataType::DOUBLE64:
		return std::get<double>(value);
	default:
		return 0.0f;
	}
}

double FilePLY::ToDouble(const VarValue value) {
	switch ((DataType) value.index()) {
	case DataType::INT8:
		return std::get<int8_t>(value);
	case DataType::UINT8:
		return std::get<uint8_t>(value);
	case DataType::INT16:
		return std::get<int16_t>(value);
	case DataType::UINT16:
		return std::get<uint16_t>(value);
	case DataType::INT32:
		return std::get<int32_t>(value);
	case DataType::UINT32:
		return std::get<uint32_t>(value);
	case DataType::FLOAT32:
		return std::get<float>(value);
	case DataType::DOUBLE64:
		return std::get<double>(value);
	default:
		return 0.0;
	}
}
