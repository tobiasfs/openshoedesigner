///////////////////////////////////////////////////////////////////////////////
// Name               : FileSTL.cpp
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

#include "FileSTL.h"

#include <fstream>
#include <stdint.h>

// Verify, that float is 32 bit on this architecture.
#define __ASSERT(e) typedef char __ASSERT__[(e)?1:-1]
__ASSERT(sizeof(float) == sizeof(uint32_t));

FileSTL::FileSTL(const std::string &filename_) :
		FileGeometry(filename_) {
}

FileSTL::FileSTL(std::istream *stream) :
		FileGeometry(stream) {
}

void FileSTL::ReadStream(Geometry &geo) {
	// Find the header of the STL file:
	std::string header(5, '\0');

	if (!inp->read(&header[0], 5)) {
		throw std::runtime_error(
				"STL File " + filename + ": File contains no header.");
	}
	if (header.compare("solid") == 0) {
		// Make sure, that number are written with a dot and not a comma.
		setlocale(LC_ALL, "C");
		try {
			ReadStreamAscii(*inp, header, geo);
		} catch (std::exception &ex) {
			setlocale(LC_ALL, "");
			throw(ex);
		}
		setlocale(LC_ALL, "");
	} else {
		ReadStreamBinary(*inp, header, geo);
	}

}

void FileSTL::ReadStreamAscii(std::istream &stream, std::string &header,
		Geometry &geo) {

	std::string name;
	std::getline(stream, name);
	header += name;

	std::string word = header.substr(0, 5);
	if (word.compare("solid") != 0)
		throw std::runtime_error(
				"STL Text File " + filename + ": Incorrect file.");

	std::array<float, 3> normal;
	std::array<float, 9> coord;
	bool solidcomplete = true;

	while (stream.good() && word.substr(0, 5).compare("solid") == 0) {
		// Set up a new geometry object.
		solidcomplete = false;
		geo.name = StringTrim(name);

		stream >> word;
		while (word.compare("facet") == 0) {
			stream >> word;

			if (word.compare("normal") == 0) {
				stream >> normal[0] >> normal[1] >> normal[2];
				geo.SetAddNormal(normal[0], normal[1], normal[2]);
			} else {
				geo.ResetAddNormal();
			}
			if (!stream.good())
				break;

			stream >> word;

			if (word.compare("outer") != 0)
				throw std::runtime_error(
						"STL Text File " + filename + ": 'outer' missing.");

			stream >> word;
			if (word.compare("loop") != 0)
				throw std::runtime_error(
						"STL Text File " + filename + ": 'loop' missing.");

			for (uint_fast8_t m = 0; m < 3; m++) {
				stream >> word;
				if (word.compare("vertex") != 0)
					throw std::runtime_error(
							"STL Text File " + filename
									+ ": 'vertex' missing.");

				*inp >> coord[m * 3 + 0] >> coord[m * 3 + 1]
						>> coord[m * 3 + 2];
			}
			stream >> word;
			if (word.compare("endloop") != 0)
				throw std::runtime_error(
						"STL Text File " + filename + ": 'endloop' missing.");

			stream >> word;
			if (word.compare("endfacet") != 0)
				throw std::runtime_error(
						"STL Text File " + filename + ": 'endfacet' missing.");

			geo.AddTriangle( { coord[0], coord[1], coord[2] }, { coord[3],
					coord[4], coord[5] }, { coord[6], coord[7], coord[8] });

//			for (uint_fast8_t j = 0; j < 3; j++) {
//
//				geo.AddTriangle();
//
//				tri.p[j].x = coord[0 + j * 3];
//				tri.p[j].y = coord[1 + j * 3];
//				tri.p[j].z = coord[2 + j * 3];
//				tri.n[j].x = normal[0];
//				tri.n[j].y = normal[1];
//				tri.n[j].z = normal[2];
//				tri.c[j] = tc.colorNewObjects;
//			}

// The normal vectors seem to be defect for some files.
// if(false)... = Calculate normals
// if(true)...  = Use normals from file
//			tc.AddTriangle(tri, false);
//			if(false){
//				geometry[n].AddTriangleWithNormals(tri.p[0], tri.p[1], tri.p[2],
//						tri.n[0], tri.n[0], tri.n[0]);
//			}else{
//				geometry[n].AddTriangle(tri.p[0], tri.p[1], tri.p[2]);
//			}

			stream >> word;
		}
		if (word.compare("endsolid") != 0)
			throw std::runtime_error(
					"STL Text File " + filename + ": 'endsolid' missing.");
		geo.Finish();
		solidcomplete = true;
		//TODO Enable AND TEST the line below. The name of the solid should follow the "endsolid" statement.
		//std::getline(stream, word);
		stream >> word;
	}
	if (!solidcomplete)
		throw std::runtime_error(
				"STL Text File " + filename
						+ ": There was an error in the data while reading a solid from the file. Maybe the file was truncated.");
}

void FileSTL::ReadStreamBinary(std::istream &stream, std::string &header,
		Geometry &geo) {
	// NOTE: When changing to std::istream, check https://stackoverflow.com/questions/5605125/why-is-iostreameof-inside-a-loop-condition-i-e-while-stream-eof-cons

	const size_t bytestoread = 80 - header.size();
	{
		std::string temp(bytestoread, '\0');
		if (!stream.read(&temp[0], bytestoread) || stream.eof()) {
			throw std::runtime_error(
					"STL File " + filename + ": File contains no header.");
		}
		header += temp;
	}

	uint32_t nrOfTriangles;
	if (!stream.read(reinterpret_cast<char*>(&nrOfTriangles),
			sizeof nrOfTriangles) || stream.eof())
		throw std::runtime_error(
				"STL File " + filename + ": File to short. Unexpected EOF.");

	std::array<float, 12> coord;
	uint16_t attribute;
	for (size_t i = 0; i < nrOfTriangles; i++) {
		if (!stream.read(reinterpret_cast<char*>(&(coord[0])),
				coord.size() * sizeof(float)) || stream.eof())
			throw std::runtime_error(
					"STL File " + filename
							+ ": File to short. Unexpected EOF.");

		if (!stream.read(reinterpret_cast<char*>(&attribute), sizeof attribute)
				|| stream.eof())
			throw std::runtime_error(
					"STL File " + filename
							+ ": File to short. Unexpected EOF.");

		if (attribute & (1 << 15)) {
			Vector3 newColor;
			newColor.x = (float) ((attribute >> 0) & 31) / 31.0f;
			newColor.y = (float) ((attribute >> 5) & 31) / 31.0f;
			newColor.z = (float) ((attribute >> 10) & 31) / 31.0f;
			geo.SetAddColor(newColor);
		} else {
			geo.ResetAddColor();
		}

		geo.SetAddNormal(coord[0], coord[1], coord[2]);

		geo.AddTriangle( { coord[3], coord[4], coord[5] }, { coord[6], coord[7],
				coord[8] }, { coord[9], coord[10], coord[11] });

//		for (uint_fast8_t j = 0; j < 3; j++) {
//			tri.p[j].x = coord[3 + j * 3];
//			tri.p[j].y = coord[4 + j * 3];
//			tri.p[j].z = coord[5 + j * 3];
//			tri.n[j].x = coord[0];
//			tri.n[j].y = coord[1];
//			tri.n[j].z = coord[2];
//			tri.c[j] = color;
//		}
//		tc.AddTriangle(tri, false);
	}
	geo.Finish();
	// The normal vectors seem to be defect for some files.
	// if(false)... = Calculate normals
	// if(true)...  = Use normals from file
//		if(false){
//			geometry[nGeometry].AddTriangleWithNormals(tri.p[0], tri.p[1],
//					tri.p[2], tri.n[0], tri.n[0], tri.n[0]);
//		}else{
//			geometry[nGeometry].AddTriangle(tri.p[0], tri.p[1], tri.p[2]);
//		}

	//			if(i <= 1){
	//				wxLogMessage(wxString::Format(_T("n: %.1f %.1f %.1f"),
	//						tri->n.x, tri->n.y, tri->n.z));
	//				for(j = 0; j < 3; j++)
	//
	//					wxLogMessage(wxString::Format(_T("p: %.1f %.1f %.1f"),
	//							tri->p[j].x, tri->p[j].y, tri->p[j].z));
	//			}
}

void FileSTL::WriteStream(const Geometry &geo) {
	uint8_t header[81] =
			"Generated by FileSTL::WriteStream                                               ";
	outp->write(reinterpret_cast<char*>(&header), 80);

//	uint32_t numTriangles = 0;
//	for (const auto &tc : trianglecollections)
//		numTriangles += tc.Size();
//	for (const auto &h : hulls)
//		numTriangles += h.GetTriangleCount();
//
//	outp->write(reinterpret_cast<char*>(&numTriangles), sizeof(numTriangles));
//
//	for (const auto &tc : trianglecollections) {
//		for (const auto &tri : tc.triangles) {
//			float coord[12];
//			coord[0] = tri.n[0].x;
//			coord[1] = tri.n[0].y;
//			coord[2] = tri.n[0].z;
//			coord[3] = tri.p[0].x;
//			coord[4] = tri.p[0].y;
//			coord[5] = tri.p[0].z;
//			coord[6] = tri.p[1].x;
//			coord[7] = tri.p[1].y;
//			coord[8] = tri.p[1].z;
//			coord[9] = tri.p[2].x;
//			coord[10] = tri.p[2].y;
//			coord[11] = tri.p[2].z;
//			outp->write(reinterpret_cast<char*>(&coord), sizeof(coord));
//			uint16_t col = (1 << 15); // bit 15 set = color valid
//			col |= ((uint16_t) (floor(tri.c[0].z * 32.0))) & 31;
//			col |= (((uint16_t) (floor(tri.c[0].y * 32.0))) & 31) << 5;
//			col |= (((uint16_t) (floor(tri.c[0].x * 32.0))) & 31) << 10;
//			outp->write(reinterpret_cast<char*>(&col), sizeof(col));
//		}
//		for (const auto &h : hulls) {
//			for (size_t n = 0; n < h.GetTriangleCount(); ++n) {
//				std::array<float, 12> coord;
//				uint16_t col = h.FillSTLVector(n, coord);
//				outp->write(reinterpret_cast<char*>(coord.data()),
//						coord.size() * sizeof(float));
//				outp->write(reinterpret_cast<char*>(&col), sizeof(col));
//			}
//		}
//	}
}
