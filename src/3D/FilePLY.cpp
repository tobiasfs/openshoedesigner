///////////////////////////////////////////////////////////////////////////////
// Name               : FilePLY.cpp
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

#include "FilePLY.h"

#include <fstream>
#include <istream>
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

	ReadHeader(*inp);

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

void FilePLY::ReadHeader(std::istream &stream) {

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
	std::vector<Vector3> v;
	std::vector<Vector3> vn;
	std::vector<Vector3> vc;
	bool hasNormals = false; //TODO Speedup
	bool hasColors = false;
	for (const Element &e : elements) {
		if (e.name == "vertex" || e.name == "vertices") {
			v.resize(e.count);
			vn.resize(e.count);
			vc.resize(e.count);
			for (size_t i = 0; i < e.count; ++i) {
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

					if (p.name == "x") {
						v[i].x = val_float;
					} else if (p.name == "y") {
						v[i].y = val_float;
					} else if (p.name == "z") {
						v[i].z = val_float;
					} else if (p.name == "nx") {
						vn[i].x = val_float;
						hasNormals = true;
					} else if (p.name == "ny") {
						vn[i].y = val_float;
					} else if (p.name == "nz") {
						vn[i].z = val_float;
					} else if (p.name == "red") {
						vc[i].x = val_float;
						hasColors = true;
					} else if (p.name == "green") {
						vc[i].y = val_float;
					} else if (p.name == "blue") {
						vc[i].z = val_float;
					}
				}
			}
		}
		if (e.name == "face" || e.name == "faces") {
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

			for (size_t i = 0; i < e.count; ++i) {
				int N;
				stream >> N;
				std::vector<int> idx(N, 0);
				for (int j = 0; j < N; ++j)
					stream >> idx[j];

				if (hasNormals)
					geo.SetAddNormal(vn[idx[0]]);
				else
					geo.ResetAddNormal();
				if (hasColors)
					geo.SetAddColor(vc[idx[0]]);
				else
					geo.ResetAddColor();
				geo.AddVertex(v[idx[0]]);
				size_t idx0 = geo.CountVertices() - 1;
				if (hasNormals)
					geo.SetAddNormal(vn[idx[1]]);
				else
					geo.ResetAddNormal();
				if (hasColors)
					geo.SetAddColor(vc[idx[1]]);
				else
					geo.ResetAddColor();
				geo.AddVertex(v[idx[1]]);
				size_t idx1 = geo.CountVertices() - 1;
				for (int j = 2; j < N; ++j) {
					if (hasNormals)
						geo.SetAddNormal(vn[idx[j]]);
					else
						geo.ResetAddNormal();
					if (hasColors)
						geo.SetAddColor(vc[idx[j]]);
					else
						geo.ResetAddColor();
					geo.AddVertex(v[idx[j]]);
					geo.ResetAddNormal();
					size_t idx2 = geo.CountVertices() - 1;
					geo.AddTriangle(idx0, idx1, idx2);

					idx1 = idx2;

//					tc.AddTriangleWithNormals(v[idx[0]], v[idx[j - 1]],
//							v[idx[j]], , vn[idx[j - 1]],
//							vn[idx[j]]);
//					else
//					tc.AddTriangle(v[idx[0]], v[idx[j - 1]], v[idx[j]]);
//					if (hasColors) {
//						size_t k = tc.Size() - 1;
//						tc.triangles[k].c[0] = vc[idx[0]];
//						tc.triangles[k].c[1] = vc[idx[j - 1]];
//						tc.triangles[k].c[2] = vc[idx[j]];
//					} else {
//						geo.ResetAddColor();
//					}
				}
//				if (hasColors)
//					tc.useColor = TriangleCollection::Style::Vertex;
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

	std::vector<Vector3> v;
	std::vector<Vector3> vn;
	std::vector<Vector3> vc;

	std::vector<float> values_float;

	bool hasNormals = false; //TODO Speedup
	bool hasColors = false;

	for (const Element &e : elements) {
		if (e.name == "vertex" || e.name == "vertices") {
			v.resize(e.count);
			vn.resize(e.count);
			vc.resize(e.count);

			// Check if all properties are FLOAT_32
			size_t fieldsPerRecord = 0;
			for (const Property &p : e.properties) {
				if (p.IsList())
					throw std::runtime_error(
							"FilePLY::ReadStreamBinaryLE - A list is not expected in the vertex definition.");
				if (p.type != DataType::FLOAT32)
					throw std::runtime_error(
							"FilePLY::ReadStreamBinaryLE - Not all elements of a vector are floats.");
				fieldsPerRecord++;
			}

			// Bulk-read all vectors.
			values_float.resize(e.count * fieldsPerRecord);
			stream.read((char*) values_float.data(),
					values_float.size() * sizeof(float));

			size_t c = 0;
			for (size_t i = 0; i < e.count; ++i) {
				for (const Property &p : e.properties) {
					if (p.name == "x") {
						v[i].x = values_float[c];
					} else if (p.name == "y") {
						v[i].y = values_float[c];
					} else if (p.name == "z") {
						v[i].z = values_float[c];
					} else if (p.name == "nx") {
						vn[i].x = values_float[c];
						hasNormals = true;
					} else if (p.name == "ny") {
						vn[i].y = values_float[c];
					} else if (p.name == "nz") {
						vn[i].z = values_float[c];
					} else if (p.name == "red") {
						vc[i].x = values_float[c];
						hasColors = true;
					} else if (p.name == "green") {
						vc[i].y = values_float[c];
					} else if (p.name == "blue") {
						vc[i].z = values_float[c];
					}
					c++;
				}
			}
		}

		if (e.name == "face" || e.name == "faces") {
			if (e.properties.size() != 1)
				throw std::runtime_error(
						"FilePLY::ReadStreamBinaryLE - Expected exactly one property for '"
								+ e.name + "'.");
			auto p = e.properties[0];
			if (p.name != "vertex_index" && p.name != "vertex_indices")
				throw std::runtime_error(
						"FilePLY::ReadStreamBinaryLE - Expected the name 'vertex_index' or 'vertex_indices' but got '"
								+ p.name + "'.");
			if (!p.IsList())
				throw std::runtime_error(
						"FilePLY::ReadStreamBinaryLE - Expected a list.");
			if (p.typeListSize == DataType::FLOAT32
					|| p.typeListSize == DataType::DOUBLE64)
				throw std::runtime_error(
						"FilePLY::ReadStreamBinaryLE - The list size has to be an integer.");
			if (p.type == DataType::FLOAT32 || p.type == DataType::DOUBLE64)
				throw std::runtime_error(
						"FilePLY::ReadStreamBinaryLE - The index in the list has to be an integer.");

			if (p.typeListSize != DataType::UINT8)
				throw std::runtime_error(
						"FilePLY::ReadStreamBinaryLE - Expected uint8 for the ListSize entry (in current implementation).");

			std::vector<uint32_t> idx;
			for (size_t i = 0; i < e.count; ++i) {
				uint8_t N;
				stream.read((char*) &N, sizeof(uint8_t));
				idx.resize(N);
				switch (p.type) {
				case DataType::UINT32:
					stream.read((char*) idx.data(), N * sizeof(uint32_t));
					break;
				case DataType::INT32:
					// FIXME: Read the data as int32 and check&cast.
					stream.read((char*) idx.data(), N * sizeof(uint32_t));
					break;
				default:
					throw std::runtime_error(
							"FilePLY::ReadStreamBinaryLE - Expected uint32 for the vertex-index entry (in current implementation).");
				}

				if (hasNormals)
					geo.SetAddNormal(vn[idx[0]]);
				else
					geo.ResetAddNormal();
				if (hasColors)
					geo.SetAddColor(vc[idx[0]]);
				else
					geo.ResetAddColor();
				geo.AddVertex(v[idx[0]]);
				size_t idx0 = geo.CountVertices() - 1;
				if (hasNormals)
					geo.SetAddNormal(vn[idx[1]]);
				else
					geo.ResetAddNormal();
				if (hasColors)
					geo.SetAddColor(vc[idx[1]]);
				else
					geo.ResetAddColor();
				geo.AddVertex(v[idx[1]]);
				size_t idx1 = geo.CountVertices() - 1;
				for (int j = 2; j < N; ++j) {
					if (hasNormals)
						geo.SetAddNormal(vn[idx[j]]);
					else
						geo.ResetAddNormal();
					if (hasColors)
						geo.SetAddColor(vc[idx[j]]);
					else
						geo.ResetAddColor();
					geo.AddVertex(v[idx[j]]);
					geo.ResetAddNormal();
					size_t idx2 = geo.CountVertices() - 1;
					geo.AddTriangle(idx0, idx1, idx2);
					idx1 = idx2;

					//					tc.AddTriangleWithNormals(v[idx[0]], v[idx[j - 1]],
					//							v[idx[j]], , vn[idx[j - 1]],
					//							vn[idx[j]]);
					//					else
					//					tc.AddTriangle(v[idx[0]], v[idx[j - 1]], v[idx[j]]);
					//					if (hasColors) {
					//						size_t k = tc.Size() - 1;
					//						tc.triangles[k].c[0] = vc[idx[0]];
					//						tc.triangles[k].c[1] = vc[idx[j - 1]];
					//						tc.triangles[k].c[2] = vc[idx[j]];
					//					} else {
					//						geo.ResetAddColor();
					//					}
				}
				//				if (hasColors)
				//					tc.useColor = TriangleCollection::Style::Vertex;
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

void FilePLY::ReadStreamBinaryBE(std::istream &stream, Geometry &geometry) {
	throw std::runtime_error("FilePLY::ReadStreamBinaryBE - Not implemented.");
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
