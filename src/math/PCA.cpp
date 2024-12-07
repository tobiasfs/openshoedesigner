///////////////////////////////////////////////////////////////////////////////
// Name               : PCA.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 05.05.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "PCA.h"

#include <cfloat>

#include "../3D/OpenGL.h"

PCA::PCA() {
	Reset();
}

void PCA::Reset() {
	weight = 0.0;
	X.Set(1, 0, 0);
	Y.Set(0, 1, 0);
	Z.Set(0, 0, 1);
	eX = eY = eZ = 0.0;
	xx = xy = xz = yy = yz = zz = 0.0;
}

void PCA::SetCenter(const Vector3 &center) {
	this->center = center;
}

void PCA::Add(const Vector3 &point, double weight_) {
	const double x = point.x - center.x;
	const double y = point.y - center.y;
	const double z = point.z - center.z;
	xx += x * x;
	xy += x * y;
	xz += x * z;
	yy += y * y;
	yz += y * z;
	zz += z * z;
	weight += weight_;
}

void PCA::Calculate() {
	if (weight < FLT_EPSILON)
		return;

	// Row-wise defined matrix A to analyse:
	Vector3 A1(xx / weight, xy / weight, xz / weight);
	Vector3 A2(xy / weight, yy / weight, yz / weight);
	Vector3 A3(xz / weight, yz / weight, zz / weight);

	// Initialize Q to the eye matrix
	Vector3 Q1(1, 0, 0);
	Vector3 Q2(0, 1, 0);
	Vector3 Q3(0, 0, 1);

	// Matrix to store R
	Vector3 R1, R2, R3;

	// QR iteration:
	size_t iter = 0;
	while (iter < maxIterations) {
		// W = A*Q
		const Vector3 W1 = A1 * Q1.x + A2 * Q1.y + A3 * Q1.z;
		const Vector3 W2 = A1 * Q2.x + A2 * Q2.y + A3 * Q2.z;
		const Vector3 W3 = A1 * Q3.x + A2 * Q3.y + A3 * Q3.z;

		// Decompose W into Q and R
		const Vector3 u1 = W1;
		const Vector3 p11 = u1 * (u1.Dot(W2) / u1.Abs2());
		const Vector3 u2 = W2 - p11;
		const Vector3 p21 = u1 * (u1.Dot(W3) / u1.Abs2());
		const Vector3 p22 = u2 * (u2.Dot(W3) / u2.Abs2());
		const Vector3 u3 = W3 - p21 - p22;
		Q1 = u1;
		Q2 = u2;
		Q3 = u3;
		Q1.Normalize();
		Q2.Normalize();
		Q3.Normalize();
		R1.Set(Q1.Dot(W1), 0, 0);
		R2.Set(Q1.Dot(W2), Q2.Dot(W2), 0);
		R3.Set(Q1.Dot(W3), Q2.Dot(W3), Q3.Dot(W3));

		const double error = R2.x * R2.x + R3.x * R3.x + R3.y * R3.y;
		if (error < maxError)
			break;
		iter++;
	}

	// After around a hand full iterations, Q contains in its columns the eigenvectors of the matrix A.
	// The diagonal of R contains the (approximate) eigenvalues.
	if (R1.x > R2.y && R1.x > R3.z) {
		X = Q1;
		if (R2.y > R3.z) {
			Y = Q2;
			Z = Q3;
		} else {
			Y = Q3;
			Z = Q2;
		}
	}
	if (R2.y > R1.x && R2.y > R3.z) {
		X = Q2;
		if (R1.x > R3.z) {
			Y = Q1;
			Z = Q3;
		} else {
			Y = Q3;
			Z = Q1;
		}
	}
	if (R3.z > R1.x && R3.z > R2.y) {
		X = Q3;
		if (R1.x > R2.y) {
			Y = Q1;
			Z = Q2;
		} else {
			Y = Q2;
			Z = Q1;
		}
	}

	// Calculate the exact eigenvalues using the Rayleight quotient:
	eX = (A1.Dot(X) * X.x + A2.Dot(X) * X.y + A3.Dot(X) * X.z) / (X.Dot(X));
	eY = (A1.Dot(Y) * Y.x + A2.Dot(Y) * Y.y + A3.Dot(Y) * Y.z) / (Y.Dot(Y));
	eZ = (A1.Dot(Z) * Z.x + A2.Dot(Z) * Z.y + A3.Dot(Z) * Z.z) / (Z.Dot(Z));

//	std::cout << "Eigenvector X: " << X << " : " << eX << "\n";
//	std::cout << "Eigenvector Y: " << Y << " : " << eY << "\n";
//	std::cout << "Eigenvector Z: " << Z << " : " << eZ << "\n";
}

void PCA::Paint() const {
	glPushMatrix();
	glTranslatef(center.x, center.y, center.z);
	Vector3 temp;
	glBegin(GL_LINES);
	temp = X * eX;
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(temp.x, temp.y, temp.z);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(-temp.x, -temp.y, -temp.z);

	temp = Y * eY;
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(temp.x, temp.y, temp.z);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(-temp.x, -temp.y, -temp.z);

	temp = Z * eZ;
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(temp.x, temp.y, temp.z);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(0, 0, 0);
	glVertex3f(-temp.x, -temp.y, -temp.z);

	glEnd();
	glPopMatrix();
}
