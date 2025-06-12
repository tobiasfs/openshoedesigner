///////////////////////////////////////////////////////////////////////////////
// Name               : FileOBJ.cpp
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

#include "FileOBJ.h"

#include <fstream>

FileOBJ::FileOBJ(const std::string &filename_) :
		FileGeometry(filename_) {
}

int FileOBJ::ExtractNumbers(std::vector<float> &v, int v_width) {
	static const std::string numchars("+-.ABCDEFINTXYabcdefintxy0123456789");
	std::string line;
	std::getline(*inp, line);
	int numberCount = 0;
	auto posStart = line.find_first_of(numchars, 0);
	while (posStart != std::string::npos) {
		const auto posEnd = line.find_first_not_of(numchars, posStart);
		const auto count =
				(posEnd == std::string::npos) ?
						(line.length() - posStart) : (posEnd - posStart);
		const auto numberstring = line.substr(posStart, count);
		v.push_back(std::strtof(numberstring.c_str(), nullptr));
		numberCount++;
		posStart = line.find_first_of(numchars, posEnd);
	}

	if (v_width < 0) {
		v_width = numberCount;
	} else {
		if (numberCount != v_width) {
			throw std::runtime_error(
					"FileOBJ::ReadStream : The count of numbers is changing for one type in the OBJ file.");
		}
	}
	return v_width;
}

std::string FileOBJ::NextToken() {
	std::string result;
	int c = inp->peek();
	while (c != '\n') {
		if (c == '/') {
			inp->get();
			return std::string("/");
		}
		if ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.') {
			while ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.') {
				result += inp->get();
				c = inp->peek();
			}
			return result;
		}
		inp->get();
		c = inp->peek();
	}
	return result;
}

void FileOBJ::ReadStream(Geometry &geo) {

	int v_width = -1;
	size_t v_offs = 0;
	std::vector<float> v;
	int vt_width = -1;
	size_t vt_offs = 0;
	std::vector<float> vt;
	int vn_width = -1;
	size_t vn_offs = 0;
	std::vector<float> vn;
	int vp_width = -1;
	size_t vp_offs = 0;
	std::vector<float> vp;

	std::string word;
	*inp >> std::ws;
	*inp >> word;
	while (inp->good()) {
		if (word.empty())
			throw std::runtime_error("This should not happen.");
		if (word[0] == '#') {
			// Move to line-end
			int c = inp->get();
			while (c != '\n')
				c = inp->get();
		}
		if (word == "o") {
			// Entity name found.
			std::string line;
			std::getline(*inp, line);
			if (!geo.IsEmpty()) {
				// If multiple geometries are found in the file, the last one is
				// returned, all others are stored in the geometries vector.
				geo.Finish();
				geometries.push_back(geo);
				geo.Clear();
			}
			geo.name = StringTrim(line);
			v_offs += v.size();
			vt_offs += vt.size();
			vn_offs += vn.size();
			vp_offs += vp.size();
			v.clear();
			vt.clear();
			vn.clear();
			vp.clear();
			v_width = -1;
			vt_width = -1;
			vn_width = -1;
			vp_width = -1;
		}
		if (word == "v")
			v_width = ExtractNumbers(v, v_width);
		if (word == "vt")
			vt_width = ExtractNumbers(vt, vt_width);
		if (word == "vn")
			vn_width = ExtractNumbers(vn, vn_width);
		if (word == "vp")
			vp_width = ExtractNumbers(vp, vp_width);

		if (word == "f") {
			size_t n = 0;
			size_t m = 0;
			size_t w = 0;
			bool flag = false;
			std::vector<int> ids;
			std::string tok = NextToken();
			while (!tok.empty()) {
				if (tok == "/") {
					if (!flag)
						ids.push_back(-1);
					++m;
					if (m > w)
						w = m;
					flag = false;
				} else {
					if (flag) {
						while (m < w) {
							ids.push_back(-1);
							++m;
						}
						m = 0;
						++n;
					}
					ids.push_back(std::atoi(tok.c_str()));
					flag = true;
				}
				tok = NextToken();
			}
			++w;
			// If there are 6 values per vertex, these are x,y,z,r,g,b
			if (v_width == 6) {
				// Loop over the face vertices to convert them into a triangle
				// chain.
				for (size_t i = 2; i <= n; ++i) {

					size_t p0 = (ids[0 * w + 0] - 1) * v_width - v_offs;
					size_t p1 = (ids[(i - 1) * w + 0] - 1) * v_width - v_offs;
					size_t p2 = (ids[i * w + 0] - 1) * v_width - v_offs;
					size_t n0 = (ids[0 * w + 2] - 1) * vn_width - vn_offs;
					size_t n1 = (ids[(i - 1) * w + 2] - 1) * vn_width - vn_offs;
					size_t n2 = (ids[i * w + 2] - 1) * vn_width - vn_offs;

					Vector3 a(v[p0], v[p0 + 1], v[p0 + 2]);
					Vector3 b(v[p1], v[p1 + 1], v[p1 + 2]);
					Vector3 c(v[p2], v[p2 + 1], v[p2 + 2]);
					Vector3 ca(v[p0 + 3], v[p0 + 4], v[p0 + 5]);
					Vector3 cb(v[p1 + 3], v[p1 + 4], v[p1 + 5]);
					Vector3 cc(v[p2 + 3], v[p2 + 4], v[p2 + 5]);
					Vector3 na(vn[n0], vn[n0 + 1], vn[n0 + 2]);
					Vector3 nb(vn[n1], vn[n1 + 1], vn[n1 + 2]);
					Vector3 nc(vn[n2], vn[n2 + 1], vn[n2 + 2]);

					geo.SetAddNormal(na);
					geo.SetAddColor(ca);
					geo.AddVertex(a);
					size_t idx0 = geo.CountVertices() - 1;
					geo.SetAddNormal(nb);
					geo.SetAddColor(cb);
					geo.AddVertex(b);
					size_t idx1 = geo.CountVertices() - 1;
					geo.SetAddNormal(nc);
					geo.SetAddColor(cc);
					geo.AddVertex(c);
					size_t idx2 = geo.CountVertices() - 1;

					geo.ResetPresets();
					geo.AddTriangle(idx0, idx1, idx2);
				}
			} else {
				for (size_t i = 2; i <= n; ++i) {
					size_t p0 = (ids[0 * w + 0] - 1) * v_width - v_offs;
					size_t p1 = (ids[(i - 1) * w + 0] - 1) * v_width - v_offs;
					size_t p2 = (ids[i * w + 0] - 1) * v_width - v_offs;
					size_t n0 = (ids[0 * w + 2] - 1) * vn_width - vn_offs;
					size_t n1 = (ids[(i - 1) * w + 2] - 1) * vn_width - vn_offs;
					size_t n2 = (ids[i * w + 2] - 1) * vn_width - vn_offs;

					Vector3 a(v[p0], v[p0 + 1], v[p0 + 2]);
					Vector3 b(v[p1], v[p1 + 1], v[p1 + 2]);
					Vector3 c(v[p2], v[p2 + 1], v[p2 + 2]);
					Vector3 na(vn[n0], vn[n0 + 1], vn[n0 + 2]);
					Vector3 nb(vn[n1], vn[n1 + 1], vn[n1 + 2]);
					Vector3 nc(vn[n2], vn[n2 + 1], vn[n2 + 2]);

					geo.SetAddNormal(na);
					geo.AddVertex(a);
					size_t idx0 = geo.CountVertices() - 1;
					geo.SetAddNormal(nb);
					geo.AddVertex(b);
					size_t idx1 = geo.CountVertices() - 1;
					geo.SetAddNormal(nc);
					geo.AddVertex(c);
					size_t idx2 = geo.CountVertices() - 1;

					geo.ResetPresets();
					geo.AddTriangle(idx0, idx1, idx2);
				}
			}
		}
		if (word == "l") {
			// Lines are not supported: read and ignore
			std::string line;
			std::getline(*inp, line);
		}
		if (word == "s") {
			// Smooth shading
			std::string line;
			std::getline(*inp, line);
			//TODO Read value from file
			geo.smooth = false;
		}
		if (word == "g") {
			std::string line;
			std::getline(*inp, line);
			if (!geo.IsEmpty()) {
				// If multiple geometries are found in the file, the last one is
				// returned, all others are stored in the geometries vector.
				geo.Finish();
				geometries.push_back(geo);
				geo.Clear();
			}
			geo.name = StringTrim(line);
			v_offs += v.size();
			vt_offs += vt.size();
			vn_offs += vn.size();
			vp_offs += vp.size();
			v.clear();
			vt.clear();
			vn.clear();
			vp.clear();
			v_width = -1;
			vt_width = -1;
			vn_width = -1;
			vp_width = -1;
		}
		*inp >> word;
	}
	if (!geo.IsEmpty()) {
		geo.Finish();
	}
}

void FileOBJ::WriteStream(const Geometry &geo) {
	throw std::runtime_error("FileOBJ::WriteStream - Not implemented.");
}

