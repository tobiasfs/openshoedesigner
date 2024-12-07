///////////////////////////////////////////////////////////////////////////////
// Name               : InsoleConstruct.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 12.11.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#include "InsoleConstruct.h"

#include "../../math/DependentVector.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

InsoleConstruct::InsoleConstruct() {
	insole = std::make_shared<Insole>();
}

bool InsoleConstruct::CanRun() {
	std::ostringstream err;
	err << __FILE__ << ":" << __LINE__ << ":" << __func__ << " -";
	bool hasMissingConnection = false;
	if (!footLength) {
		hasMissingConnection = true;
		err << " Input \"footLength\" not connected.";
	}
	if (!footLength) {
		hasMissingConnection = true;
		err << " Input \"footLength\" not connected.";
	}
	if (!ballMeasurementAngle) {
		hasMissingConnection = true;
		err << " Input \"ballMeasurementAngle\" not connected.";
	}
	if (!heelDirectionAngle) {
		hasMissingConnection = true;
		err << " Input \"heelDirectionAngle\" not connected.";
	}
	if (!littleToeAngle) {
		hasMissingConnection = true;
		err << " Input \"littleToeAngle\" not connected.";
	}
	if (!bigToeAngle) {
		hasMissingConnection = true;
		err << " Input \"bigToeAngle\" not connected.";
	}
	if (!ballWidth) {
		hasMissingConnection = true;
		err << " Input \"ballWidth\" not connected.";
	}
	if (!heelWidth) {
		hasMissingConnection = true;
		err << " Input \"heelWidth\" not connected.";
	}
	if (!extraLength) {
		hasMissingConnection = true;
		err << " Input \"extraLength\" not connected.";
	}
	if (!insole) {
		hasMissingConnection = true;
		err << " Output \"insole\" not set.";
	}
	if (hasMissingConnection)
		throw std::runtime_error(err.str());
	return true;
}

bool InsoleConstruct::Propagate() {
	bool modify = false;
	if (!CanRun())
		return modify;

	bool modified = false;
	modified |= footLength->IsModified();
	modified |= ballMeasurementAngle->IsModified();
	modified |= heelDirectionAngle->IsModified();
	modified |= littleToeAngle->IsModified();
	modified |= bigToeAngle->IsModified();
	modified |= ballWidth->IsModified();
	modified |= heelWidth->IsModified();
	modified |= extraLength->IsModified();

	if (modified) {
		modify |= insole->IsValid();
		insole->MarkValid(false);
	}
	return modify;
}

bool InsoleConstruct::HasToRun() {
	if (!CanRun())
		return false;
	return !insole->IsValid() && insole->IsNeeded();
}

void InsoleConstruct::Run() {
	Construct();
	insole->MarkValid(true);
	insole->MarkNeeded(false);
}

void InsoleConstruct::Construct() {
	// A and B have to be on the coordinate y = 0.0

	insole->J.p.Set(0, 0, 0);
	insole->A.p = insole->J.p - Vector3(footLength->ToDouble() / 6.0, 0, 0);
	insole->C.p = insole->A.p + Vector3(footLength->ToDouble() * 0.62, 0, 0);
	insole->D.p = insole->A.p + Vector3(footLength->ToDouble(), 0, 0);
	insole->B.p = insole->A.p
			+ Vector3(footLength->ToDouble() + extraLength->ToDouble(), 0, 0);

	{
		const double s = sin(ballMeasurementAngle->ToDouble());
		const double c = cos(ballMeasurementAngle->ToDouble());

		insole->E.p = insole->C.p
				+ Vector3(s * ballWidth->ToDouble() / 2,
						c * ballWidth->ToDouble() / 2, 0);
		insole->F.p = insole->C.p
				- Vector3(s * ballWidth->ToDouble() / 2 * 1.24,
						c * ballWidth->ToDouble() / 2 * 1.24, 0);
	}
	{
		const double s = sin(heelDirectionAngle->ToDouble());
		const double c = cos(heelDirectionAngle->ToDouble());
		insole->K.p = insole->J.p
				+ Vector3(s * heelWidth->ToDouble() / 2,
						c * heelWidth->ToDouble() / 2, 0);
		insole->L.p = insole->J.p
				- Vector3(s * heelWidth->ToDouble() / 2,
						c * heelWidth->ToDouble() / 2, 0);
		insole->N.p.x = insole->A.p.x * c;
		insole->N.p.y = -insole->A.p.x * s;
		insole->N.p.z = insole->A.p.z;
	}
	{
		const double s = sin(bigToeAngle->ToDouble());
		const double c = cos(bigToeAngle->ToDouble());
		const double r = (insole->D.p.x - insole->E.p.x) * c;
		insole->G.p = insole->E.p + Vector3(r, -s * r, 0);
	}
	{
		const double s = sin(littleToeAngle->ToDouble());
		const double c = cos(littleToeAngle->ToDouble());
		const double r = (insole->D.p.x - insole->F.p.x) * c;
		insole->H.p = insole->F.p + Vector3(r, s * r, 0);
	}
	insole->Z.p = insole->H.p - Vector3(footLength->ToDouble() / 5, 0, 0);

	// Normals
	insole->N.SetNormal(insole->K, insole->L);
	{
		const double s = sin(heelDirectionAngle->ToDouble());
		const double c = cos(heelDirectionAngle->ToDouble());
		insole->K.n.Set(-c, s, 0);
		insole->L.n.Set(c, -s, 0);
	}
	insole->E.SetNormal(insole->E, insole->J);
	insole->F.SetNormal(insole->J, insole->H);
	insole->G.SetNormal(insole->B, insole->E);
	insole->Z.SetNormal(insole->L, insole->C);
	insole->B.SetNormal(insole->H, insole->G);

	// The first line has to start at point B for the rest of the
	// algorithm to work.
	Insole::Line temp;
	insole->lines.clear();
	temp.Setup(insole->B, insole->G, 0.39, 0.39);
	insole->lines.push_back(temp);
	temp.Setup(insole->G, insole->E, 0.2, 0.3);
	insole->lines.push_back(temp);
	temp.Setup(insole->E, insole->K, 0.39, 0.7);
	insole->lines.push_back(temp);
	temp.Setup(insole->K, insole->N);
	insole->lines.push_back(temp);
	temp.Setup(insole->N, insole->L);
	insole->lines.push_back(temp);
	temp.Setup(insole->L, insole->F, 0.3, 0.39);
	insole->lines.push_back(temp);
	temp.Setup(insole->F, insole->Z, 0.39, 0.2);
	insole->lines.push_back(temp);
	temp.Setup(insole->Z, insole->B, 0.4, 0.3);
	insole->lines.push_back(temp);

	FinishConstruction(100);
}

void InsoleConstruct::FinishConstruction(const size_t N) {
	// Calculate the total length
	double L0 = 0.0;
	for (const auto &line : insole->lines)
		L0 += line.Length();

	// Rescale the polygons from 0..2*M_PI
	double Lsum = 0.0;
	for (auto &line : insole->lines) {
		const double L = line.Length();
		const double f = M_PI * 2.0 / L0;
		const double mr = Lsum * f;
		const double sr = L * f;
		line.x.ScaleX(sr);
		line.y.ScaleX(sr);
		line.z.ScaleX(sr);
		line.x.ShiftX(mr);
		line.y.ShiftX(mr);
		line.z.ShiftX(mr);
		line.r0 = mr;
		line.r1 = mr + sr;
		Lsum += L;
	}

	// Find the orientation
	bool positive = true;
	{
		const double r0 = RatX(0.0, true);
		const double r1 = RatX(0.0, false);
		positive = (r0 < r1);
	}

	// Setup the transformation into the u-Coordinate
	DependentVector tr;
	tr.PushBack(0.0, 0.0);
	{
		const double rC1 = RatX(insole->C.p.x, positive);
		const double rC2 = RatX(insole->C.p.x, !positive);
		const double rC = (rC1 + (2 * M_PI) - rC2) / 2.0;
		const double rJ1 = RatX(insole->J.p.x, positive);
		const double rJ2 = RatX(insole->J.p.x, !positive);
		const double rJ = (rJ1 + (2 * M_PI) - rJ2) / 2.0;
		const double rA = RatY(0.0, false);
		tr.PushBack(rC1, rC);
		tr.PushBack(rJ1, rJ);
		tr.PushBack(rA, M_PI);
		tr.PushBack(rJ2, 2 * M_PI - rJ);
		tr.PushBack(rC2, 2 * M_PI - rC);
		tr.PushBack(2 * M_PI, 2 * M_PI);
	}

	// Sample the lines
	{
		insole->outline.clear();
		Polynomial rs = Polynomial::ByValue(0.0, 0.0, (double) N, 2 * M_PI);
		size_t m = 0;
		for (size_t n = 0; n < N; ++n) {
			const double r = rs(n);
			while ((m + 1) < insole->lines.size() && insole->lines[m].r1 < r)
				++m;
			const double r2 = (r > M_PI) ? (r - 2 * M_PI) : r;
			insole->outline.push_back(Insole::Point(insole->lines[m](r), r2));
		}
	}

	// Interpolate the polygons on the inside and the outside of the last.
	insole->inside.Clear();
	insole->outside.Clear();

	const Polynomial px = Polynomial::ByValue(-0.2, insole->A.p.x,
			(double) N - 1 + 0.2, insole->B.p.x);
	for (size_t n = 0; n < N; ++n) {
		const double x = px(n);
		double y0 = DBL_MAX;
		double y1 = -DBL_MAX;
		for (const auto &line : insole->lines) {
			const double r = (line.x - x).FindZero((line.r0 + line.r1) / 2.0);
			if (r < line.r0 || r > line.r1)
				continue;
			const double y = line.y(r);
			if (y < y0)
				y0 = y;
			if (y > y1)
				y1 = y;
		}
		Vector3 vert0(x, y0, 0);
		insole->inside.AddEdgeToVertex(vert0);

		Vector3 vert1(x, y1, 0);
		insole->outside.AddEdgeToVertex(vert1);
	}
	insole->inside.CalculateNormals(Polygon3::CalculateNormalMethod::InPlaneZX);
	insole->outside.CalculateNormals(
			Polygon3::CalculateNormalMethod::InPlaneZX);
}

double InsoleConstruct::RatX(const double x, const bool yPositive) const {
	double rr = -DBL_MAX;
	double yy = (yPositive) ? (-DBL_MAX) : (DBL_MAX);
	for (auto &line : insole->lines) {
		const double r = (line.x - x).FindZero((line.r0 + line.r1) / 2.0);
		if (!line.IsInside(r))
			continue;
		const double y = line.y(r);
		if (yPositive) {
			if (y > yy) {
				yy = y;
				rr = r;
			}
		} else {
			if (y < yy) {
				yy = y;
				rr = r;
			}
		}
	}
	return rr;
}

double InsoleConstruct::RatY(const double y, const bool xPositive) const {
	double rr = -DBL_MAX;
	double xx = (xPositive) ? (-DBL_MAX) : (DBL_MAX);
	for (auto &line : insole->lines) {
		const double r = (line.y - y).FindZero((line.r0 + line.r1) / 2.0);
		if (!line.IsInside(r))
			continue;
		const double x = line.x(r);
		if (xPositive) {
			if (x > xx) {
				xx = x;
				rr = r;
			}
		} else {
			if (x < xx) {
				xx = x;
				rr = r;
			}
		}
	}
	return rr;
}

