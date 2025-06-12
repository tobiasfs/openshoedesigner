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
	out = std::make_shared<Insole>();
}

std::string InsoleConstruct::GetName() const {
	return "InsoleConstruct";
}

bool InsoleConstruct::CanRun() {
	std::string missing;

	if (!footLength)
		missing += missing.empty() ? "\"footLength\"" : ", \"footLength\"";
	if (!ballWidth)
		missing += missing.empty() ? "\"ballWidth\"" : ", \"ballWidth\"";
	if (!heelWidth)
		missing += missing.empty() ? "\"heelWidth\"" : ", \"heelWidth\"";
	if (!ballMeasurementAngle)
		missing +=
				missing.empty() ?
						"\"ballMeasurementAngle\"" :
						", \"ballMeasurementAngle\"";
	if (!heelDirectionAngle)
		missing +=
				missing.empty() ?
						"\"heelDirectionAngle\"" : ", \"heelDirectionAngle\"";
	if (!littleToeAngle)
		missing +=
				missing.empty() ? "\"littleToeAngle\"" : ", \"littleToeAngle\"";
	if (!bigToeAngle)
		missing += missing.empty() ? "\"bigToeAngle\"" : ", \"bigToeAngle\"";
	if (!tipSharpness)
		missing += missing.empty() ? "\"tipSharpness\"" : ", \"tipSharpness\"";
	if (!extraLength)
		missing += missing.empty() ? "\"extraLength\"" : ", \"extraLength\"";
	if (!out)
		missing += missing.empty() ? "\"out\"" : ", \"out\"";

	if (!missing.empty()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " -";
		err << "The variables " << missing << " are not connected.";
		error = err.str();
		throw std::logic_error(err.str());
	}

	error.clear();

	if (footLength->GetString().empty()) {
		error += " Input \"footLength\" for InsoleConstruct is empty.";
	}
	if (ballWidth->GetString().empty()) {
		error += " Input \"ballWidth\" for InsoleConstruct is empty.";
	}
	if (heelWidth->GetString().empty()) {
		error += " Input \"heelWidth\" for InsoleConstruct is empty.";
	}
	if (ballMeasurementAngle->GetString().empty()) {
		error +=
				" Input \"ballMeasurementAngle\" for InsoleConstruct is empty.";
	}
	if (heelDirectionAngle->GetString().empty()) {
		error += " Input \"heelDirectionAngle\" for InsoleConstruct is empty.";
	}
	if (littleToeAngle->GetString().empty()) {
		error += " Input \"littleToeAngle\" for InsoleConstruct is empty.";
	}
	if (bigToeAngle->GetString().empty()) {
		error += " Input \"bigToeAngle\" for InsoleConstruct is empty.";
	}
	if (tipSharpness->GetString().empty()) {
		error += " Input \"tipSharpness\" for InsoleConstruct is empty.";
	}
	if (extraLength->GetString().empty()) {
		error += " Input \"extraLength\" for InsoleConstruct is empty.";
	}

	return error.empty();
}

bool InsoleConstruct::Propagate() {
	if (!footLength || !ballWidth || !heelWidth || !ballMeasurementAngle
			|| !heelDirectionAngle || !littleToeAngle || !bigToeAngle
			|| !tipSharpness || !extraLength || !out)
		return false;

	bool modify = false;
	bool parameterModified = false;
	parameterModified |= footLength->IsModified();
	parameterModified |= ballWidth->IsModified();
	parameterModified |= heelWidth->IsModified();
	parameterModified |= ballMeasurementAngle->IsModified();
	parameterModified |= heelDirectionAngle->IsModified();
	parameterModified |= littleToeAngle->IsModified();
	parameterModified |= bigToeAngle->IsModified();
	parameterModified |= tipSharpness->IsModified();
	parameterModified |= extraLength->IsModified();

	if (parameterModified) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}
	return modify;
}

bool InsoleConstruct::HasToRun() {
	return out && !out->IsValid() && out->IsNeeded();
}

void InsoleConstruct::Run() {
	Construct();
	out->MarkValid(true);
	out->MarkNeeded(false);
}

void InsoleConstruct::Construct() {

	// A and B have to be on the axis y = 0.0
	out->J.Set(0, 0, 0);
	out->A = out->J - Insole::Point::FromUV(footLength->ToDouble() / 6.0, 0.0);
	out->C = out->A + Insole::Point::FromUV(footLength->ToDouble() * 0.62, 0.0);
	out->D = out->A + Insole::Point::FromUV(footLength->ToDouble(), 0.0);
	out->B = out->A
			+ Insole::Point::FromUV(
					footLength->ToDouble() + extraLength->ToDouble(), 0.0);

	{
		const double s = sin(ballMeasurementAngle->ToDouble());
		const double c = cos(ballMeasurementAngle->ToDouble());

		out->E = out->C
				+ Insole::Point::FromUV(s * ballWidth->ToDouble() / 2,
						-c * ballWidth->ToDouble() / 2);
		out->F = out->C
				- Insole::Point::FromUV(s * ballWidth->ToDouble() / 2 * 1.24,
						-c * ballWidth->ToDouble() / 2 * 1.24);
	}
	{
		const double s = sin(heelDirectionAngle->ToDouble());
		const double c = cos(heelDirectionAngle->ToDouble());
		out->K = out->J
				+ Insole::Point::FromUV(s * heelWidth->ToDouble() / 2,
						-c * heelWidth->ToDouble() / 2);
		out->L = out->J
				- Insole::Point::FromUV(s * heelWidth->ToDouble() / 2,
						-c * heelWidth->ToDouble() / 2);
		out->P.u = out->A.u * c;
		out->P.v = out->A.u * s;
		out->P.x = out->P.u;
		out->P.y = out->P.v;
		out->P.z = 0.0;
	}
	{
		const double s = sin(bigToeAngle->ToDouble());
		const double c = cos(bigToeAngle->ToDouble());
		const double r = out->D.x - out->E.x;
		out->G = out->E + Insole::Point::FromUV(r, s * r / c);
	}
	{
		const double s = sin(littleToeAngle->ToDouble());
		const double c = cos(littleToeAngle->ToDouble());
		const double r = out->D.u - out->F.u;
		const double f = footLength->ToDouble() / 5;
		out->H = out->F + Insole::Point::FromUV(r, -s * r / c);
		out->Z = out->H - Insole::Point::FromUV(f, -s * f / c);
	}

	// Normals
	out->P.SetNormal(out->K, out->L);
	{
		const double s = sin(heelDirectionAngle->ToDouble());
		const double c = cos(heelDirectionAngle->ToDouble());
		out->K.dir.Set(-c, -s, 0);
		out->L.dir.Set(c, s, 0);
	}
	out->E.SetNormal(out->E, out->J);
	out->F.SetNormal(out->J, out->H);
	out->G.SetNormal(out->B, out->E);
	out->Z.SetNormal(out->L, out->C);
	out->B.SetNormal(out->H, out->G);

	// The first line has to start at point B for the rest of the
	// algorithm to work.
	Insole::Line temp;
	out->lines.clear();
	double tip = 1.0 - tipSharpness->ToDouble();
	temp.Setup(out->B, out->G, 0.39 * tip, 0.39);
	out->lines.push_back(temp);
	temp.Setup(out->G, out->E, 0.2, 0.3);
	out->lines.push_back(temp);
	temp.Setup(out->E, out->K, 0.39, 0.7);
	out->lines.push_back(temp);
	temp.Setup(out->K, out->P);
	out->lines.push_back(temp);
	temp.Setup(out->P, out->L);
	out->lines.push_back(temp);
	temp.Setup(out->L, out->F, 0.3, 0.39);
	out->lines.push_back(temp);
	temp.Setup(out->F, out->Z, 0.39, 0.2);
	out->lines.push_back(temp);
	temp.Setup(out->Z, out->B, 0.4, 0.3 * tip);
	out->lines.push_back(temp);

	FinishConstruction(100);
}

void InsoleConstruct::FinishConstruction(const size_t N) {
	// Calculate the total length
	double L0 = 0.0;
	for (const auto &line : out->lines)
		L0 += line.Length();

	// Rescale the polygons from 0..2*M_PI
	double Lsum = 0.0;
	for (auto &line : out->lines) {
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
		const double rC1 = RatX(out->C.x, positive);
		const double rC2 = RatX(out->C.x, !positive);
		const double rC = (rC1 + (2 * M_PI) - rC2) / 2.0;
		const double rJ1 = RatX(out->J.x, positive);
		const double rJ2 = RatX(out->J.x, !positive);
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
		out->outline.Clear();
		Polynomial rs = Polynomial::ByValue(0.0, 0.0, (double) N, 2 * M_PI);
		size_t m = 0;
		for (size_t n = 0; n < N; ++n) {
			const double r = rs(n);
			while ((m + 1) < out->lines.size() && out->lines[m].r1 < r)
				++m;
			const double r2 = (r > M_PI) ? (r - 2 * M_PI) : r;
			out->outline.AddEdgeToVertex(out->lines[m](r));
		}
		out->outline.CloseLoopNextGroup();
	}

//	// Interpolate the polygons on the inside and the outside of the last.
//	out->inside.Clear();
//	out->outside.Clear();
//
//	const Polynomial px = Polynomial::ByValue(-0.2, out->A.x,
//			(double) N - 1 + 0.2, out->B.x);
//	for (size_t n = 0; n < N; ++n) {
//		const double x = px(n);
//		double y0 = DBL_MAX;
//		double y1 = -DBL_MAX;
//		for (const auto &line : out->lines) {
//			const double r = (line.x - x).FindZero((line.r0 + line.r1) / 2.0);
//			if (r < line.r0 || r > line.r1)
//				continue;
//			const double y = line.y(r);
//			if (y < y0)
//				y0 = y;
//			if (y > y1)
//				y1 = y;
//		}
//		Vector3 vert0(x, y0, 0);
//		out->inside.AddEdgeToVertex(vert0);
//
//		Vector3 vert1(x, y1, 0);
//		out->outside.AddEdgeToVertex(vert1);
//	}
//	out->inside.CalculateNormals(Polygon3::CalculateNormalMethod::InPlaneZX);
//	out->outside.CalculateNormals(Polygon3::CalculateNormalMethod::InPlaneZX);
}

double InsoleConstruct::RatX(const double x, const bool yPositive) const {
	double rr = -DBL_MAX;
	double yy = (yPositive) ? (-DBL_MAX) : (DBL_MAX);
	for (auto &line : out->lines) {
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
	for (auto &line : out->lines) {
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

