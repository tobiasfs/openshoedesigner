///////////////////////////////////////////////////////////////////////////////
// Name               : InsoleAnalyze.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 12.01.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#include "InsoleAnalyze.h"

#include <sstream>
#include <stdexcept>

InsoleAnalyze::InsoleAnalyze() {
	out = std::make_shared<Insole>();
}

std::string InsoleAnalyze::GetName() const {
	return "InsoleAnalyze";
}

bool InsoleAnalyze::CanRun() {
	std::string missing;

	if (!footLength)
		missing += missing.empty() ? "\"footLength\"" : ", \"footLength\"";
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
	if (!extraLength)
		missing += missing.empty() ? "\"extraLength\"" : ", \"extraLength\"";
	if (!in)
		missing += missing.empty() ? "\"in\"" : ", \"in\"";
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
		error += " Input \"footLength\" for InsoleAnalyze is empty.";
	}
	if (ballMeasurementAngle->GetString().empty()) {
		error += " Input \"ballMeasurementAngle\" for InsoleAnalyze is empty.";
	}
	if (heelDirectionAngle->GetString().empty()) {
		error += " Input \"heelDirectionAngle\" for InsoleAnalyze is empty.";
	}
	if (littleToeAngle->GetString().empty()) {
		error += " Input \"littleToeAngle\" for InsoleAnalyze is empty.";
	}
	if (bigToeAngle->GetString().empty()) {
		error += " Input \"bigToeAngle\" for InsoleAnalyze is empty.";
	}
	if (extraLength->GetString().empty()) {
		error += " Input \"extraLength\" for InsoleAnalyze is empty.";
	}

	return error.empty();
}

bool InsoleAnalyze::Propagate() {
	if (!footLength || !ballMeasurementAngle || !heelDirectionAngle
			|| !littleToeAngle || !bigToeAngle || !extraLength || !in || !out)
		return false;

	bool modify = false;
	bool parameterModified = false;
	parameterModified |= !in->IsValid();
	parameterModified |= footLength->IsModified();
	parameterModified |= ballMeasurementAngle->IsModified();
	parameterModified |= heelDirectionAngle->IsModified();
	parameterModified |= littleToeAngle->IsModified();
	parameterModified |= bigToeAngle->IsModified();
	parameterModified |= extraLength->IsModified();

	if (parameterModified) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}
	if (out->IsNeeded()) {
		modify |= !in->IsNeeded();
		in->MarkNeeded(true);
	}
	return modify;
}

bool InsoleAnalyze::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void InsoleAnalyze::Run() {
	*out = *in;

	auto dist = [](double a, double b) {
		return sqrt(a * a + b * b);
	};

	// Find longest distance in insole
	double LLast = 0;
	size_t idx0 = 0;
	size_t idx1 = 0;
	bool hasImproved = true;
	while (hasImproved) {
		hasImproved = false;
		const Geometry::Vertex &v0 = out->GetVertex(idx0);
		for (size_t idx = 0; idx < out->CountVertices(); idx++) {
			const Geometry::Vertex &v1 = out->GetVertex(idx);
			const double d1 = dist(v0.u - v1.u, v0.v - v1.v);
			if (d1 > LLast + FLT_EPSILON) {
				idx1 = idx;
				LLast = d1;
				hasImproved = true;
			}
		}
		const Geometry::Vertex &v2 = out->GetVertex(idx1);
		for (size_t idx = 0; idx < out->CountVertices(); idx++) {
			const Geometry::Vertex &v1 = out->GetVertex(idx);
			const double d1 = dist(v2.u - v1.u, v2.v - v1.v);
			if (d1 > LLast + FLT_EPSILON) {
				idx0 = idx;
				LLast = d1;
				hasImproved = true;
			}
		}
	}
	const double LFoot = footLength->ToDouble()
			/ (footLength->ToDouble() + extraLength->ToDouble()) * LLast;

	std::cout << "Insole length: " << LLast * 100 << " cm\n";
	std::cout << "Mapped foot length: " << LFoot * 100 << " cm\n";

	const Geometry::Vertex &v0 = out->GetVertex(idx0);
	const Geometry::Vertex &v1 = out->GetVertex(idx1);
	Geometry::Vertex dir = Normal(Diff(v1, v0));

	// Find the orientation and the exact positioning of the insole by sampling
	// at 0.2, 0.4, 0.5, 0.6, 0.8 of the distance between v0 and v1.

	const Polygon3::Intersections r2 = Intersect(out->outline, v0, v1, 0.2);
	const Polygon3::Intersections r4 = Intersect(out->outline, v0, v1, 0.4);
	const Polygon3::Intersections r5 = Intersect(out->outline, v0, v1, 0.5);
	const Polygon3::Intersections r6 = Intersect(out->outline, v0, v1, 0.6);
	const Polygon3::Intersections r8 = Intersect(out->outline, v0, v1, 0.8);

	if (r2.positive.empty() || r2.negative.empty() || r4.positive.empty()
			|| r4.negative.empty() || r5.positive.empty() || r5.negative.empty()
			|| r6.positive.empty() || r6.negative.empty() || r8.positive.empty()
			|| r8.negative.empty()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " -";
		err
				<< "For the insole extracted, the intersections on the line A-B are not complete.";
		error = err.str();
		throw std::runtime_error(err.str());
	}
	if (r2.positive.size() > 1 || r2.negative.size() > 1
			|| r4.positive.size() > 1 || r4.negative.size() > 1
			|| r5.positive.size() > 1 || r5.negative.size() > 1
			|| r6.positive.size() > 1 || r6.negative.size() > 1
			|| r8.positive.size() > 1 || r8.negative.size() > 1) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " -";
		err
				<< "For the insole extracted, the outline is not clean and there are some inconsistencies, that make the analysis difficult.";
		std::cout << err.str() << '\n';
	}

	const double L2 = Dist(r2.positive[0], r2.negative[0]);
	const double L4 = Dist(r4.positive[0], r4.negative[0]);
//	const double L5 = Dist(r5.positive[0], r5.negative[0]);
	const double L6 = Dist(r6.positive[0], r6.negative[0]);
	const double L8 = Dist(r8.positive[0], r8.negative[0]);

	if (L2 + L4 > L6 + L8) {
		// The middle of L2 is close to D and the middle of L8 is close to J
		out->J = r8.negative[0].Interp(r8.positive[0], 0.5);
		// Temporary calculation of A, B, and M
		out->A = v1;
		out->B = v0;
		out->M = r6.negative[0].Interp(r6.positive[0], 0.5);
	} else {
		// The middle of L2 is close to J and the middle of L8 is close to D
		out->J = r2.negative[0].Interp(r2.positive[0], 0.5);
		// Temporary calculation of A, B, and M
		out->A = v0;
		out->B = v1;
		out->M = r4.negative[0].Interp(r4.positive[0], 0.5);
	}
	// Temporary C
	out->C = out->A.Interp(out->B, 0.62);
	// Find P
	{
		Geometry::Vertex ntemp(0, 0, 0, out->J.u + (out->M.v - out->J.v),
				out->J.v - (out->M.u - out->J.u));
		const Polygon3::Intersections Ip = Intersect(out->outline, out->J,
				ntemp, 0.0);

		out->P = Ip.negative[0];

		std::cout << 'x' << '\n';

	}
	// Find E-F from C
	{
		const Polygon3::Intersections Ic = Intersect(out->outline, out->C,
				out->C + out->M - out->J, 0.0);
		out->E = Ic.positive[0];
		out->F = Ic.negative[0];
	}
	// Find correct C
	{
		// Formula from page 167, Alexander Besching, "Handbuch der
		// Schuhindustrie", 1990
		const double mix = 1.0 / 6.0 / (1.0 / 6.0 + 1.0 / 6.0 * 1.24);
		out->C = out->E.Interp(out->F, mix);
	}
	// Find correct J, also K and L
	{
		double r = Dist(out->P, out->M);
		out->J = out->P.Interp(out->M, LFoot / 5.0 / r);
		const Polygon3::Intersections Ic = Intersect(out->outline, out->J,
				out->M, 0.0);
		out->K = Ic.positive[0];
		out->L = Ic.negative[0];
	}
	// Find correct A and B, also D and N.
	{
		const Polygon3::Intersections Iab = Intersect(out->outline, out->J,
				out->J
						+ Insole::Point::FromUV(out->C.v - out->J.v,
								-(out->C.u - out->J.u)), 0.0);
		out->A = Iab.negative[0];
		out->B = Iab.positive[0];
		double r = Dist(out->A, out->B);
		out->D = out->B.Interp(out->A, extraLength->ToDouble() / r);
		double w = Dist(out->K, out->L);
		out->N = out->A.Interp(out->B, w / 2.0 / r);
	}

	// Find G
	{
		const Polygon3::Intersections Ig = Intersect(out->outline, out->A,
				out->D, 1.0);
		out->G = Ig.positive[0];
	}
	//Find Z
	{
		double r = Dist(out->D, out->C);
		const Polygon3::Intersections Iz = Intersect(out->outline, out->D,
				out->C, footLength->ToDouble() / 5.0 / r);
		out->Z = Iz.positive[0];
	}

	// Position M correctly
	{
		Geometry::Vertex n = Normal(Diff(out->M, out->P));
		Geometry::Vertex rC = Diff(out->C, out->P);
		Geometry::Vertex rM = Diff(out->M, out->P);
		double extra = (rC.u * n.u + rC.v * n.v) - (rM.u * n.u + rM.v * n.v);
		out->M += Insole::Point::FromUV(n.u * extra, n.v * extra);
	}

	// Find H
	{
		Geometry::Vertex n = Normal(Diff(out->B, out->A));
		Geometry::Vertex n2 = Normal(Diff(out->Z, out->F));

		Geometry::Vertex rD = Diff(out->D, out->A);
		Geometry::Vertex rF = Diff(out->F, out->A);
		Geometry::Vertex rZ = Diff(out->Z, out->A);
		double m0 = Dist(out->Z, out->F);

		double mD = rD.u * n.u + rD.v * n.v;
		double mF = rF.u * n.u + rF.v * n.v;
		double mZ = rZ.u * n.u + rZ.v * n.v;

		double m = m0 / (mZ - mF) * (mD - mF);

		out->H = out->F + Insole::Point::FromUV(n2.u * m, n2.v * m);

	}

	out->MarkValid(true);
	out->MarkNeeded(false);
	return;

	const Vector3 side = { 0, 1, 0 };

	// Extract the outline of the sole
//	Polygon3 outline;
////	outline.ExtractOutline(*in);
//	out->inside.Clear();
//	out->outside.Clear();
//	const Vector3 center = out->GetCenter();
//	for (size_t n = 0; n < out->CountEdges(); ++n) {
//		const auto &e = out->GetEdge(n);
//		if (e.trianglecount >= 2)
//			continue;
//		const auto &v0 = out->GetEdgeVertex(n, 0);
//		const auto &v1 = out->GetEdgeVertex(n, 1);
//		Vector3 rot = (v0 - center) * (v1 - v0);
//		if (rot.z > 0.0)
//			outline.AddEdge(v0, v1);
//		else
//			outline.AddEdge(v1, v0);
//
//		double d0 = v0.Dot(side);
//		double d1 = v1.Dot(side);
//
//		if (d0 >= 0.0 && d1 >= 0.0) {
//			if (rot.z > 0.0)
//				out->outside.AddEdge(v0, v1);
//			else
//				out->outside.AddEdge(v1, v0);
//		}
//		if (d0 <= 0.0 && d1 <= 0.0) {
//			if (rot.z > 0.0)
//				out->inside.AddEdge(v1, v0);
//			else
//				out->inside.AddEdge(v0, v1);
//		}
//	}
//	outline.Join();
//	outline.SortLoop();
//
//	out->inside.Join();
//	out->inside.SortLoop();
//	out->outside.Join();
//	out->outside.SortLoop();

// Find the center line A-B

	auto intersect = out->outline.Intersect(side, 0);
	if (!intersect.negative.empty())
		out->A = intersect.negative[0];
	if (!intersect.positive.empty())
		out->B = intersect.positive[0];

	out->C = out->A.Interp(out->B, 0.62);
	Vector3 n = (out->B - out->A).Normal();
	Vector3 rot = n * side;
	n = AffineTransformMatrix::RotationAroundVector(rot,
			ballMeasurementAngle->ToDouble()).Transform(n);
//	n.Set(1, 0, 0);

	auto int2 = out->outline.Intersect(n, out->C.Dot(n));

	if (!int2.positive.empty())
		out->E = int2.positive[0];
	if (!int2.negative.empty())
		out->F = int2.negative[0];

	const double fl = footLength->ToDouble();
	const double ll = fl + extraLength->ToDouble();
//
//	out->C = (out->E + out->F) / 2.0;
//	out->J = out->A.Interp(out->C, fl / ll / 0.62 / 6);
//
//	n = (out->J - out->A).Normal();
//	rot = n * side;
//	n = AffineTransformMatrix::RotationAroundVector(rot,
//			heelDirectionAngle->ToDouble()).Transform(n);
//	n.Set(1, 0, 0);

//	auto int3 = out->outline.Intersect(n, out->J.Dot(n));
//
//	if (!int3.positive.empty())
//		out->K = int3.positive[0];
//	if (!int3.negative.empty())
//		out->L = int3.negative[0];
//
//	out->J = (out->K + out->L) / 2.0;
//
//	n = AffineTransformMatrix::RotationAroundVector(rot, -M_PI_2).Transform(n);
//	auto int4 = out->outline.Intersect(n, out->J.Dot(n));
//	if (!int4.positive.empty())
//		out->P = int4.positive[0];

// Calculate foot/last relation

//	out->D = out->C.Interp(out->B, fl / ll);
//
//	n = (out->D - out->C).Normal();
//	auto int5 = out->outline.Intersect(n, out->D.Dot(n));
//
//	if (!int5.positive.empty())
//		out->G = int5.positive[0];
//	if (!int5.negative.empty())
//		out->H = int5.negative[0];
//
//	double fsl = (out->B - out->C).Abs() / 0.38 * fl / ll;
//	Vector3 hz = out->D.Interp(out->C, (fsl / 5) / (out->B - out->C).Abs());
//	auto int6 = out->outline.Intersect(n, hz.Dot(n));
//	if (!int6.positive.empty())
//		out->Z = int6.negative[0];
//
//	out->H.y = out->F.y
//			+ (out->Z.y - out->F.y) / (out->Z.x - out->F.x)
//					* (out->H.x - out->F.x);

	out->MarkValid(true);
	out->MarkNeeded(false);

}

Geometry::Vertex InsoleAnalyze::Diff(Geometry::Vertex a,
		const Geometry::Vertex &b) const {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.u -= b.u;
	a.v -= b.v;
	return a;
}

double InsoleAnalyze::Dist(const Geometry::Vertex &a,
		const Geometry::Vertex &b) const {
	const double du = b.u - a.u;
	const double dv = b.v - a.v;
	return sqrt(du * du + dv * dv);
}

Geometry::Vertex InsoleAnalyze::Normal(Geometry::Vertex a) const {
	const double d = sqrt(a.u * a.u + a.v * a.v);
	if (d < FLT_EPSILON)
		return a;
	a.x /= d;
	a.y /= d;
	a.z /= d;
	a.u /= d;
	a.v /= d;
	return a;
}

Polygon3::Intersections InsoleAnalyze::Intersect(const Geometry &geo,
		const Geometry::Vertex &a, const Geometry::Vertex &b,
		double relDistance) const {
	Geometry::Vertex dir = Diff(b, a);
	{
		const double L = sqrt(dir.u * dir.u + dir.v * dir.v);
		if (L > FLT_EPSILON) {
			const double F = 1.0 / (L * L);
			dir.u *= F;
			dir.v *= F;
		}
	}
	Polygon3::Intersections ret;
	for (size_t idx = 0; idx < geo.CountEdges(); ++idx) {
		const Geometry::Vertex &vert0 = geo.GetEdgeVertex(idx, 0);
		const Geometry::Vertex &vert1 = geo.GetEdgeVertex(idx, 1);
		Vector3 v0(vert0.u - a.u, vert0.v - a.v);
		Vector3 v1(vert1.u - a.u, vert1.v - a.v);
		const double h0 = v0.x * dir.u + v0.y * dir.v;
		const double h1 = v1.x * dir.u + v1.y * dir.v;
		if (h0 <= relDistance && h1 > relDistance)
			ret.positive.push_back(
					vert0.Interp(vert1, (relDistance - h0) / (h1 - h0)));
		if (h0 >= relDistance && h1 < relDistance)
			ret.negative.push_back(
					vert0.Interp(vert1, (relDistance - h0) / (h1 - h0)));
	}
	return ret;
}

void InsoleAnalyze::UpdateXYZfromUV(const Geometry &geo, Geometry::Vertex &V) {
}
