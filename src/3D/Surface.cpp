///////////////////////////////////////////////////////////////////////////////
// Name               : Surface.cpp
// Purpose            : Quadratic surface
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 05.10.2023
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

#include "Surface.h"

#include "../math/Dependencies.h"
#include "../math/MatlabFile.h"
#include "../math/Matrix.h"
#include "../math/SVD.h"
//#include "../system/StopWatch.h"

#include <cmath>
#include <iostream>

#include "OpenGL.h"

Surface::Vector Surface::Patch::operator ()(double u, double v) const {

	std::array<double, 36> x;
	x[0] = 1.0;
	// Fill the low part for the point with the powers from 0 to 3
	for (uint_fast8_t i = 1; i < 4; ++i)
		x[i] = x[i - 1] * u;
	// Mid part from v = 1 to v = 5
	for (uint_fast8_t i = 4; i < 24; ++i)
		x[i] = x[i - 4] * v;
	// Fill the high part for the normal vector with u = 4 and u = 5
	for (uint_fast8_t i = 24; i < 30; ++i)
		x[i] = x[i * 4 - 93] * u;
	for (uint_fast8_t i = 30; i < 36; ++i)
		x[i] = x[i - 6] * u;

	Surface::Vector ret;
	for (uint_fast8_t i = 0; i < 16; ++i)
		ret.x += x[i] * cx[i];
	for (uint_fast8_t i = 0; i < 16; ++i)
		ret.y += x[i] * cy[i];
	for (uint_fast8_t i = 0; i < 16; ++i)
		ret.z += x[i] * cz[i];
	for (uint_fast8_t i = 0; i < 36; ++i)
		ret.nx += x[i] * cnx[i];
	for (uint_fast8_t i = 0; i < 36; ++i)
		ret.ny += x[i] * cny[i];
	for (uint_fast8_t i = 0; i < 36; ++i)
		ret.nz += x[i] * cnz[i];

	//Debugging:
//	ret.nx = cx[5] * u + cx[4];
//	ret.ny = cy[5] * u + cy[4];
//	ret.nz = cz[5] * u + cz[4];

	double d = ret.nx * ret.nx + ret.ny * ret.ny + ret.nz * ret.nz;
	if (d > 0.0) {
		double ds = std::sqrt(d);
		ret.nx /= ds;
		ret.ny /= ds;
		ret.nz /= ds;
	}
	return ret;
}

double Surface::Patch::MaxBend() const {
	return 0;
}

void Surface::Patch::Paint() const {
	const size_t Nu = 20;
	const size_t Nv = 30;

	const double du = (u1 - u0) / (double) Nu;
	const double dv = (v1 - v0) / (double) Nv;
	{
		double u = u0;
		for (size_t i = 0; i < Nu; ++i) {
			glBegin(GL_QUAD_STRIP);
			Surface::Vector vt0 = this->operator()(u, v0);
			Surface::Vector vt1 = this->operator()(u + du, v0);
			glNormal3f(vt0.nx, vt0.ny, vt0.nz);
			glVertex3f(vt0.x, vt0.y, vt0.z);
			glNormal3f(vt1.nx, vt1.ny, vt1.nz);
			glVertex3f(vt1.x, vt1.y, vt1.z);
			double v = v0;
			for (size_t j = 0; j < Nv; ++j) {
				Surface::Vector vx0 = this->operator()(u, v + dv);
				Surface::Vector vx1 = this->operator()(u + du, v + dv);
				glNormal3f(vx0.nx, vx0.ny, vx0.nz);
				glVertex3f(vx0.x, vx0.y, vx0.z);
				glNormal3f(vx1.nx, vx1.ny, vx1.nz);
				glVertex3f(vx1.x, vx1.y, vx1.z);
				v += dv;
			}
			glEnd();
			u += du;
		}
	}
	Surface::Vector p0 = operator ()(u0, v0);
	Surface::Vector p1 = operator ()(u1, v0);
	Surface::Vector p2 = operator ()(u0, v1);
	Surface::Vector p3 = operator ()(u1, v1);

	glColor3f(0.9f, 0.9f, 0.7f);
	glPointSize(10);
	glBegin(GL_POINTS);
	glNormal3d(p0.nx, p0.ny, p0.nz);
	glVertex3d(p0.x, p0.y, p0.z);
	glNormal3d(p1.nx, p1.ny, p1.nz);
	glVertex3d(p1.x, p1.y, p1.z);
	glNormal3d(p2.nx, p2.ny, p2.nz);
	glVertex3d(p2.x, p2.y, p2.z);
	glNormal3d(p3.nx, p3.ny, p3.nz);
	glVertex3d(p3.x, p3.y, p3.z);
	glEnd();

	if (false) {
		glBegin(GL_LINES);
		for (double u = u0; u < u1 - du / 2.0; u += du) {
			for (double v = v0; v < v1 - dv / 2.0; v += dv) {
				Surface::Vector vt0 = this->operator()(u + du / 2.0,
						v + dv / 2.0);
				glNormal3f(vt0.nx, vt0.ny, vt0.nz);
				glVertex3f(vt0.x, vt0.y, vt0.z);
				glVertex3f(vt0.x + vt0.nx / 20, vt0.y + vt0.ny / 20.0,
						vt0.z + vt0.nz / 20);
			}
		}
		glEnd();
	}
}

void Surface::AddConnection(Dependencies &dep, uint_fast8_t N, size_t idx1,
		size_t idx2, double u, double v) {
	const uint_fast8_t N2 = N * N;
	size_t n;

	static const size_t M = 4;
	std::array<double, M * M> x;
	if (N < 4)
		x.fill(0.0);
	x[0] = 1.0;
	for (uint_fast8_t i = 1; i < N; ++i)
		x[i] = x[i - 1] * u;
	for (uint_fast8_t i = N; i < N2; ++i)
		x[i] = x[i - N] * v;

	std::array<double, M * M> xdu;
	for (uint_fast8_t j = 0; j < M; ++j) {
		xdu[j * M] = 0.0;
		for (uint_fast8_t i = 1; i < M; ++i)
			xdu[j * M + i] = x[j * M + i - 1] * i;
	}

	std::array<double, M * M> xdv;
	for (uint_fast8_t i = 0; i < M; ++i) {
		xdv[i] = 0.0;
		for (uint_fast8_t j = 1; j < M; ++j)
			xdv[j * M + i] = x[(j - 1) * M + i] * j;
	}

	if (N == 2) {
		n = dep.Size(1);
		dep.SetSize(N2 * patches.size(), n + 1);

		for (uint_fast8_t i = 0; i < N2; ++i) {
			dep(idx1 * N2 + i, n) = x[i];
			dep(idx2 * N2 + i, n) = -x[i];
		}
	} else {
		// Link positions
		n = dep.Size(1);
		dep.SetSize(N2 * patches.size(), n + 3);

		for (uint_fast8_t i = 0; i < N2; ++i) {
			dep(idx1 * N2 + i, n) = x[i];
			dep(idx2 * N2 + i, n) = -x[i];
			dep(idx1 * N2 + i, n + 1) = xdu[i];
			dep(idx2 * N2 + i, n + 1) = -xdu[i];
			dep(idx1 * N2 + i, n + 2) = xdv[i];
			dep(idx2 * N2 + i, n + 2) = -xdv[i];
		}
	}
}

void Surface::Calculate(const std::vector<Surface::Point> &points) {
	patches.clear();
	{
		Surface::Patch temp;
		temp.u0 = 0.0;
		temp.u1 = 0.3;
		temp.v0 = 0.3;
		temp.v1 = 0.5;
		patches.push_back(temp);
	}
	{
		Surface::Patch temp;
		temp.u0 = 0.0;
		temp.u1 = 0.3;
		temp.v0 = 0.5;
		temp.v1 = 0.6;
		patches.push_back(temp);
	}
	{
		Surface::Patch temp;
		temp.u0 = 0.0;
		temp.u1 = 0.3;
		temp.v0 = 0.6;
		temp.v1 = 0.8;
		patches.push_back(temp);
	}

	{
		Surface::Patch temp;
		temp.u0 = 0.3;
		temp.u1 = 0.6;
		temp.v0 = 0.3;
		temp.v1 = 0.5;
		patches.push_back(temp);
	}
	{
		Surface::Patch temp;
		temp.u0 = 0.3;
		temp.u1 = 0.6;
		temp.v0 = 0.5;
		temp.v1 = 0.6;
		patches.push_back(temp);
	}
	{
		Surface::Patch temp;
		temp.u0 = 0.3;
		temp.u1 = 0.6;
		temp.v0 = 0.6;
		temp.v1 = 0.8;
		patches.push_back(temp);
	}

	debug.clear();

	const uint8_t N = 4;

	Matrix X;
	Matrix Yx;
	Matrix Yy;
	Matrix Yz;

//	debug = points;
	for (size_t idxPoint = 0; idxPoint < points.size(); ++idxPoint) {
		const Surface::Point &point = points[idxPoint];
		for (size_t idxPatch = 0; idxPatch < patches.size(); ++idxPatch) {
			Surface::Patch &patch = patches[idxPatch];
			if (point.u >= patch.u0 && point.u <= patch.u1
					&& point.v >= patch.v0 && point.v <= patch.v1) {

				debug.push_back(point);

				std::array<double, 16> x;
				if (N < 4)
					x.fill(0.0);
				x[0] = 1.0;
				for (uint_fast8_t i = 1; i < N; ++i)
					x[i] = x[i - 1] * point.u;
				for (uint_fast8_t i = N; i < (N * N); ++i)
					x[i] = x[i - N] * point.v;

				size_t n = X.Size(1);
				X.SetSize(N * N * patches.size(), n + 1);
				Yx.SetSize(n + 1, 1);
				Yy.SetSize(n + 1, 1);
				Yz.SetSize(n + 1, 1);

				for (uint_fast8_t i = 0; i < (N * N); ++i)
					X.Insert(x[i], idxPatch * N * N + i, n);

//				X.SetInsertPosition(idxPatch * 16, idxPoint);
//				X.Insert(x.data(), x.size());
				Yx.Insert(point.x, n, 0);
				Yy.Insert(point.y, n, 0);
				Yz.Insert(point.z, n, 0);
			}
		}
	}
	X.Transpose();
	Dependencies dep;

	for (size_t idxCheck = 1; idxCheck < patches.size(); ++idxCheck) {
		for (size_t idxRef = 0; idxRef < idxCheck; ++idxRef) {
			const Surface::Patch &patch = patches[idxCheck];
			const Surface::Patch &ref = patches[idxRef];
			if (patch.u0 > ref.u0 - eps && patch.u0 < ref.u1 + eps
					&& patch.v0 > ref.v0 - eps && patch.v0 < ref.v1 + eps)
				AddConnection(dep, N, idxCheck, idxRef, patch.u0, patch.v0);
			if (patch.u1 > ref.u0 - eps && patch.u1 < ref.u1 + eps
					&& patch.v0 > ref.v0 - eps && patch.v0 < ref.v1 + eps)
				AddConnection(dep, N, idxCheck, idxRef, patch.u1, patch.v0);
			if (patch.u0 > ref.u0 - eps && patch.u0 < ref.u1 + eps
					&& patch.v1 > ref.v0 - eps && patch.v1 < ref.v1 + eps)
				AddConnection(dep, N, idxCheck, idxRef, patch.u0, patch.v1);
			if (patch.u1 > ref.u0 - eps && patch.u1 < ref.u1 + eps
					&& patch.v1 > ref.v0 - eps && patch.v1 < ref.v1 + eps)
				AddConnection(dep, N, idxCheck, idxRef, patch.u1, patch.v1);
		}
	}

	dep.Transpose();
	Matrix T = dep;

	Matrix Sx;
	Matrix Sy;
	Matrix Sz;

	{
		// Write .mat file for checking with Octave
		MatlabFile mf("/tmp/surf.mat");
		dep.SetVariableName("deptest");
		mf.WriteMatrix(dep);
		dep.Calculate();
		{
			T *= dep;
			if (T.Min() < -1e-9)
				std::cout
						<< "The dependency matrix is not fully solvable. Min = "
						<< T.Min() << '\n';
			if (T.Max() > 1e-9)
				std::cout
						<< "The dependency matrix is not fully solvable. Max = "
						<< T.Max() << '\n';
		}
		dep.SetVariableName("dep");
		mf.WriteMatrix(dep);
		X.SetVariableName("X0");
		mf.WriteMatrix(X);
		X = X * dep;
		{
//			const double c = X.Cond();
//			std::cout << "Conditional number of problem: " << c << '\n';
//			std::cout << "Solving the matrix looses " << log10(c)
//					<< " digits of precision.\n";
		}

//		StopWatch sw;
//		sw.Start();
		SVD svd(X);
//		sw.Stop();
//		std::cout << "Duration: " << sw.GetSecondsCPU() << "s\n";
		mf.WriteMatrix(svd.U);
		mf.WriteMatrix(svd.W);
		mf.WriteMatrix(svd.V);

//		X.PseudoInvert();
//		X = dep * X;
//
//		X.SetVariableName("Xinv");
//		mf.WriteMatrix(X);

		const double maxCond = 80.0;

		Sx = svd.Solve(Yx, maxCond);
		Sy = svd.Solve(Yy, maxCond);
		Sz = svd.Solve(Yz, maxCond);

		Sx = dep * Sx;
		Sy = dep * Sy;
		Sz = dep * Sz;

//		Sx = X * Yx;
//		Sy = X * Yy;
//		Sz = X * Yz;

		Yx.SetVariableName("Yx");
		Yy.SetVariableName("Yy");
		Yz.SetVariableName("Yz");
		Sx.SetVariableName("Sx");
		Sy.SetVariableName("Sy");
		Sz.SetVariableName("Sz");

		mf.WriteMatrix(Yx);
		mf.WriteMatrix(Yy);
		mf.WriteMatrix(Yz);
		mf.WriteMatrix(Sx);
		mf.WriteMatrix(Sy);
		mf.WriteMatrix(Sz);

		mf.Close();
	}

	if (false) {
		// Read .mat file generated by Octave
		MatlabFile mf2("/tmp/surf2.mat");
		mf2.ReadMatrix(&Sx, "Rx");
		mf2.ReadMatrix(&Sy, "Ry");
		mf2.ReadMatrix(&Sz, "Rz");
		mf2.Close();
	}

	for (size_t idxPatch = 0; idxPatch < patches.size(); ++idxPatch) {
		Surface::Patch &patch = patches[idxPatch];
		for (uint_fast8_t i = 0; i < N; ++i) {
			for (uint_fast8_t j = 0; j < N; ++j) {
				patch.cx[i + 4 * j] = Sx(idxPatch * N * N + j * N + i, 0);
				patch.cy[i + 4 * j] = Sy(idxPatch * N * N + j * N + i, 0);
				patch.cz[i + 4 * j] = Sz(idxPatch * N * N + j * N + i, 0);
			}
		}
		patch.CalcNormalMatrices();
	}

}

void Surface::Paint() const {

//	glColor3f(0.0, 1.0, 1.0);
//	patches[0].Paint();
//	glColor3f(1.0, 0.0, 1.0);
//	patches[1].Paint();
//	glColor3f(1.0, 1.0, 0.0);
//	patches[2].Paint();

	for (const auto &patch : patches) {
		glColor3f(1.0, 1.0, 0.0);
		patch.Paint();
	}

	glPointSize(5);
	glBegin(GL_POINTS);
	glNormal3f(0.0f, 1.0f, 0.2f);
	glColor3f(0.3f, 0.5f, 0.9f);
	for (const auto &point : debug) {
//		glColor3d(point.u, point.v, 0.0);
		glVertex3d(point.x, point.y, point.z);
	}
	glEnd();
}

void Surface::Patch::CalcNormalMatrices() {
	cnx[0] = -cy[4] * cz[1] + cy[1] * cz[4];
	cnx[1] = -2.0 * cy[4] * cz[2] + cy[1] * cz[5] - cy[5] * cz[1]
			+ 2.0 * cy[2] * cz[4];
	cnx[2] = -3.0 * cy[4] * cz[3] + cy[1] * cz[6] - 2.0 * cy[5] * cz[2]
			+ 2.0 * cy[2] * cz[5] - cy[6] * cz[1] + 3.0 * cy[3] * cz[4];
	cnx[3] = cy[1] * cz[7] - 3.0 * cy[5] * cz[3] + 2.0 * cy[2] * cz[6]
			- 2.0 * cy[6] * cz[2] + 3.0 * cy[3] * cz[5] - cy[7] * cz[1];
	cnx[4] = -cy[4] * cz[5] - 2.0 * cy[8] * cz[1] + 2.0 * cy[1] * cz[8]
			+ cy[5] * cz[4];
	cnx[5] = -2.0 * cy[4] * cz[6] - 4.0 * cy[8] * cz[2] + 2.0 * cy[1] * cz[9]
			- 2.0 * cy[9] * cz[1] + 4.0 * cy[2] * cz[8] + 2.0 * cy[6] * cz[4];
	cnx[6] = -3.0 * cy[4] * cz[7] - 6.0 * cy[8] * cz[3] + 2.0 * cy[1] * cz[10]
			- cy[5] * cz[6] - 4.0 * cy[9] * cz[2] + 4.0 * cy[2] * cz[9]
			+ cy[6] * cz[5] - 2.0 * cy[10] * cz[1] + 6.0 * cy[3] * cz[8]
			+ 3.0 * cy[7] * cz[4];
	cnx[7] = 2.0 * cy[1] * cz[11] - 2.0 * cy[5] * cz[7] - 6.0 * cy[9] * cz[3]
			+ 4.0 * cy[2] * cz[10] - 4.0 * cy[10] * cz[2] + 6.0 * cy[3] * cz[9]
			+ 2.0 * cy[7] * cz[5] - 2.0 * cy[11] * cz[1];
	cnx[8] = -cy[4] * cz[9] - 2.0 * cy[8] * cz[5] - 3.0 * cy[12] * cz[1]
			+ 3.0 * cy[1] * cz[12] + 2.0 * cy[5] * cz[8] + cy[9] * cz[4];
	cnx[9] = -2.0 * cy[4] * cz[10] - 4.0 * cy[8] * cz[6] - 6.0 * cy[12] * cz[2]
			+ 3.0 * cy[1] * cz[13] + cy[5] * cz[9] - cy[9] * cz[5]
			- 3.0 * cy[13] * cz[1] + 6.0 * cy[2] * cz[12] + 4.0 * cy[6] * cz[8]
			+ 2.0 * cy[10] * cz[4];
	cnx[10] = -3.0 * cy[4] * cz[11] - 6.0 * cy[8] * cz[7] - 9.0 * cy[12] * cz[3]
			+ 3.0 * cy[1] * cz[14] - 3.0 * cy[9] * cz[6] - 6.0 * cy[13] * cz[2]
			+ 6.0 * cy[2] * cz[13] + 3.0 * cy[6] * cz[9] - 3.0 * cy[14] * cz[1]
			+ 9.0 * cy[3] * cz[12] + 6.0 * cy[7] * cz[8] + 3.0 * cy[11] * cz[4];
	cnx[11] = 3.0 * cy[1] * cz[15] - cy[5] * cz[11] - 5.0 * cy[9] * cz[7]
			- 9.0 * cy[13] * cz[3] + 6.0 * cy[2] * cz[14] + 2.0 * cy[6] * cz[10]
			- 2.0 * cy[10] * cz[6] - 6.0 * cy[14] * cz[2] + 9.0 * cy[3] * cz[13]
			+ 5.0 * cy[7] * cz[9] + cy[11] * cz[5] - 3.0 * cy[15] * cz[1];
	cnx[12] = -cy[4] * cz[13] - 2.0 * cy[8] * cz[9] - 3.0 * cy[12] * cz[5]
			+ 3.0 * cy[5] * cz[12] + 2.0 * cy[9] * cz[8] + cy[13] * cz[4];
	cnx[13] = -2.0 * cy[4] * cz[14] - 4.0 * cy[8] * cz[10]
			- 6.0 * cy[12] * cz[6] + 2.0 * cy[5] * cz[13] - 2.0 * cy[13] * cz[5]
			+ 6.0 * cy[6] * cz[12] + 4.0 * cy[10] * cz[8]
			+ 2.0 * cy[14] * cz[4];
	cnx[14] = -3.0 * cy[4] * cz[15] - 6.0 * cy[8] * cz[11]
			- 9.0 * cy[12] * cz[7] + cy[5] * cz[14] - 2.0 * cy[9] * cz[10]
			- 5.0 * cy[13] * cz[6] + 5.0 * cy[6] * cz[13] + 2.0 * cy[10] * cz[9]
			- cy[14] * cz[5] + 9.0 * cy[7] * cz[12] + 6.0 * cy[11] * cz[8]
			+ 3.0 * cy[15] * cz[4];
	cnx[15] = -4.0 * cy[9] * cz[11] - 8.0 * cy[13] * cz[7]
			+ 4.0 * cy[6] * cz[14] - 4.0 * cy[14] * cz[6] + 8.0 * cy[7] * cz[13]
			+ 4.0 * cy[11] * cz[9];
	cnx[16] = -2.0 * cy[8] * cz[13] - 3.0 * cy[12] * cz[9]
			+ 3.0 * cy[9] * cz[12] + 2.0 * cy[13] * cz[8];
	cnx[17] = -4.0 * cy[8] * cz[14] - 6.0 * cy[12] * cz[10] + cy[9] * cz[13]
			- cy[13] * cz[9] + 6.0 * cy[10] * cz[12] + 4.0 * cy[14] * cz[8];
	cnx[18] = -6.0 * cy[8] * cz[15] - 9.0 * cy[12] * cz[11] - cy[9] * cz[14]
			- 4.0 * cy[13] * cz[10] + 4.0 * cy[10] * cz[13] + cy[14] * cz[9]
			+ 9.0 * cy[11] * cz[12] + 6.0 * cy[15] * cz[8];
	cnx[19] = -3.0 * cy[9] * cz[15] - 7.0 * cy[13] * cz[11]
			+ 2.0 * cy[10] * cz[14] - 2.0 * cy[14] * cz[10]
			+ 7.0 * cy[11] * cz[13] + 3.0 * cy[15] * cz[9];
	cnx[20] = -3.0 * cy[12] * cz[13] + 3.0 * cy[13] * cz[12];
	cnx[21] = -6.0 * cy[12] * cz[14] + 6.0 * cy[14] * cz[12];
	cnx[22] = -9.0 * cy[12] * cz[15] - 3.0 * cy[13] * cz[14]
			+ 3.0 * cy[14] * cz[13] + 9.0 * cy[15] * cz[12];
	cnx[23] = -6.0 * cy[13] * cz[15] + 6.0 * cy[15] * cz[13];
	cnx[24] = 2.0 * cy[2] * cz[7] - 3.0 * cy[6] * cz[3] + 3.0 * cy[3] * cz[6]
			- 2.0 * cy[7] * cz[2];
	cnx[25] = 4.0 * cy[2] * cz[11] - cy[6] * cz[7] - 6.0 * cy[10] * cz[3]
			+ 6.0 * cy[3] * cz[10] + cy[7] * cz[6] - 4.0 * cy[11] * cz[2];
	cnx[26] = 6.0 * cy[2] * cz[15] + cy[6] * cz[11] - 4.0 * cy[10] * cz[7]
			- 9.0 * cy[14] * cz[3] + 9.0 * cy[3] * cz[14] + 4.0 * cy[7] * cz[10]
			- cy[11] * cz[6] - 6.0 * cy[15] * cz[2];
	cnx[27] = 3.0 * cy[6] * cz[15] - 2.0 * cy[10] * cz[11]
			- 7.0 * cy[14] * cz[7] + 7.0 * cy[7] * cz[14]
			+ 2.0 * cy[11] * cz[10] - 3.0 * cy[15] * cz[6];
	cnx[28] = -5.0 * cy[14] * cz[11] + 5.0 * cy[11] * cz[14];
	cnx[29] = -3.0 * cy[14] * cz[15] + 3.0 * cy[15] * cz[14];
	cnx[30] = 3.0 * cy[3] * cz[7] - 3.0 * cy[7] * cz[3];
	cnx[31] = 6.0 * cy[3] * cz[11] - 6.0 * cy[11] * cz[3];
	cnx[32] = 9.0 * cy[3] * cz[15] + 3.0 * cy[7] * cz[11] - 3.0 * cy[11] * cz[7]
			- 9.0 * cy[15] * cz[3];
	cnx[33] = 6.0 * cy[7] * cz[15] - 6.0 * cy[15] * cz[7];
	cnx[34] = 3.0 * cy[11] * cz[15] - 3.0 * cy[15] * cz[11];
	cnx[35] = 0.0;

	cny[0] = cx[4] * cz[1] - cx[1] * cz[4];
	cny[1] = 2.0 * cx[4] * cz[2] - cx[1] * cz[5] + cx[5] * cz[1]
			- 2.0 * cx[2] * cz[4];
	cny[2] = 3.0 * cx[4] * cz[3] - cx[1] * cz[6] + 2.0 * cx[5] * cz[2]
			- 2.0 * cx[2] * cz[5] + cx[6] * cz[1] - 3.0 * cx[3] * cz[4];
	cny[3] = -cx[1] * cz[7] + 3.0 * cx[5] * cz[3] - 2.0 * cx[2] * cz[6]
			+ 2.0 * cx[6] * cz[2] - 3.0 * cx[3] * cz[5] + cx[7] * cz[1];
	cny[4] = cx[4] * cz[5] + 2.0 * cx[8] * cz[1] - 2.0 * cx[1] * cz[8]
			- cx[5] * cz[4];
	cny[5] = 2.0 * cx[4] * cz[6] + 4.0 * cx[8] * cz[2] - 2.0 * cx[1] * cz[9]
			+ 2.0 * cx[9] * cz[1] - 4.0 * cx[2] * cz[8] - 2.0 * cx[6] * cz[4];
	cny[6] = 3.0 * cx[4] * cz[7] + 6.0 * cx[8] * cz[3] - 2.0 * cx[1] * cz[10]
			+ cx[5] * cz[6] + 4.0 * cx[9] * cz[2] - 4.0 * cx[2] * cz[9]
			- cx[6] * cz[5] + 2.0 * cx[10] * cz[1] - 6.0 * cx[3] * cz[8]
			- 3.0 * cx[7] * cz[4];
	cny[7] = -2.0 * cx[1] * cz[11] + 2.0 * cx[5] * cz[7] + 6.0 * cx[9] * cz[3]
			- 4.0 * cx[2] * cz[10] + 4.0 * cx[10] * cz[2] - 6.0 * cx[3] * cz[9]
			- 2.0 * cx[7] * cz[5] + 2.0 * cx[11] * cz[1];
	cny[8] = cx[4] * cz[9] + 2.0 * cx[8] * cz[5] + 3.0 * cx[12] * cz[1]
			- 3.0 * cx[1] * cz[12] - 2.0 * cx[5] * cz[8] - cx[9] * cz[4];
	cny[9] = 2.0 * cx[4] * cz[10] + 4.0 * cx[8] * cz[6] + 6.0 * cx[12] * cz[2]
			- 3.0 * cx[1] * cz[13] - cx[5] * cz[9] + cx[9] * cz[5]
			+ 3.0 * cx[13] * cz[1] - 6.0 * cx[2] * cz[12] - 4.0 * cx[6] * cz[8]
			- 2.0 * cx[10] * cz[4];
	cny[10] = 3.0 * cx[4] * cz[11] + 6.0 * cx[8] * cz[7] + 9.0 * cx[12] * cz[3]
			- 3.0 * cx[1] * cz[14] + 3.0 * cx[9] * cz[6] + 6.0 * cx[13] * cz[2]
			- 6.0 * cx[2] * cz[13] - 3.0 * cx[6] * cz[9] + 3.0 * cx[14] * cz[1]
			- 9.0 * cx[3] * cz[12] - 6.0 * cx[7] * cz[8] - 3.0 * cx[11] * cz[4];
	cny[11] = -3.0 * cx[1] * cz[15] + cx[5] * cz[11] + 5.0 * cx[9] * cz[7]
			+ 9.0 * cx[13] * cz[3] - 6.0 * cx[2] * cz[14] - 2.0 * cx[6] * cz[10]
			+ 2.0 * cx[10] * cz[6] + 6.0 * cx[14] * cz[2] - 9.0 * cx[3] * cz[13]
			- 5.0 * cx[7] * cz[9] - cx[11] * cz[5] + 3.0 * cx[15] * cz[1];
	cny[12] = cx[4] * cz[13] + 2.0 * cx[8] * cz[9] + 3.0 * cx[12] * cz[5]
			- 3.0 * cx[5] * cz[12] - 2.0 * cx[9] * cz[8] - cx[13] * cz[4];
	cny[13] = 2.0 * cx[4] * cz[14] + 4.0 * cx[8] * cz[10] + 6.0 * cx[12] * cz[6]
			- 2.0 * cx[5] * cz[13] + 2.0 * cx[13] * cz[5] - 6.0 * cx[6] * cz[12]
			- 4.0 * cx[10] * cz[8] - 2.0 * cx[14] * cz[4];
	cny[14] = 3.0 * cx[4] * cz[15] + 6.0 * cx[8] * cz[11] + 9.0 * cx[12] * cz[7]
			- cx[5] * cz[14] + 2.0 * cx[9] * cz[10] + 5.0 * cx[13] * cz[6]
			- 5.0 * cx[6] * cz[13] - 2.0 * cx[10] * cz[9] + cx[14] * cz[5]
			- 9.0 * cx[7] * cz[12] - 6.0 * cx[11] * cz[8]
			- 3.0 * cx[15] * cz[4];
	cny[15] = 4.0 * cx[9] * cz[11] + 8.0 * cx[13] * cz[7] - 4.0 * cx[6] * cz[14]
			+ 4.0 * cx[14] * cz[6] - 8.0 * cx[7] * cz[13]
			- 4.0 * cx[11] * cz[9];
	cny[16] = 2.0 * cx[8] * cz[13] + 3.0 * cx[12] * cz[9] - 3.0 * cx[9] * cz[12]
			- 2.0 * cx[13] * cz[8];
	cny[17] = 4.0 * cx[8] * cz[14] + 6.0 * cx[12] * cz[10] - cx[9] * cz[13]
			+ cx[13] * cz[9] - 6.0 * cx[10] * cz[12] - 4.0 * cx[14] * cz[8];
	cny[18] = 6.0 * cx[8] * cz[15] + 9.0 * cx[12] * cz[11] + cx[9] * cz[14]
			+ 4.0 * cx[13] * cz[10] - 4.0 * cx[10] * cz[13] - cx[14] * cz[9]
			- 9.0 * cx[11] * cz[12] - 6.0 * cx[15] * cz[8];
	cny[19] = 3.0 * cx[9] * cz[15] + 7.0 * cx[13] * cz[11]
			- 2.0 * cx[10] * cz[14] + 2.0 * cx[14] * cz[10]
			- 7.0 * cx[11] * cz[13] - 3.0 * cx[15] * cz[9];
	cny[20] = 3.0 * cx[12] * cz[13] - 3.0 * cx[13] * cz[12];
	cny[21] = 6.0 * cx[12] * cz[14] - 6.0 * cx[14] * cz[12];
	cny[22] = 9.0 * cx[12] * cz[15] + 3.0 * cx[13] * cz[14]
			- 3.0 * cx[14] * cz[13] - 9.0 * cx[15] * cz[12];
	cny[23] = 6.0 * cx[13] * cz[15] - 6.0 * cx[15] * cz[13];
	cny[24] = -2.0 * cx[2] * cz[7] + 3.0 * cx[6] * cz[3] - 3.0 * cx[3] * cz[6]
			+ 2.0 * cx[7] * cz[2];
	cny[25] = -4.0 * cx[2] * cz[11] + cx[6] * cz[7] + 6.0 * cx[10] * cz[3]
			- 6.0 * cx[3] * cz[10] - cx[7] * cz[6] + 4.0 * cx[11] * cz[2];
	cny[26] = -6.0 * cx[2] * cz[15] - cx[6] * cz[11] + 4.0 * cx[10] * cz[7]
			+ 9.0 * cx[14] * cz[3] - 9.0 * cx[3] * cz[14] - 4.0 * cx[7] * cz[10]
			+ cx[11] * cz[6] + 6.0 * cx[15] * cz[2];
	cny[27] = -3.0 * cx[6] * cz[15] + 2.0 * cx[10] * cz[11]
			+ 7.0 * cx[14] * cz[7] - 7.0 * cx[7] * cz[14]
			- 2.0 * cx[11] * cz[10] + 3.0 * cx[15] * cz[6];
	cny[28] = 5.0 * cx[14] * cz[11] - 5.0 * cx[11] * cz[14];
	cny[29] = 3.0 * cx[14] * cz[15] - 3.0 * cx[15] * cz[14];
	cny[30] = -3.0 * cx[3] * cz[7] + 3.0 * cx[7] * cz[3];
	cny[31] = -6.0 * cx[3] * cz[11] + 6.0 * cx[11] * cz[3];
	cny[32] = -9.0 * cx[3] * cz[15] - 3.0 * cx[7] * cz[11]
			+ 3.0 * cx[11] * cz[7] + 9.0 * cx[15] * cz[3];
	cny[33] = -6.0 * cx[7] * cz[15] + 6.0 * cx[15] * cz[7];
	cny[34] = -3.0 * cx[11] * cz[15] + 3.0 * cx[15] * cz[11];
	cny[35] = 0.0;

	cnz[0] = -cx[4] * cy[1] + cx[1] * cy[4];
	cnz[1] = -2.0 * cx[4] * cy[2] + cx[1] * cy[5] - cx[5] * cy[1]
			+ 2.0 * cx[2] * cy[4];
	cnz[2] = -3.0 * cx[4] * cy[3] + cx[1] * cy[6] - 2.0 * cx[5] * cy[2]
			+ 2.0 * cx[2] * cy[5] - cx[6] * cy[1] + 3.0 * cx[3] * cy[4];
	cnz[3] = cx[1] * cy[7] - 3.0 * cx[5] * cy[3] + 2.0 * cx[2] * cy[6]
			- 2.0 * cx[6] * cy[2] + 3.0 * cx[3] * cy[5] - cx[7] * cy[1];
	cnz[4] = -cx[4] * cy[5] - 2.0 * cx[8] * cy[1] + 2.0 * cx[1] * cy[8]
			+ cx[5] * cy[4];
	cnz[5] = -2.0 * cx[4] * cy[6] - 4.0 * cx[8] * cy[2] + 2.0 * cx[1] * cy[9]
			- 2.0 * cx[9] * cy[1] + 4.0 * cx[2] * cy[8] + 2.0 * cx[6] * cy[4];
	cnz[6] = -3.0 * cx[4] * cy[7] - 6.0 * cx[8] * cy[3] + 2.0 * cx[1] * cy[10]
			- cx[5] * cy[6] - 4.0 * cx[9] * cy[2] + 4.0 * cx[2] * cy[9]
			+ cx[6] * cy[5] - 2.0 * cx[10] * cy[1] + 6.0 * cx[3] * cy[8]
			+ 3.0 * cx[7] * cy[4];
	cnz[7] = 2.0 * cx[1] * cy[11] - 2.0 * cx[5] * cy[7] - 6.0 * cx[9] * cy[3]
			+ 4.0 * cx[2] * cy[10] - 4.0 * cx[10] * cy[2] + 6.0 * cx[3] * cy[9]
			+ 2.0 * cx[7] * cy[5] - 2.0 * cx[11] * cy[1];
	cnz[8] = -cx[4] * cy[9] - 2.0 * cx[8] * cy[5] - 3.0 * cx[12] * cy[1]
			+ 3.0 * cx[1] * cy[12] + 2.0 * cx[5] * cy[8] + cx[9] * cy[4];
	cnz[9] = -2.0 * cx[4] * cy[10] - 4.0 * cx[8] * cy[6] - 6.0 * cx[12] * cy[2]
			+ 3.0 * cx[1] * cy[13] + cx[5] * cy[9] - cx[9] * cy[5]
			- 3.0 * cx[13] * cy[1] + 6.0 * cx[2] * cy[12] + 4.0 * cx[6] * cy[8]
			+ 2.0 * cx[10] * cy[4];
	cnz[10] = -3.0 * cx[4] * cy[11] - 6.0 * cx[8] * cy[7] - 9.0 * cx[12] * cy[3]
			+ 3.0 * cx[1] * cy[14] - 3.0 * cx[9] * cy[6] - 6.0 * cx[13] * cy[2]
			+ 6.0 * cx[2] * cy[13] + 3.0 * cx[6] * cy[9] - 3.0 * cx[14] * cy[1]
			+ 9.0 * cx[3] * cy[12] + 6.0 * cx[7] * cy[8] + 3.0 * cx[11] * cy[4];
	cnz[11] = 3.0 * cx[1] * cy[15] - cx[5] * cy[11] - 5.0 * cx[9] * cy[7]
			- 9.0 * cx[13] * cy[3] + 6.0 * cx[2] * cy[14] + 2.0 * cx[6] * cy[10]
			- 2.0 * cx[10] * cy[6] - 6.0 * cx[14] * cy[2] + 9.0 * cx[3] * cy[13]
			+ 5.0 * cx[7] * cy[9] + cx[11] * cy[5] - 3.0 * cx[15] * cy[1];
	cnz[12] = -cx[4] * cy[13] - 2.0 * cx[8] * cy[9] - 3.0 * cx[12] * cy[5]
			+ 3.0 * cx[5] * cy[12] + 2.0 * cx[9] * cy[8] + cx[13] * cy[4];
	cnz[13] = -2.0 * cx[4] * cy[14] - 4.0 * cx[8] * cy[10]
			- 6.0 * cx[12] * cy[6] + 2.0 * cx[5] * cy[13] - 2.0 * cx[13] * cy[5]
			+ 6.0 * cx[6] * cy[12] + 4.0 * cx[10] * cy[8]
			+ 2.0 * cx[14] * cy[4];
	cnz[14] = -3.0 * cx[4] * cy[15] - 6.0 * cx[8] * cy[11]
			- 9.0 * cx[12] * cy[7] + cx[5] * cy[14] - 2.0 * cx[9] * cy[10]
			- 5.0 * cx[13] * cy[6] + 5.0 * cx[6] * cy[13] + 2.0 * cx[10] * cy[9]
			- cx[14] * cy[5] + 9.0 * cx[7] * cy[12] + 6.0 * cx[11] * cy[8]
			+ 3.0 * cx[15] * cy[4];
	cnz[15] = -4.0 * cx[9] * cy[11] - 8.0 * cx[13] * cy[7]
			+ 4.0 * cx[6] * cy[14] - 4.0 * cx[14] * cy[6] + 8.0 * cx[7] * cy[13]
			+ 4.0 * cx[11] * cy[9];
	cnz[16] = -2.0 * cx[8] * cy[13] - 3.0 * cx[12] * cy[9]
			+ 3.0 * cx[9] * cy[12] + 2.0 * cx[13] * cy[8];
	cnz[17] = -4.0 * cx[8] * cy[14] - 6.0 * cx[12] * cy[10] + cx[9] * cy[13]
			- cx[13] * cy[9] + 6.0 * cx[10] * cy[12] + 4.0 * cx[14] * cy[8];
	cnz[18] = -6.0 * cx[8] * cy[15] - 9.0 * cx[12] * cy[11] - cx[9] * cy[14]
			- 4.0 * cx[13] * cy[10] + 4.0 * cx[10] * cy[13] + cx[14] * cy[9]
			+ 9.0 * cx[11] * cy[12] + 6.0 * cx[15] * cy[8];
	cnz[19] = -3.0 * cx[9] * cy[15] - 7.0 * cx[13] * cy[11]
			+ 2.0 * cx[10] * cy[14] - 2.0 * cx[14] * cy[10]
			+ 7.0 * cx[11] * cy[13] + 3.0 * cx[15] * cy[9];
	cnz[20] = -3.0 * cx[12] * cy[13] + 3.0 * cx[13] * cy[12];
	cnz[21] = -6.0 * cx[12] * cy[14] + 6.0 * cx[14] * cy[12];
	cnz[22] = -9.0 * cx[12] * cy[15] - 3.0 * cx[13] * cy[14]
			+ 3.0 * cx[14] * cy[13] + 9.0 * cx[15] * cy[12];
	cnz[23] = -6.0 * cx[13] * cy[15] + 6.0 * cx[15] * cy[13];
	cnz[24] = 2.0 * cx[2] * cy[7] - 3.0 * cx[6] * cy[3] + 3.0 * cx[3] * cy[6]
			- 2.0 * cx[7] * cy[2];
	cnz[25] = 4.0 * cx[2] * cy[11] - cx[6] * cy[7] - 6.0 * cx[10] * cy[3]
			+ 6.0 * cx[3] * cy[10] + cx[7] * cy[6] - 4.0 * cx[11] * cy[2];
	cnz[26] = 6.0 * cx[2] * cy[15] + cx[6] * cy[11] - 4.0 * cx[10] * cy[7]
			- 9.0 * cx[14] * cy[3] + 9.0 * cx[3] * cy[14] + 4.0 * cx[7] * cy[10]
			- cx[11] * cy[6] - 6.0 * cx[15] * cy[2];
	cnz[27] = 3.0 * cx[6] * cy[15] - 2.0 * cx[10] * cy[11]
			- 7.0 * cx[14] * cy[7] + 7.0 * cx[7] * cy[14]
			+ 2.0 * cx[11] * cy[10] - 3.0 * cx[15] * cy[6];
	cnz[28] = -5.0 * cx[14] * cy[11] + 5.0 * cx[11] * cy[14];
	cnz[29] = -3.0 * cx[14] * cy[15] + 3.0 * cx[15] * cy[14];
	cnz[30] = 3.0 * cx[3] * cy[7] - 3.0 * cx[7] * cy[3];
	cnz[31] = 6.0 * cx[3] * cy[11] - 6.0 * cx[11] * cy[3];
	cnz[32] = 9.0 * cx[3] * cy[15] + 3.0 * cx[7] * cy[11] - 3.0 * cx[11] * cy[7]
			- 9.0 * cx[15] * cy[3];
	cnz[33] = 6.0 * cx[7] * cy[15] - 6.0 * cx[15] * cy[7];
	cnz[34] = 3.0 * cx[11] * cy[15] - 3.0 * cx[15] * cy[11];
	cnz[35] = 0.0;
}

