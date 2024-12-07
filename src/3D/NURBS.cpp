///////////////////////////////////////////////////////////////////////////////
// Name               : NURBS.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 26.02.2018
// Copyright          : (C) 2018 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "NURBS.h"

#include <cassert>
#include <cmath>

#include "OpenGL.h"

NURBS::NURBS() {
	cyclicU = false;
	cyclicV = false;
	nU = 0;
	nV = 0;
	lastU = 0;
	lastV = 0;
	insertpos = 0;
	resolutionU = 25;
	resolutionV = 25;
	subresolutionU = 40;
	subresolutionV = 40;
	controlPointSize = 0;
}

NURBS::~NURBS() {
}

void NURBS::Initialize(size_t nu, size_t nv, bool cyclicU, bool cyclicV,
		bool normalizeU, bool normalizeV) {
	nU = nu;
	nV = nv;
	assert(nU >= 2);
	assert(nV >= 2);
	if (nU < 3)
		cyclicU = false;
	if (nV < 3)
		cyclicV = false;

	c.resize(nU * nV);
	w.resize(nU * nV);
	insertpos = 0;

	this->cyclicU = cyclicU;
	this->cyclicV = cyclicV;
	lastU = 0;
	lastV = 0;

	knotU.resize(nU + (cyclicU ? 6 : 4));
	knotV.resize(nV + (cyclicV ? 6 : 4));
	posU.resize(nU + (cyclicU ? 6 : 4));
	posV.resize(nV + (cyclicV ? 6 : 4));

	if (cyclicU) {
		for (size_t n = 0; n < (nU + 6); n++)
			knotU[n] = (double) n - ((double) nU + 5.0) / 2.0;
		for (size_t n = 0; n < nU; n++)
			posU[n + 3] = n;
		posU[0] = nU - 3;
		posU[1] = nU - 2;
		posU[2] = nU - 1;
		posU[nU + 3] = 0;
		posU[nU + 4] = 1;
		posU[nU + 5] = 2;
	} else {
		for (size_t n = 0; n < nU; n++)
			posU[n + 2] = n;
		posU[0] = 0;
		posU[1] = 0;
		posU[nU + 2] = nU - 1;
		posU[nU + 3] = nU - 1;
		for (size_t n = 0; n < (nU + 4); n++)
			knotU[n] = (double) posU[n];
	}
	if (cyclicV) {
		for (size_t n = 0; n < (nV + 6); n++)
			knotV[n] = (double) n - ((double) nV + 5.0) / 2.0;
		for (size_t n = 0; n < nV; n++)
			posV[n + 3] = n;
		posV[0] = nV - 3;
		posV[1] = nV - 2;
		posV[2] = nV - 1;
		posV[nV + 3] = 0;
		posV[nV + 4] = 1;
		posV[nV + 5] = 1;
	} else {
		for (size_t n = 0; n < nV; n++)
			posV[n + 2] = n;
		posV[0] = 0;
		posV[1] = 0;
		posV[nV + 2] = nV - 1;
		posV[nV + 3] = nV - 1;
		for (size_t n = 0; n < (nV + 4); n++)
			knotV[n] = (double) posV[n];
	}
	if (normalizeU) {
		if (cyclicU) {
			for (size_t n = 0; n < nU + 6; n++)
				knotU[n] /= (double) (nU) / 2.0;
		} else {
			for (size_t n = 0; n < nU + 4; n++)
				knotU[n] /= (double) (nU - 1);
		}
	}
	if (normalizeV) {
		if (cyclicV) {
			for (size_t n = 0; n < nV + 6; n++)
				knotV[n] /= (double) (nV) / 2.0;
		} else {
			for (size_t n = 0; n < nV + 4; n++)
				knotV[n] /= (double) (nV - 1);
		}
	}
}

void NURBS::InsertData(Vector3 v, double w) {
	this->c[insertpos] = v;
	this->w[insertpos] = w;
	insertpos++;
}

Vector3 NURBS::Position(double u, double v) const {
	const size_t maxU = knotU.size() - 4;
	const size_t maxV = knotV.size() - 4;

	while (lastU < maxU && knotU[lastU + 1] <= u)
		lastU++;
	while (lastU > 2 && knotU[lastU] >= u)
		lastU--;
	while (lastV < maxV && knotV[lastV + 1] <= v)
		lastV++;
	while (lastV > 2 && knotV[lastV] >= v)
		lastV--;

	double dU[4];
	double dV[4];

	{
		const double T0 = knotU[lastU - 2];
		const double T1 = knotU[lastU - 1];
		const double T2 = knotU[lastU];
		const double T3 = knotU[lastU + 1];
		const double T4 = knotU[lastU + 2];
		const double T5 = knotU[lastU + 3];

		const double a13 = (u - T5) / (T2 - T5);
		const double a12 = (u - T4) / (T1 - T4);
		const double a11 = (u - T3) / (T0 - T3);
		const double a23 = (u - T4) / (T2 - T4);
		const double a22 = (u - T3) / (T1 - T3);
		const double a33 = (u - T3) / (T2 - T3);

		const double H2 = a12 * a23;
		dU[0] = a11 * a22 * a33;
		dU[1] = (-H2 + (-a12 - a11 + 1) * a22 + a12) * a33 + H2;
		dU[2] = ((a13 + a12 - 1) * a23 + (a12 - 1) * a22 - a13 - a12 + 1) * a33
				+ (-a13 - a12 + 1) * a23 + a13;
		dU[3] = ((-a13 + 1) * a23 + a13 - 1) * a33 + (a13 - 1) * a23 - a13 + 1;
	}
	{
		const double T0 = knotV[lastV - 2];
		const double T1 = knotV[lastV - 1];
		const double T2 = knotV[lastV];
		const double T3 = knotV[lastV + 1];
		const double T4 = knotV[lastV + 2];
		const double T5 = knotV[lastV + 3];

		const double a13 = (v - T5) / (T2 - T5);
		const double a12 = (v - T4) / (T1 - T4);
		const double a11 = (v - T3) / (T0 - T3);
		const double a23 = (v - T4) / (T2 - T4);
		const double a22 = (v - T3) / (T1 - T3);
		const double a33 = (v - T3) / (T2 - T3);

		const double H2 = a12 * a23;
		dV[0] = a11 * a22 * a33;
		dV[1] = (-H2 + (-a12 - a11 + 1) * a22 + a12) * a33 + H2;
		dV[2] = ((a13 + a12 - 1) * a23 + (a12 - 1) * a22 - a13 - a12 + 1) * a33
				+ (-a13 - a12 + 1) * a23 + a13;
		dV[3] = ((-a13 + 1) * a23 + a13 - 1) * a33 + (a13 - 1) * a23 - a13 + 1;
	}

	double Wref = 0.0;
	for (uint_fast8_t n = 0; n < 4; n++) {
		for (uint_fast8_t m = 0; m < 4; m++) {
			Wref += w[posU[lastU - 1 + n] + posV[lastV - 1 + m] * nU] * dU[n]
					* dV[m];
		}
	}

	Vector3 x;
	for (uint_fast8_t n = 0; n < 4; n++) {
		for (uint_fast8_t m = 0; m < 4; m++) {
			x += c[posU[lastU - 1 + n] + posV[lastV - 1 + m] * nU]
					* (w[posU[lastU - 1 + n] + posV[lastV - 1 + m] * nU] * dU[n]
							* dV[m]);
		}
	}
	x /= Wref;
	return x;
}

Vector3 NURBS::Normal(double u, double v) const {
	const double eps = 1e-3;
	const Vector3 p0 = Position(u, v);
	const Vector3 p1 = Position(u + eps, v);
	const Vector3 p2 = Position(u, v + eps);
	Vector3 n = (p1 - p0) * (p2 - p0);
	n.Normalize();
	return n;
}

double NURBS::GetU(int block, int subBlock) {
	double u0 = knotU[2];
	double u1 = knotU[nU + 1];
	if (cyclicU) {
		u0 = (u0 + knotU[3]) / 2.0;
		u1 = (knotU[nU + 2] + knotU[nU + 3]) / 2.0;
	}
	const double du = (u1 - u0) / (double) resolutionU;
	const double dsu = (u1 - u0) / (double) (resolutionU * subresolutionU);
	if (subBlock < 0)
		return u0 + ((double) block + 0.5) * du;
	else
		return u0 + (double) block * du + ((double) subBlock + 0.5) * dsu;
}

double NURBS::GetV(int block, int subBlock) {
	double v0 = knotV[2];
	double v1 = knotV[nV + 1];
	if (cyclicV) {
		v0 = (v0 + knotV[3]) / 2.0;
		v1 = (knotV[nV + 2] + knotV[nV + 3]) / 2.0;
	}
	const double dv = (v1 - v0) / (double) resolutionV;
	const double dsv = (v1 - v0) / (double) (resolutionV * subresolutionV);

	if (subBlock < 0)
		return v0 + ((double) block + 0.5) * dv;
	else
		return v0 + (double) block * dv + ((double) subBlock + 0.5) * dsv;
}

void NURBS::Paint(void) const {
	if (controlPointSize > 0) {
		glBegin(GL_POINTS);
		glColor3f(1, 1, 0);
		glPointSize(controlPointSize);
		for (size_t n = 0; n < c.size(); n++) {
			double x = c[n].x;
			double y = c[n].y;
			double z = c[n].z;
			const double L = sqrt(x * x + y * y + z * z);
			glNormal3d(x / L, y / L, z / L);
			glVertex3d(x, y, z);
		}
		glEnd();
	}

	double u0 = knotU[2];
	double u1 = knotU[nU + 1];
	if (cyclicU) {
		u0 = (u0 + knotU[3]) / 2.0;
		u1 = (knotU[nU + 2] + knotU[nU + 3]) / 2.0;
	}
	const double du = (u1 - u0) / (double) resolutionU;

	double v0 = knotV[2];
	double v1 = knotV[nV + 1];
	if (cyclicV) {
		v0 = (v0 + knotV[3]) / 2.0;
		v1 = (knotV[nV + 2] + knotV[nV + 3]) / 2.0;
	}
	const double dv = (v1 - v0) / (double) resolutionV;

	bool reenableCulling = glIsEnabled(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	double u = u0;
	for (size_t n = 0; n < resolutionU; n++) {
		glPushName((int) n);
		double v = v0;
		for (double m = 0; m < resolutionV; m++) {
			glPushName((int) m);

			const Vector3 p0 = Position(u, v);
			const Vector3 p1 = Position(u + du, v);
			const Vector3 p2 = Position(u + du, v + dv);
			const Vector3 p3 = Position(u, v + dv);
			Vector3 n = (p1 - p0) * (p3 - p0);
			n.Normalize();
			glNormal3d(n.x, n.y, n.z);
			glBegin(GL_QUADS);
			glVertex3d(p0.x, p0.y, p0.z);
			glVertex3d(p1.x, p1.y, p1.z);
			glVertex3d(p2.x, p2.y, p2.z);
			glVertex3d(p3.x, p3.y, p3.z);
			glEnd();

			glPopName();
			v += dv;
		}
		glPopName();
		u += du;
	}
	if (reenableCulling)
		glEnable(GL_CULL_FACE);
}

void NURBS::Paint(int blockU, int blockV) const {
//	glBlendEquation(1);
}
