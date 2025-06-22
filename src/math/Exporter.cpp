///////////////////////////////////////////////////////////////////////////////
// Name               : Exporter.cpp
// Purpose            : Exports objects to .mat files
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 26.06.2022
// Copyright          : (C) 2022 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "Exporter.h"

#include "../3D/Polygon3.h"
#include "DependentVector.h"
#include "Matrix.h"

#include <stddef.h>

Exporter::Exporter(const std::string &filename_, const std::string &prefix_) :
		MatlabFile(filename_), prefix(prefix_) {
}

void Exporter::Add(const Matrix &matrix, const std::string &name) {
	std::string name_ = GenName(name);
	Matrix M = matrix;
	M.SetVariableName(name_);
	WriteMatrix(M);
}

void Exporter::Add(const Geometry &geo, const std::string &name) {
	std::string name_ = GenName(name);
	std::string nameX = name_ + "x";
	std::string nameY = name_ + "y";
	std::string nameZ = name_ + "z";
	std::string nameNX = name_ + "nx";
	std::string nameNY = name_ + "ny";
	std::string nameNZ = name_ + "nz";
	std::string nameU = name_ + "u";
	std::string nameV = name_ + "v";

	Matrix Mx(nameX, geo.CountVertices(), 1);
	Matrix My(nameY, geo.CountVertices(), 1);
	Matrix Mz(nameZ, geo.CountVertices(), 1);
	Matrix Mnx(nameNX, geo.CountVertices(), 1);
	Matrix Mny(nameNY, geo.CountVertices(), 1);
	Matrix Mnz(nameNZ, geo.CountVertices(), 1);
	Matrix Mu(nameU, geo.CountVertices(), 1);
	Matrix Mv(nameV, geo.CountVertices(), 1);
	for (size_t n = 0; n < geo.CountVertices(); ++n) {
		Mx[n] = geo[n].x;
		My[n] = geo[n].y;
		Mz[n] = geo[n].z;
		Mnx[n] = geo[n].n.x;
		Mny[n] = geo[n].n.y;
		Mnz[n] = geo[n].n.z;
		Mu[n] = geo[n].u;
		Mv[n] = geo[n].v;
	}
	WriteMatrix(Mx);
	WriteMatrix(My);
	WriteMatrix(Mz);
	WriteMatrix(Mnx);
	WriteMatrix(Mny);
	WriteMatrix(Mnz);
	WriteMatrix(Mu);
	WriteMatrix(Mv);
}

void Exporter::Add(const DependentVector &vector, const std::string &name) {
	Matrix M = vector;
	M.SetVariableName(GenName(name));
	WriteMatrix(M);
}

void Exporter::Add(const Polygon3 &polygon, const std::string &name) {
	std::string name_ = GenName(name);
	std::string nameX = name_ + "x";
	std::string nameY = name_ + "y";
	std::string nameZ = name_ + "z";
	std::string nameNX = name_ + "nx";
	std::string nameNY = name_ + "ny";
	std::string nameNZ = name_ + "nz";
	std::string nameU = name_ + "u";
	std::string nameV = name_ + "v";

	Matrix Mx(nameX, polygon.CountVertices(), 1);
	Matrix My(nameY, polygon.CountVertices(), 1);
	Matrix Mz(nameZ, polygon.CountVertices(), 1);
	Matrix Mnx(nameNX, polygon.CountVertices(), 1);
	Matrix Mny(nameNY, polygon.CountVertices(), 1);
	Matrix Mnz(nameNZ, polygon.CountVertices(), 1);
	Matrix Mu(nameU, polygon.CountVertices(), 1);
	Matrix Mv(nameV, polygon.CountVertices(), 1);
	for (size_t n = 0; n < polygon.Size(); ++n) {
		Mx[n] = polygon[n].x;
		My[n] = polygon[n].y;
		Mz[n] = polygon[n].z;
		Mnx[n] = polygon[n].n.x;
		Mny[n] = polygon[n].n.y;
		Mnz[n] = polygon[n].n.z;
		Mu[n] = polygon[n].u;
		Mv[n] = polygon[n].v;
	}
	WriteMatrix(Mx);
	WriteMatrix(My);
	WriteMatrix(Mz);
	WriteMatrix(Mnx);
	WriteMatrix(Mny);
	WriteMatrix(Mnz);
	WriteMatrix(Mu);
	WriteMatrix(Mv);
}

void Exporter::Add(const std::initializer_list<double> &values,
		const std::string &name) {

	Matrix m(GenName(name), values.size());
	for (const double &i : values)
		m.Insert(i);
	WriteMatrix(m);
}

void Exporter::Add(const std::initializer_list<double> &values0,
		const std::initializer_list<double> &values1, const std::string &name) {
	Matrix m(GenName(name), values0.size());
	for (const double &i : values0)
		m.Insert(i);
	for (const double &i : values1)
		m.Insert(i);
	WriteMatrix(m);
}

void Exporter::Add(const std::vector<double> &values, const std::string &name) {
	Matrix m(GenName(name), values.size());
	for (const double &i : values)
		m.Insert(i);
	WriteMatrix(m);
}

void Exporter::Add(const std::vector<double> &values0,
		const std::vector<double> &values1, const std::string &name) {
	Matrix m(GenName(name), values0.size());
	for (const double &i : values0)
		m.Insert(i);
	for (const double &i : values1)
		m.Insert(i);
	WriteMatrix(m);
}

std::string Exporter::GenName(const std::string &name) {
	if (name.empty()) {
		if (prefix.empty())
			prefix = "x";
		return prefix + std::to_string(count++);
	}
	if (!prefix.empty() && name == prefix)
		return prefix + std::to_string(count++);

	prefix = name;
	return name;
}
