///////////////////////////////////////////////////////////////////////////////
// Name               : Surface.cpp
// Purpose            : Quadratic surface
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : -isystem /usr/include/eigen3
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
#include "../math/Exporter.h"
#include "../math/MatlabFile.h"
#include "../math/Matrix.h"

#include <stdexcept>
//#include "../system/StopWatch.h"
#ifdef USE_EIGEN
#include <Eigen/Dense>
#else
#include "../math/SVD.h"
#endif

#include <cmath>
#include <iostream>
#include <random>
#include <sstream>

#include "OpenGL.h"

Geometry::Vertex Surface::Patch::operator ()(double u, double v) const {
	const size_t N = Nu * Nv;
	// Fill the UV matrix with the 2D polynomial factors for UV.

	std::vector<double> x(N);
	Fill(x, u, v);

	Geometry::Vertex ret;
	ret.u = u;
	ret.v = v;

	for (uint_fast8_t i = 0; i < N; i++)
		ret.x += x[i] * cx[i];
	for (uint_fast8_t i = 0; i < N; i++)
		ret.y += x[i] * cy[i];
	for (uint_fast8_t i = 0; i < N; i++)
		ret.z += x[i] * cz[i];

	Vector3 du;
	Vector3 dv;
	for (uint_fast8_t i = 0; i < N; i++)
		du.x += x[i] * cdxdu[i];
	for (uint_fast8_t i = 0; i < N; i++)
		du.y += x[i] * cdydu[i];
	for (uint_fast8_t i = 0; i < N; i++)
		du.z += x[i] * cdzdu[i];
	for (uint_fast8_t i = 0; i < N; i++)
		dv.x += x[i] * cdxdv[i];
	for (uint_fast8_t i = 0; i < N; i++)
		dv.y += x[i] * cdydv[i];
	for (uint_fast8_t i = 0; i < N; i++)
		dv.z += x[i] * cdzdv[i];

	ret.n = du * dv;

	//Debugging:
//	ret.nx = cx[5] * u + cx[4];
//	ret.ny = cy[5] * u + cy[4];
//	ret.nz = cz[5] * u + cz[4];

	ret.n.Normalize();
	return ret;
}

AffineTransformMatrix Surface::Patch::GetMatrix(double u, double v,
		bool originAtPos) const {
	const size_t N = Nu * Nv;
	std::vector<double> x(N);
	Fill(x, u, v);
	Vector3 pos;
	AffineTransformMatrix m;
	for (uint_fast8_t i = 0; i < N; i++)
		pos.x += x[i] * cx[i];
	for (uint_fast8_t i = 0; i < N; i++)
		pos.y += x[i] * cy[i];
	for (uint_fast8_t i = 0; i < N; i++)
		pos.z += x[i] * cz[i];

	Vector3 du;
	Vector3 dv;
	for (uint_fast8_t i = 0; i < N; i++)
		du.x += x[i] * cdxdu[i];
	for (uint_fast8_t i = 0; i < N; i++)
		du.y += x[i] * cdydu[i];
	for (uint_fast8_t i = 0; i < N; i++)
		du.z += x[i] * cdzdu[i];
	for (uint_fast8_t i = 0; i < N; i++)
		dv.x += x[i] * cdxdv[i];
	for (uint_fast8_t i = 0; i < N; i++)
		dv.y += x[i] * cdydv[i];
	for (uint_fast8_t i = 0; i < N; i++)
		dv.z += x[i] * cdzdv[i];

	du.Normalize();
	dv.Normalize();
	m.SetEx(du);
	m.SetEy(dv);
	m.CalculateEz();

	m.SetOrigin(pos);
	if (!originAtPos) {
		// Translate the matrix locally, so that x=u, y=v, z=0 ends up at pos
		// afterwards.
		m.TranslateLocal(-u, -v, 0);
	}
	return m;
}

bool Surface::Patch::IsInside(double u, double v) const {
	return (u >= u0 - DBL_EPSILON && u <= u1 + DBL_EPSILON
			&& v >= v0 - DBL_EPSILON && v <= v1 + DBL_EPSILON);
}

void Surface::Patch::Fill(std::vector<double> &vect, double u, double v) const {
	const size_t N = Nu * Nv;
	const double u_ = mapu(u);
	const double v_ = mapv(v);
	vect.resize(N);
	vect[0] = 1.0;
	for (uint_fast8_t i = 1; i < Nu; i++)
		vect[i] = vect[i - 1] * u_;
	for (uint_fast8_t j = Nu; j < N; j++)
		vect[j] = vect[j - Nu] * v_;
}

void Surface::Patch::FilldU(std::vector<double> &vect, double u, double v,
		uint8_t diff) const {
	const size_t N = Nu * Nv;
	const double u_ = mapu(u);
	const double v_ = mapv(v);
	vect.assign(N, 0.0);
	vect[diff] = 1.0;
	for (uint_fast8_t i = 1 + diff; i < Nu; i++)
		vect[i] = vect[i - 1] * u_;
	if (diff > 0) {
		for (uint_fast8_t i = 1 + diff; i < Nu; i++) {
			double f = 1.0;
			for (uint8_t h = i; h < i + diff; h++)
				f *= (double) h;
			vect[i] *= f;
		}
	}
	for (uint_fast8_t j = Nu; j < N; j++)
		vect[j] = vect[j - Nu] * v_;
}

void Surface::Patch::FilldV(std::vector<double> &vect, double u, double v,
		uint8_t diff) const {
	const size_t N = Nu * Nv;
	const double u_ = mapu(u);
	const double v_ = mapv(v);
	vect.assign(N, 0.0);

	vect[diff * Nu] = 1.0;
	for (uint_fast8_t j = 1 + diff; j < Nv; j++)
		vect[j * Nu] = vect[(j - 1) * Nu] * v_;
	if (diff > 0) {
		for (uint_fast8_t j = 1 + diff; j < Nv; j++) {
			double f = 1.0;
			for (uint8_t h = j; h < j + diff; h++)
				f *= (double) h;
			vect[j * Nu] *= f;
		}
	}
	for (uint_fast8_t j = diff; j < Nv; j++)
		for (uint_fast8_t i = 1; i < Nu; i++)
			vect[i + j * Nu] = vect[i + j * Nu - 1] * u_;
}

void Surface::Patch::UpdatePostSetup() {
	const size_t N = Nu * Nv;

	cx.assign(N, 0.0);
	cy.assign(N, 0.0);
	cz.assign(N, 0.0);
	mapu = Polynomial::ByValue(u0, 0, u1, 1);
	mapv = Polynomial::ByValue(v0, 0, v1, 1);
}

void Surface::Patch::UpdatePostSolve() {
	const size_t N = Nu * Nv;

	// Update the matrixes with the derivatives for calculating the normal
	// vectors.
	cdxdu.assign(N, 0.0);
	cdydu.assign(N, 0.0);
	cdzdu.assign(N, 0.0);
	cdxdv.assign(N, 0.0);
	cdydv.assign(N, 0.0);
	cdzdv.assign(N, 0.0);
	for (uint8_t j = 0; j < Nv; j++)
		for (uint8_t i = 1; i < Nu; i++) {
			cdxdu[j * Nu + i - 1] = cx[j * Nu + i] * (double) (i);
			cdydu[j * Nu + i - 1] = cy[j * Nu + i] * (double) (i);
			cdzdu[j * Nu + i - 1] = cz[j * Nu + i] * (double) (i);
		}
	for (uint8_t i = 0; i < Nu; i++) {
		for (uint8_t j = 1; j < Nv; j++) {
			cdxdv[(j - 1) * Nu + i] = cx[j * Nu + i] * (double) (j);
			cdydv[(j - 1) * Nu + i] = cy[j * Nu + i] * (double) (j);
			cdzdv[(j - 1) * Nu + i] = cz[j * Nu + i] * (double) (j);
		}
	}
}

void Surface::Patch::AddToGeometry(Geometry &geo) {
	UpdatePostSolve();

	const size_t Cu = 20; // Resolution in U
	const size_t Cv = 20; // Resolution in V

	// Starting vertex
	const size_t vert0 = geo.CountVertices();

//	Polynomial U0 = Polynomial::ByValue(0, 0.0, Cu, 1.0);
//	Polynomial V0 = Polynomial::ByValue(0, 0.0, Cv, 1.0);
	Polynomial U = Polynomial::ByValue(0, u0, Cu, u1);
	Polynomial V = Polynomial::ByValue(0, v0, Cv, v1);

	// Add vertices
	for (size_t j = 0; j <= Cv; j++) {
		for (size_t i = 0; i <= Cu; i++) {
			const double u = U((double) i);
			const double v = V((double) j);
			Geometry::Vertex vert = this->operator ()(u, v);
			geo.AddVertex(vert);
		}
	}

	// Add quads
	for (size_t j = 0; j < Cv; j++) {
		for (size_t i = 0; i < Cu; i++) {
			const size_t idx0 = vert0 + i + j * (Cu + 1);
			const size_t idx1 = vert0 + i + j * (Cu + 1) + 1;
			const size_t idx2 = vert0 + i + j * (Cu + 1) + 1 + (Cu + 1);
			const size_t idx3 = vert0 + i + j * (Cu + 1) + (Cu + 1);
			geo.AddQuad(idx0, idx1, idx2, idx3);
		}
	}
}

bool Surface::Boundary::IsContinuity() const {
	return u.empty() && v.empty() && values.empty();
}

bool Surface::Boundary::IsPoints() const {
	return (!u.empty() || !v.empty()) && !values.empty();
}

bool Surface::Boundary::IsPolynomial() const {
	return u.empty() && v.empty() && !values.empty();
}

bool Surface::Boundary::IsRelevantFor(const Patch &patch) const {
	// Check, if the patch is correct.

	if (allowPartialPatches) {
		// The patch has to overlap with the boundary condition at least a little.
		if (patch.u1 < u0 - DBL_EPSILON)
			return false;
		if (patch.u0 > u1 + DBL_EPSILON)
			return false;

		if (patch.v1 < v0 - DBL_EPSILON)
			return false;
		if (patch.v0 > v1 + DBL_EPSILON)
			return false;
		return true;
	}
	// Special conditions dependent on the boundary type.
	if (IsContinuity()) {
		// The patch-origin (u0, v0) has to be inside.
		if (patch.v0 > v1 + DBL_EPSILON || patch.v0 < v0 - DBL_EPSILON)
			return false;
		if (patch.u0 > u1 + DBL_EPSILON || patch.u0 < u0 - DBL_EPSILON)
			return false;
		// The endpoint u1 or v1 has to be inside. (The other coordinate was
		// already checked above. (u0,v1 or u1,v0).)
		if (patch.u1 > u1 + DBL_EPSILON && patch.v1 > v1 + DBL_EPSILON)
			return false;
	}

	if (patch.u0 < u0 - DBL_EPSILON)
		return false;
	if (patch.u1 > u1 + DBL_EPSILON)
		return false;

	if (patch.v0 < v0 - DBL_EPSILON)
		return false;
	if (patch.v1 > v1 + DBL_EPSILON)
		return false;

	return true;
}

void Surface::Clear() {
	Geometry::Clear();
	patches.clear();
	Ahard.Reset();
	Asoft.Reset();
	bhard.Reset();
	bsoft.Reset();
	boundariedFixed = false;
	debug.clear();
}

void Surface::Setup(size_t Nu, size_t Nv, double u0, double u1, double v0,
		double v1) {
	Polynomial mapU = Polynomial::ByValue(0, u0, Nu, u1);
	Polynomial mapV = Polynomial::ByValue(0, v0, Nv, v1);
	for (size_t j = 0; j < Nv; j++) {
		for (size_t i = 0; i < Nu; i++) {
			Patch p;
			p.u0 = mapU((double) i);
			p.v0 = mapV((double) j);
			p.u1 = mapU((double) i + 1);
			p.v1 = mapV((double) j + 1);

			// Set 4 as a standard value for 3rd order polynomials.
			// -> c0 + c1*x + c2*x*x + c3*x*x*x for each direction
			p.Nu = 4;
			p.Nv = 4;

			p.UpdatePostSetup();
			patches.push_back(p);
		}
	}
	gu0 = std::fmin(gu0, u0);
	gu1 = std::fmax(gu1, u1);
	gv0 = std::fmin(gv0, v0);
	gv1 = std::fmax(gv1, v1);
}

void Surface::SetOrder(size_t Nu_, size_t Nv_, double u0, double u1, double v0,
		double v1) {

	for (auto &p : patches)
		if (u0 - DBL_EPSILON <= p.u0 && u1 + DBL_EPSILON >= p.u1
				&& v0 - DBL_EPSILON <= p.v0 && v1 + DBL_EPSILON >= p.v1) {
			p.Nu = Nu_;
			p.Nv = Nv_;
			p.UpdatePostSetup();
		}
}

void Surface::ClearBoundaries() {
	boundaries.clear();
	softBoundaries = false;
}

void Surface::HardBoundaries() {
	softBoundaries = false;
}

void Surface::SoftBoundaries() {
	softBoundaries = true;
}

void Surface::AddStitching(double u0, double u1, double v0, double v1,
		size_t order, bool cyclicU, bool cyclicV, bool allowPartial) {
	Boundary b;
	b.u0 = u0;
	b.u1 = u1;
	b.v0 = v0;
	b.v1 = v1;
	b.order = order;
	b.cyclicU = cyclicU;
	b.cyclicV = cyclicV;
	if (softBoundaries)
		b.level = 1;
	else
		b.level = 0;
	b.allowPartialPatches = allowPartial;
	boundaries.push_back(b);
}

void Surface::AddPoint(const Geometry::Vertex &p) {
	Boundary b;
	b.u0 = p.u;
	b.u1 = p.u;
	b.v0 = p.v;
	b.v1 = p.v;
	b.values.push_back( { p.x, p.y, p.z });
	b.u.push_back(p.u);
	b.v.push_back(p.v);
	b.order = 0;
	if (softBoundaries)
		b.level = 1;
	else
		b.level = 0;
// As long as there is an overlap allow this boundary condition to apply.
// In the end this has to be decided on a point to point basis.
	b.allowPartialPatches = true;
	boundaries.push_back(b);
}

void Surface::AddPoint(double u, double v, const Vector3 &p) {
	Boundary b;
	b.u0 = u;
	b.u1 = u;
	b.v0 = v;
	b.v1 = v;
	b.values.push_back( { p.x, p.y, p.z });
	b.u.push_back(u);
	b.v.push_back(v);
	b.order = 0;
	if (softBoundaries)
		b.level = 1;
	else
		b.level = 0;
// As long as there is an overlap allow this boundary condition to apply.
// In the end this has to be decided on a point to point basis.
	b.allowPartialPatches = true;
	boundaries.push_back(b);
}

void Surface::AdddPdU(double u, double v, const Vector3 &dPdU) {
	Boundary b;
	b.u0 = u;
	b.u1 = u;
	b.v0 = v;
	b.v1 = v;
	b.values.push_back( { dPdU.x, dPdU.y, dPdU.z });
	b.u.push_back(u);
	b.v.push_back(v);
	b.order = 1;
	b.direction = Boundary::Direction::U;
	if (softBoundaries)
		b.level = 1;
	else
		b.level = 0;
// As long as there is an overlap allow this boundary condition to apply.
// In the end this has to be decided on a point to point basis.
	b.allowPartialPatches = true;
	boundaries.push_back(b);
}

void Surface::AdddPdV(double u, double v, const Vector3 &dPdV) {
	Boundary b;
	b.u0 = u;
	b.u1 = u;
	b.v0 = v;
	b.v1 = v;
	b.values.push_back( { dPdV.x, dPdV.y, dPdV.z });
	b.u.push_back(u);
	b.v.push_back(v);
	b.order = 1;
	b.direction = Boundary::Direction::V;
	if (softBoundaries)
		b.level = 1;
	else
		b.level = 0;
// As long as there is an overlap allow this boundary condition to apply.
// In the end this has to be decided on a point to point basis.
	b.allowPartialPatches = true;
	boundaries.push_back(b);
}

void Surface::AddPolynomial(double u0, double u1, double v0, double v1,
		const Polynomial3 &p) {
	Boundary b;
	b.u0 = u0;
	b.u1 = u1;
	b.v0 = v0;
	b.v1 = v1;
	if (fabs(u0 - u1) < fabs(v0 - v1))
		b.direction = Boundary::Direction::V;
	else
		b.direction = Boundary::Direction::U;
	if (softBoundaries)
		b.level = 1;
	else
		b.level = 0;

	const size_t N = p.x.size();
	if (p.y.size() != N || p.z.size() != N) {
		std::ostringstream err;
		err << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ << ") : ";
		err
				<< "The Polynomial3 does not contain the same number of coefficients in all of x, y, and z.";
		throw std::logic_error(err.str());
	}
	b.values.resize(N);
	for (size_t n = 0; n < N; n++) {
		b.values[n].x = p.x[n];
		b.values[n].y = p.y[n];
		b.values[n].z = p.z[n];
	}
// Each boundary condition only applies to some very specific connection
// line. Partial patch overlap is therefore not allowed.
	b.allowPartialPatches = true;
	boundaries.push_back(b);
}

//void Surface::AddStitching(size_t order, bool cyclicU, bool cyclicV) {
//
//	double au0 = DBL_MAX;
//	double av0 = DBL_MAX;
//	double au1 = -DBL_MAX;
//	double av1 = -DBL_MAX;
//	for (const auto &p : patches) {
//		au0 = fmin(au0, p.u0);
//		av0 = fmin(av0, p.v0);
//		au1 = fmax(au1, p.u1);
//		av1 = fmax(av1, p.v1);
//	}
//
//	const size_t N = Pos(patches.size());
//
//	for (size_t i = 0; i < patches.size(); i++) {
//		const auto &pt0 = patches[i];
//		if (fabs(pt0.u0 - au0) > FLT_EPSILON) {
//
//			for (size_t j = 0; j < patches.size(); j++) {
//				const auto &pt1 = patches[j];
//				if (fabs(pt0.u0 - pt1.u1) < FLT_EPSILON
//						&& fabs(pt0.v0 - pt1.v0) < FLT_EPSILON
//						&& fabs(pt0.v1 - pt1.v1) < FLT_EPSILON) {
//					size_t n = IncreaseAB(order * pt0.Nv);
//					std::vector<double> conn0;
//					std::vector<double> conn1;
//					for (uint8_t ord = 0; ord < order; ord++) {
//						pt0.FilldU(conn0, pt0.u0, pt0.v1, ord);
//						pt1.FilldU(conn1, pt1.u1, pt1.v1, ord);
//						for (size_t cp = 0; cp < pt0.Nv; cp++) {
//							const size_t offs0 = Pos(i, 0, cp) + n * N;
//							const size_t offs1 = Pos(j, 0, cp) + n * N;
//							SetA(conn0, cp * pt0.Nu, offs0, pt0.Nu, 1, 1, 1);
//							SetA(conn1, cp * pt0.Nu, offs1, pt0.Nu, 1, 1, -1);
//							n++;
//						}
//					}
//				}
//			}
//		} else if (cyclicU) {
//			for (size_t j = 0; j < patches.size(); j++) {
//				const auto &pt1 = patches[j];
//				if (fabs(au1 - pt1.u1) < FLT_EPSILON
//						&& fabs(pt0.v0 - pt1.v0) < FLT_EPSILON
//						&& fabs(pt0.v1 - pt1.v1) < FLT_EPSILON) {
//					size_t n = IncreaseAB(order * pt0.Nv);
//					std::vector<double> conn0;
//					std::vector<double> conn1;
//					for (uint8_t ord = 0; ord < order; ord++) {
//						pt0.FilldU(conn0, au0, pt0.v1, ord);
//						pt1.FilldU(conn1, au1, pt1.v1, ord);
//						for (size_t cp = 0; cp < pt0.Nv; cp++) {
//							const size_t offs0 = Pos(i, 0, cp) + n * N;
//							const size_t offs1 = Pos(j, 0, cp) + n * N;
//							SetA(conn0, cp * pt0.Nu, offs0, pt0.Nu, 1, 1, 1);
//							SetA(conn1, cp * pt0.Nu, offs1, pt0.Nu, 1, 1, -1);
//							n++;
//						}
//					}
//				}
//			}
//		}
//	}
//
//	for (size_t i = 0; i < patches.size(); i++) {
//		const auto &pt0 = patches[i];
//		if (fabs(pt0.v0 - av0) > FLT_EPSILON) {
//
//			for (size_t j = 0; j < patches.size(); j++) {
//				const auto &pt1 = patches[j];
//				if (fabs(pt0.v0 - pt1.v1) < FLT_EPSILON
//						&& fabs(pt0.u0 - pt1.u0) < FLT_EPSILON
//						&& fabs(pt0.u1 - pt1.u1) < FLT_EPSILON) {
//					size_t n = IncreaseAB(order * pt0.Nu);
//					std::vector<double> conn0;
//					std::vector<double> conn1;
//					for (uint8_t ord = 0; ord < order; ord++) {
//						pt0.FilldV(conn0, pt0.u1, pt0.v0, ord);
//						pt1.FilldV(conn1, pt1.u1, pt1.v1, ord);
//						for (size_t cp = 0; cp < pt0.Nu; cp++) {
//							const size_t offs0 = Pos(i, cp, 0) + n * N;
//							const size_t offs1 = Pos(j, cp, 0) + n * N;
//							SetA(conn0, cp, offs0, pt0.Nv, pt0.Nu, pt0.Nu, 1);
//							SetA(conn1, cp, offs1, pt0.Nv, pt0.Nu, pt0.Nu, -1);
//							n++;
//						}
//					}
//				}
//			}
//		} else if (cyclicV) {
//			for (size_t j = 0; j < patches.size(); j++) {
//				const auto &pt1 = patches[j];
//				if (fabs(av1 - pt1.v1) < FLT_EPSILON
//						&& fabs(pt0.u0 - pt1.u0) < FLT_EPSILON
//						&& fabs(pt0.u1 - pt1.u1) < FLT_EPSILON) {
//					size_t n = IncreaseAB(order * pt0.Nu);
//					std::vector<double> conn0;
//					std::vector<double> conn1;
//					for (uint8_t ord = 0; ord < order; ord++) {
//						pt0.FilldV(conn0, pt0.u1, av0, ord);
//						pt1.FilldV(conn1, pt1.u1, av1, ord);
//						for (size_t cp = 0; cp < pt0.Nu; cp++) {
//							const size_t offs0 = Pos(i, cp, 0) + n * N;
//							const size_t offs1 = Pos(j, cp, 0) + n * N;
//							SetA(conn0, cp, offs0, pt0.Nv, pt0.Nu, pt0.Nu, 1);
//							SetA(conn1, cp, offs1, pt0.Nv, pt0.Nu, pt0.Nu, -1);
//							n++;
//						}
//					}
//				}
//			}
//		}
//	}
//}
//
//void Surface::AddPoint(const Geometry::Vertex &vertex, double normalscale) {
//	const size_t N = Pos(patches.size());
//
//	const double u = vertex.u;
//	const double v = vertex.v;
//	size_t idx = (size_t) -1;
//	for (auto &p : patches) {
//		idx++;
//		if (u < p.u0 - DBL_EPSILON || u > p.u1 + DBL_EPSILON
//				|| v < p.v0 - DBL_EPSILON || v > p.v1 + DBL_EPSILON)
//			continue;
//		std::vector<double> x;
//		{
//			p.Fill(x, u, v);
//			const size_t n = IncreaseAB(1);
//			size_t offs = Pos(idx) + n * N;
//			SetA(x, 0, offs, p.Nv * p.Nu);
//			SetB(vertex, n * 3);
//		}
//		if (vertex.n.Abs2() > FLT_EPSILON) {
//			p.FilldV(x, u, v, 1);
//			const size_t n = IncreaseAB(1);
//			size_t offs = Pos(idx) + n * N;
//			SetA(x, 0, offs, p.Nv * p.Nu);
//			SetB(vertex.n * normalscale, n * 3);
//		}
//	}
//	//TODO Check if no patch was found.
//}
//
//void Surface::AdddU(const Geometry::Vertex &vertex, double normalscale) {
//	const size_t N = Pos(patches.size());
//	const double u = vertex.u;
//	const double v = vertex.v;
//	size_t idx = (size_t) -1;
//	for (auto &p : patches) {
//		idx++;
//		if (u < p.u0 - DBL_EPSILON || u > p.u1 + DBL_EPSILON
//				|| v < p.v0 - DBL_EPSILON || v > p.v1 + DBL_EPSILON)
//			continue;
//		std::vector<double> x;
//		p.FilldU(x, u, v, 1);
//		const size_t n = IncreaseAB(1);
//		size_t offs = Pos(idx) + n * N;
//		SetA(x, 0, offs, p.Nv * p.Nu);
//		SetB(vertex * normalscale, n * 3);
//	}
//	//TODO Check if no patch was found.
//}
//
//void Surface::AdddV(const Geometry::Vertex &vertex, double normalscale) {
//	const size_t N = Pos(patches.size());
//	const double u = vertex.u;
//	const double v = vertex.v;
//	size_t idx = (size_t) -1;
//	for (auto &p : patches) {
//		idx++;
//		if (u < p.u0 - DBL_EPSILON || u > p.u1 + DBL_EPSILON
//				|| v < p.v0 - DBL_EPSILON || v > p.v1 + DBL_EPSILON)
//			continue;
//		std::vector<double> x;
//		p.FilldV(x, u, v, 1);
//		const size_t n = IncreaseAB(1);
//		size_t offs = Pos(idx) + n * N;
//		SetA(x, 0, offs, p.Nv * p.Nu);
//		SetB(vertex * normalscale, n * 3);
//	}
//	//TODO Check if no patch was found.
//}
//
//void Surface::AddPolynomial(double u0, double u1, double v0, double v1,
//		const Polynomial3 &poly) {
//
//#ifdef DEBUG
//	{
//		auto bx = poly.x.GetBezier();
//		auto by = poly.y.GetBezier();
//		auto bz = poly.z.GetBezier();
//		for (size_t n = 0; n < bx.size(); n++)
//			debug.emplace_back(bx[n], by[n], bz[n]);
//	}
//#endif
//
//	const size_t N = Pos(patches.size());
//	if (fabs(u1 - u0) < DBL_EPSILON) {
//		// "Vertical"
//		size_t idx = (size_t) -1;
//		for (auto &p : patches) {
//			idx++;
//			if (u0 < p.u0 - DBL_EPSILON || u0 > p.u1 + DBL_EPSILON
//					|| fabs(v0 - p.v0) > DBL_EPSILON
//					|| fabs(v1 - p.v1) > DBL_EPSILON)
//				continue;
//			std::vector<double> x;
//			p.Fill(x, u0, p.v1);
//			size_t n = IncreaseAB(p.Nv);
//			for (size_t cp = 0; cp < p.Nv; cp++) {
//				size_t offs = Pos(idx, 0, cp) + n * N;
//				SetA(x, cp, offs, p.Nu, 1, 1);
//				SetB( { poly.x[cp], poly.y[cp], poly.z[cp] }, n * 3);
//				n++;
//			}
//		}
//		//TODO Check if no patch was found.
//
//	} else if (fabs(v1 - v0) < DBL_EPSILON) {
//		// "Horizontal"
//		size_t idx = (size_t) -1;
//		for (auto &p : patches) {
//			idx++;
//			if (v0 < p.v0 - DBL_EPSILON || v0 > p.v1 + DBL_EPSILON
//					|| fabs(u0 - p.u0) > DBL_EPSILON
//					|| fabs(u1 - p.u1) > DBL_EPSILON)
//				continue;
//			std::vector<double> x;
//			p.Fill(x, p.u1, v0);
//			size_t n = IncreaseAB(p.Nu);
//			for (size_t cp = 0; cp < p.Nu; cp++) {
//				size_t offs = Pos(idx, cp, 0) + n * N;
//				SetA(x, cp, offs, p.Nv, p.Nu, p.Nu);
//				SetB( { poly.x[cp], poly.y[cp], poly.z[cp] }, n * 3);
//				n++;
//			}
//		}
//		//TODO Check if no patch was found.
//
//	} else {
//		std::ostringstream err;
//		err << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ << ") : ";
//		err << "(u0 == u1) or (v0 == v1) is necessary.";
//		throw std::runtime_error(err.str());
//	}
//}

//void Surface::AddPatch(const Surface::Patch &other) {
//	const size_t N = Pos(patches.size());
//			size_t idx = (size_t) -1;
//			for (auto &p : patches) {
//				idx++;
//				if (u0 < p.u0 - DBL_EPSILON || u0 > p.u1 + DBL_EPSILON
//						|| fabs(v0 - p.v0) > DBL_EPSILON
//						|| fabs(v1 - p.v1) > DBL_EPSILON)
//					continue;
//				std::vector<double> x;
//				p.Fill(x, u0, p.v1);
//				size_t n = IncreaseAB(p.Nv);
//				for (size_t cp = 0; cp < p.Nv; cp++) {
//					size_t offs = Pos(idx, 0, cp) + n * N;
//					SetA(x, cp, offs, p.Nu, 1, 1);
//					SetB( { poly.x[cp], poly.y[cp], poly.z[cp] }, n * 3);
//					n++;
//				}
//			}
//
//}
//
//
//void Surface::AddSurface(const Surface &other) {
//}

size_t Surface::IncreaseAB(size_t additional) {
	size_t n = (size_t) -1;
	if (softBoundaries) {
		size_t N = Asoft.Size(0);
		if (N == 0) {
			N = Pos(patches.size());
			n = 0;
		} else {
			n = Asoft.Size(1);
		}
		Asoft.SetSize(N, n + additional);
		bsoft.SetSize(3, n + additional);
	} else {
		size_t N = Ahard.Size(0);
		if (N == 0) {
			N = Pos(patches.size());
			n = 0;
		} else {
			n = Ahard.Size(1);
		}
		Ahard.SetSize(N, n + additional);
		bhard.SetSize(3, n + additional);
	}
	return n;
}

void Surface::SetA(const std::vector<double> &values, size_t posFrom,
		size_t posTo, size_t count, size_t strideFrom, size_t strideTo,
		double factor) {

	size_t i = posFrom;
	size_t j = posTo;

	if (softBoundaries) {
		for (size_t n = 0; n < count; n++) {

			Asoft[j] = values[i] * factor;

			i += strideFrom;
			j += strideTo;
		}
	} else {
		for (size_t n = 0; n < count; n++) {

			Ahard[j] = values[i] * factor;

			i += strideFrom;
			j += strideTo;
		}
	}

}

void Surface::SetB(const Vector3 value, size_t posTo) {
	if (softBoundaries) {
		bsoft[posTo + 0] = value.x;
		bsoft[posTo + 1] = value.y;
		bsoft[posTo + 2] = value.z;
	} else {
		bhard[posTo + 0] = value.x;
		bhard[posTo + 1] = value.y;
		bhard[posTo + 2] = value.z;
	}
}

void Surface::Calculate() {
	const size_t N = Pos(patches.size());

	Ahard.Reset();
	Asoft.Reset();
	bhard.Reset();
	bsoft.Reset();
	Ahard.SetSize(N, 0);
	bhard.SetSize(3, 0);
	Asoft.SetSize(N, 0);
	bsoft.SetSize(3, 0);

	//TODO Shovel the code below into separate functions to reduce the cyclomatic-complexity. (Only _after_ it is tested.)

	for (int level = 0; level <= 1; level++) {
		softBoundaries = (level == 1);
		for (const Boundary &b : boundaries) {
			if (!b.active || b.level != level)
				continue;
			size_t idx = (size_t) -1;
			for (const Patch &p : patches) {
				idx++;
				if (!b.IsRelevantFor(p))
					continue;

				if (b.IsPoints()) {
					for (size_t idxp = 0; idxp < b.values.size(); idxp++) {
						std::vector<double> x;
						{
							if (b.order == 0) {
								p.Fill(x, b.u[idxp], b.v[idxp]);
							} else {
								if (b.direction == Boundary::Direction::U)
									p.FilldU(x, b.u[idxp], b.v[idxp], b.order);
								else
									p.FilldV(x, b.u[idxp], b.v[idxp], b.order);
							}

							const size_t n = IncreaseAB(1);
							size_t offs = Pos(idx) + n * N;
							SetA(x, 0, offs, p.Nv * p.Nu);
							SetB(b.values[idxp], n * 3);
						}
					}
				} else if (b.IsContinuity()) {
					// Continuity on vertical lines
					if (p.u0 > b.u0 - FLT_EPSILON && p.u0 < b.u1 + FLT_EPSILON
							&& ((p.v0 > b.v0 - FLT_EPSILON
									&& p.v1 < b.v1 + FLT_EPSILON)
									|| (b.allowPartialPatches
											&& p.v1 > b.v0 - FLT_EPSILON
											&& p.v0 < b.v1 + FLT_EPSILON))) {

						for (size_t idx2 = 0; idx2 < patches.size(); idx2++) {
							const auto &p2 = patches[idx2];
							if (!(fabs(p.v0 - p2.v0) < FLT_EPSILON
									&& fabs(p.v1 - p2.v1) < FLT_EPSILON
									&& (fabs(p.u0 - p2.u1) < FLT_EPSILON
											|| (b.cyclicU
													&& fabs(
															p.u0 - b.u0 - p2.u1
																	+ b.u1)
															< FLT_EPSILON))))
								continue;
							if (p.Nv != p2.Nv)
								continue;

							std::cout << "Adding U-continuity: from " << idx2
									<< " to " << idx << "\n";

							size_t n = IncreaseAB((b.order + 1) * p.Nv);
							std::vector<double> conn0;
							std::vector<double> conn1;
							for (uint8_t ord = 0; ord <= b.order; ord++) {
								p.FilldU(conn0, p.u0, p.v1, ord);
								p2.FilldU(conn1, p2.u1, p2.v1, ord);
								for (size_t cp = 0; cp < p.Nv; cp++) {
									const size_t offs0 = Pos(idx, 0, cp)
											+ n * N;
									const size_t offs1 = Pos(idx2, 0, cp)
											+ n * N;
									SetA(conn0, cp * p.Nu, offs0, p.Nu, 1, 1,
											1);
									SetA(conn1, cp * p2.Nu, offs1, p2.Nu, 1, 1,
											-1);
									n++;
								}
							}
						}
					}
					// Continuity on horizontal lines
					if (p.v0 > b.v0 - FLT_EPSILON && p.v0 < b.v1 + FLT_EPSILON
							&& ((p.u0 > b.u0 - FLT_EPSILON
									&& p.u1 < b.u1 + FLT_EPSILON)
									|| (b.allowPartialPatches
											&& p.u1 > b.u0 - FLT_EPSILON
											&& p.u0 < b.u1 + FLT_EPSILON))) {

						for (size_t idx2 = 0; idx2 < patches.size(); idx2++) {
							const auto &p2 = patches[idx2];
							if (!(fabs(p.u0 - p2.u0) < FLT_EPSILON
									&& fabs(p.u1 - p2.u1) < FLT_EPSILON
									&& (fabs(p.v0 - p2.v1) < FLT_EPSILON
											|| (b.cyclicV
													&& fabs(
															p.v0 - b.v0 - p2.v1
																	+ b.v1)
															< FLT_EPSILON))))
								continue;
							if (p.Nu != p2.Nu)
								continue;
							std::cout << "Adding V-continuity: from " << idx2
									<< " to " << idx << "\n";

							size_t n = IncreaseAB((b.order + 1) * p.Nu);
							std::vector<double> conn0;
							std::vector<double> conn1;
							for (uint8_t ord = 0; ord <= b.order; ord++) {
								p.FilldV(conn0, p.u1, p.v0, ord);
								p2.FilldV(conn1, p2.u1, p2.v1, ord);
								for (size_t cp = 0; cp < p.Nu; cp++) {
									const size_t offs0 = Pos(idx, cp, 0)
											+ n * N;
									const size_t offs1 = Pos(idx2, cp, 0)
											+ n * N;
									SetA(conn0, cp, offs0, p.Nv, p.Nu, p.Nu, 1);
									SetA(conn1, cp, offs1, p2.Nv, p2.Nu, p2.Nu,
											-1);
									n++;
								}
							}
						}
					}
				} else if (b.IsPolynomial()) {

					if (fabs(b.u1 - b.u0) < DBL_EPSILON) {
						// "Vertical"

						if (b.u0 < p.u0 - DBL_EPSILON
								|| b.u0 > p.u1 + DBL_EPSILON
								|| fabs(b.v0 - p.v0) > DBL_EPSILON
								|| fabs(b.v1 - p.v1) > DBL_EPSILON)
							continue;
						std::vector<double> x;
						p.Fill(x, b.u0, p.v1);
						size_t n = IncreaseAB(p.Nv);
						for (size_t cp = 0; cp < p.Nv; cp++) {
							size_t offs = Pos(idx, 0, cp) + n * N;
							SetA(x, cp, offs, p.Nu, 1, 1);
							SetB(
									{ b.values[cp].x, b.values[cp].y,
											b.values[cp].z }, n * 3);
							n++;
						}
					} else if (fabs(b.v1 - b.v0) < DBL_EPSILON) {
						// "Horizontal"

						if (b.v0 < p.v0 - DBL_EPSILON
								|| b.v0 > p.v1 + DBL_EPSILON
								|| fabs(b.u0 - p.u0) > DBL_EPSILON
								|| fabs(b.u1 - p.u1) > DBL_EPSILON)
							continue;
						std::vector<double> x;
						p.Fill(x, p.u1, b.v0);
						size_t n = IncreaseAB(p.Nu);
						for (size_t cp = 0; cp < p.Nu; cp++) {
							size_t offs = Pos(idx, cp, 0) + n * N;
							SetA(x, cp, offs, p.Nv, p.Nu, p.Nu);
							SetB(
									{ b.values[cp].x, b.values[cp].y,
											b.values[cp].z }, n * 3);
							n++;
						}

					} else {
						std::ostringstream err;
						err << __FILE__ << ":" << __FUNCTION__ << "("
								<< __LINE__ << ") : ";
						err << "(u0 == u1) or (v0 == v1) is necessary.";
						throw std::runtime_error(err.str());
					}

				} else {
					std::ostringstream err;
					err << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__
							<< ") : ";
					err << " The boundary condition could not be identified.";
					throw std::logic_error(err.str());
				}
			}
		}
	}

//	Ahard.ReorderDimensions(Matrix::Order::TWO_REVERSED);
//	bhard.ReorderDimensions(Matrix::Order::TWO_REVERSED);
//	Asoft.ReorderDimensions(Matrix::Order::TWO_REVERSED);
//	bsoft.ReorderDimensions(Matrix::Order::TWO_REVERSED);

#ifndef USE_EIGEN
	size_t n1 = Ahard.Size(1);
	if (n1 < N)
		n1 = N;
	Ahard.SetSize(N, n1);
	bhard.SetSize(3, n1);
	size_t n2 = Asoft.Size(1);
	if (n2 < N)
		n2 = N;
	Asoft.SetSize(N, n2);
	bsoft.SetSize(3, n2);
#endif

#ifdef USE_EIGEN
	{
		Exporter exp("/tmp/surf.mat");
		exp.Add(Ahard, "Ahard");
		exp.Add(bhard, "bhard");
		exp.Add(Asoft, "Asoft");
		exp.Add(bsoft, "bsoft");
	}

	Eigen::MatrixXd A0;
	Eigen::MatrixXd b0;
	Eigen::MatrixXd A1;
	Eigen::MatrixXd b1;

	A0.resize(Ahard.Size(1), Ahard.Size(0));
	A1.resize(Asoft.Size(1), Asoft.Size(0));
	b0.resize(bhard.Size(1), bhard.Size(0));
	b1.resize(bsoft.Size(1), bsoft.Size(0));

// Transpose and map to Eigen matrices.
	size_t p = 0;
	for (size_t i = 0; i < Ahard.Size(1); i++)
		for (size_t j = 0; j < Ahard.Size(0); j++)
			A0(i, j) = Ahard[p++];
	p = 0;
	for (size_t i = 0; i < Asoft.Size(1); i++)
		for (size_t j = 0; j < Asoft.Size(0); j++)
			A1(i, j) = Asoft[p++];

	p = 0;
	for (size_t i = 0; i < bhard.Size(1); i++)
		for (size_t j = 0; j < bhard.Size(0); j++)
			b0(i, j) = bhard[p++];
	p = 0;
	for (size_t i = 0; i < bsoft.Size(1); i++)
		for (size_t j = 0; j < bsoft.Size(0); j++)
			b1(i, j) = bsoft[p++];

#ifdef DEBUG
	std::cout << "A0: [" << A0.rows() << "x" << A0.cols() << "]\n";
	std::cout << "A1: [" << A1.rows() << "x" << A1.cols() << "]\n";
	std::cout << "b0: [" << b0.rows() << "x" << b0.cols() << "]\n";
	std::cout << "b1: [" << b1.rows() << "x" << b1.cols() << "]\n";
#endif

//	Eigen::MatrixXd c = A0.completeOrthogonalDecomposition().solve(b0);

//	Eigen::MatrixXd pinvA0 =
//			A0.completeOrthogonalDecomposition().pseudoInverse();
//	Eigen::MatrixXd J = pinvA0 * b0; // @suppress("Invalid arguments")
//	Eigen::MatrixXd H = Eigen::MatrixXd::Identity(A0.cols(), A0.cols())
//			- pinvA0 * A0;
//
//	Eigen::MatrixXd K = A1 * H; // @suppress("Invalid arguments")
//	Eigen::MatrixXd pinvA1 =
//			K.completeOrthogonalDecomposition().pseudoInverse();
//	Eigen::MatrixXd w = pinvA1 * (b1 - A1 * J); // @suppress("Invalid arguments")
//
//	Eigen::MatrixXd c = J + H * w;

	Eigen::CompleteOrthogonalDecomposition Dec0 =
			A0.completeOrthogonalDecomposition();
	Eigen::MatrixXd J = Dec0.solve(b0); // @suppress("Invalid arguments")
	Eigen::MatrixXd H = Eigen::MatrixXd::Identity(A0.cols(), A0.cols())
			- Dec0.solve(A0);
	Eigen::MatrixXd K = A1 * H; // @suppress("Invalid arguments")
	Eigen::CompleteOrthogonalDecomposition Dec1 =
			K.completeOrthogonalDecomposition();
	Eigen::MatrixXd w = Dec1.solve(b1 - A1 * J); // @suppress("Invalid arguments")
	Eigen::MatrixXd c = J + H * w;

	const size_t N1 = N - Dec0.rank();
	const size_t N2 = N1 - Dec1.rank();
#ifdef DEBUG
	std::cout << "Exact solution: DOF " << N << " -> " << N1 << '\n';
	std::cout << "Interpolation: DOF " << N1 << " -> " << N2 << '\n';
#endif
#else
	Ahard.Transpose();
	bhard.Transpose();
	Asoft.Transpose();
	bsoft.Transpose();

#ifdef DEBUG
//	PrintProblem(Ahard, bhard);

//	for (size_t n = 0; n < bhard.Size(0); n++)
//		debug.emplace_back(bhard(n, 0), bhard(n, 1), bhard(n, 2));
//	for (size_t n = 0; n < bsoft.Size(0); n++)
//		debug.emplace_back(bsoft(n, 0), bsoft(n, 1), bsoft(n, 2));
#endif

	SVD svdhard;
	SVD svdsoft;

	svdhard.Decompose(Ahard);

#ifdef DEBUG
	{
		Exporter exp("/tmp/surf.mat");
		exp.AddPoint(Ahard, "Ahard");
		exp.AddPoint(bhard, "bhard");
		exp.AddPoint(Asoft, "Asoft");
		exp.AddPoint(bsoft, "bsoft");
		exp.AddPoint(svdhard.U, "U");
		exp.AddPoint(svdhard.W, "S");
		exp.AddPoint(svdhard.V, "V");
	}
#endif

	bool combinedSolution = true;

	Matrix c;
	if (combinedSolution) {
		Matrix J = svdhard.Solve(bhard, 100);
		Matrix H = svdhard.Variation(100);
		svdsoft.Decompose(Asoft * H);
		Matrix w = svdsoft.Solve(bsoft - Asoft * J, 100);
		c = J + H * w;
	} else {
		Matrix J = svdhard.Solve(bhard, 100);
		Matrix H = svdhard.Variation(100);

		Matrix w;
		w.SetSize(H.Size(1), J.Size(1));

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(-0.01, 0.01);
		for (int n = 0; n < w.Numel(); n++)
			w[n] = dis(gen);

		c = J + H * w;
	}

#endif

	// Note that the classes Matrix and Eigen::MatrixXd have the same interface
	// In both cases c is of a different type but can be accessed the same way.

	// Map the solution into the patches.
	size_t offs = 0;
	for (auto &p : patches) {
		const size_t Np = p.Nu * p.Nv;
		for (size_t m = 0; m < Np; m++)
			p.cx[m] = c(offs + m, 0);
		for (size_t m = 0; m < Np; m++)
			p.cy[m] = c(offs + m, 1);
		for (size_t m = 0; m < Np; m++)
			p.cz[m] = c(offs + m, 2);
		offs += Np;
	}

}

Geometry::Vertex Surface::operator ()(double u, double v) const {
	size_t check = patches.size();
	if (lastSearchedPatch >= patches.size())
		lastSearchedPatch = 0;
	while (u <= -M_PI)
		u += 2.0 * M_PI;
	while (u > M_PI)
		u -= 2.0 * M_PI;
	if (v > gv1)
		v = gv1;
	if (v < gv0)
		v = gv0;

	while (check > 0) {
		if (patches[lastSearchedPatch].IsInside(u, v))
			break;
		lastSearchedPatch = (lastSearchedPatch + 1) % patches.size();
		check--;
	}
	if (check == 0) {
		std::ostringstream err;
		err << __FILE__ << ":" << __FUNCTION__ << "(" << __LINE__ << ") : ";
		err << " The position u=" << u << ", v=" << v
				<< " is outside of what is covered by the coordinate system.";
		throw std::logic_error(err.str());
	}
	Geometry::Vertex ret = patches[lastSearchedPatch](u, v);
	ret.u = u;
	ret.v = v;
	return ret;
}

void Surface::Apply(Geometry &geo) {
	const size_t vc = geo.CountVertices();
	for (size_t n = 0; n < vc; n++) {
		Geometry::Vertex &vert = geo[n];
		Geometry::Vertex addi = this->operator ()(vert.u, vert.v);
		vert.x = addi.x;
		vert.y = addi.y;
		vert.z = addi.z;
		vert.n = addi.n;
	}
	geo.FlagNormals(true, false, false);
	geo.CalculateUVCoordinateSystems();
}

void Surface::Update() {
	Geometry::Clear();
	SetAddColor(1.0, 1.0, 0.0);
	for (auto &patch : patches)
		patch.AddToGeometry(*this);
	this->verticesHaveTextur = true;
	this->verticesHaveColor = false;
	Geometry::PropagateNormals();
	Geometry::CalculateUVCoordinateSystems();
}

void Surface::Paint() const {
	Geometry::Paint();
#ifdef DEBUG
	glPointSize(5);
	glBegin(GL_POINTS);
	glNormal3f(0.0f, 1.0f, 0.2f);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (const auto &point : debug) {
//		glColor3d(point.u, point.v, 0.0);
		glVertex3d(point.x, point.y, point.z);
	}
	glEnd();
	glPointSize(1);
#endif
}

size_t Surface::Pos(size_t nPatch, uint8_t idxU, uint8_t idxV) const {
	size_t pos = 0;
	for (size_t idx = 0; idx < nPatch; idx++)
		pos += patches[idx].Nu * patches[idx].Nv;
// Early exit, if the Pos command is run with nPatch = patches.size().
// In this case the total number of coefficients is returned.
	if (idxU == 0 && idxV == 0)
		return pos;
	pos += patches[nPatch].Nu * idxV;
	pos += idxU;
	return pos;
}

void Surface::PrintProblem(const Matrix &A, const Matrix &b) const {

	std::vector<std::string> varnames;
	for (size_t i = 0; i < patches.size(); i++) {
		for (size_t j = 0; j < patches[i].Nv; j++) {
			for (size_t k = 0; k < patches[i].Nu; k++) {
				std::ostringstream out;
				out << "Cp" << i;
				out << "u" << k;
				out << "v" << j;
				varnames.push_back(out.str());
			}
		}
	}

	std::ostringstream r;
	for (size_t i = 0; i < A.Size(0); i++) {
		std::ostringstream lhs;
		std::ostringstream rhs;
		bool lhsfirst = true;
		bool rhsfirst = true;
		for (size_t j = 0; j < A.Size(1); j++) {
			const double v = A(i, j);
			if (fabs(v) < FLT_EPSILON)
				continue;

			if (v > 0.0) {
				if (lhsfirst)
					lhsfirst = false;
				else
					lhs << " + ";
				if (fabs(v - 1.0) > FLT_EPSILON)
					lhs << v << "*";
				lhs << varnames[j];

			} else {
				if (rhsfirst)
					rhsfirst = false;
				else
					rhs << " + ";
				if (fabs(v + 1.0) > FLT_EPSILON)
					rhs << -v << "*";
				rhs << varnames[j];
			}
		}
		if (fabs(b(i, 0)) > FLT_EPSILON || fabs(b(i, 1)) > FLT_EPSILON
				|| fabs(b(i, 2)) > FLT_EPSILON) {
			if (!rhsfirst)
				rhs << " + ";
			rhs << "[" << b(i, 0) << "," << b(i, 1) << "," << b(i, 2) << "]";
		}
		if (!rhsfirst || !lhsfirst) {
			r << i + 1 << ": " << lhs.str() << " = " << rhs.str() << '\n';
		}
	}
	std::cout << r.str();
}

Geometry Surface::ExtractByUVPlane(double u_, double v_, double d) {
	UnselectAll();
	for (size_t i = 0; i < v.size(); i++) {
		const Geometry::Vertex &vert = v[i];
		const double temp = vert.u * u_ + vert.v * v_;
		if (temp >= d - DBL_EPSILON)
			selected.insert(i);
	}
	Geometry ret;
	ret.CopyPropertiesFrom(*this);
	ret.AddSelectedFrom(*this);
	return ret;
}

