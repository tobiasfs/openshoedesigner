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
#include "../../math/Exporter.h"

#include <sstream>
#include <stdexcept>

InsoleAnalyze::InsoleAnalyze() {
	insole_out = std::make_shared<Insole>();
	insoleFlat_out = std::make_shared<Insole>();
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
	if (!insole_in)
		missing += missing.empty() ? "\"insole_in\"" : ", \"insole_in\"";
	if (!insoleFlat_in)
		missing +=
				missing.empty() ? "\"insoleFlat_in\"" : ", \"insoleFlat_in\"";
	if (!insole_out)
		missing += missing.empty() ? "\"insole_out\"" : ", \"insole_out\"";
	if (!insoleFlat_out)
		missing +=
				missing.empty() ? "\"insoleFlat_out\"" : ", \"insoleFlat_out\"";

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
			|| !littleToeAngle || !bigToeAngle || !extraLength || !insole_in
			|| !insoleFlat_in || !insole_out || !insoleFlat_out)
		return false;

	bool parameterModified = false;
	parameterModified |= !insole_in->IsValid();
	parameterModified |= !insoleFlat_in->IsValid();
	parameterModified |= footLength->IsModified();
	parameterModified |= ballMeasurementAngle->IsModified();
	parameterModified |= heelDirectionAngle->IsModified();
	parameterModified |= littleToeAngle->IsModified();
	parameterModified |= bigToeAngle->IsModified();
	parameterModified |= extraLength->IsModified();

	bool modify = false;
	if (parameterModified) {
		modify |= insole_out->IsValid();
		modify |= insoleFlat_out->IsValid();
		insole_out->MarkValid(false);
		insoleFlat_out->MarkValid(false);
	}
	if (insole_out->IsNeeded() || insoleFlat_out->IsNeeded()) {
		modify |= !insole_in->IsNeeded();
		modify |= !insoleFlat_in->IsNeeded();
		insole_in->MarkNeeded(true);
		insoleFlat_in->MarkNeeded(true);
	}
	return modify;
}

bool InsoleAnalyze::HasToRun() {
	return insole_in && insole_in->IsValid() && insoleFlat_in
			&& insoleFlat_in->IsValid() && insole_out && insoleFlat_out
			&& ((!insole_out->IsValid() && insole_out->IsNeeded())
					|| (!insoleFlat_out->IsValid() && insoleFlat_out->IsNeeded()));
}

void InsoleAnalyze::Run() {
	*insole_out = *insole_in;
	*insoleFlat_out = *insoleFlat_in;

	// Copy the UV coordinates over to the unflattened insole. The ordering of
	// the vertices in the geometry should have stayed the same during the
	// flattening.
	if (insoleFlat_out->CountVertices() != insole_out->CountVertices()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " - ";
		err
				<< "The insole and the flattened insole have different counts of vertices. These counts should be exactly the same.";
		error = err.str();
		throw std::runtime_error(err.str());
	}
	for (size_t idx = 0; idx < insole_out->CountVertices(); idx++) {
		auto &src = insoleFlat_out->GetVertex(idx);
		auto &dst = insole_out->GetVertex(idx);
		dst.u = src.u;
		dst.v = src.v;
		dst.n = src.n; // Replace the normals by one that is flat on the
		//                unique dimension.
	}
	for (size_t idx = 0; idx < insole_out->CountEdges(); idx++) {
		auto &src = insoleFlat_out->GetEdge(idx);
		auto &dst = insole_out->GetEdge(idx);
		dst.n = src.n;
	}
	for (size_t idx = 0; idx < insole_out->CountTriangles(); idx++) {
		auto &src = insoleFlat_out->GetTriangle(idx);
		auto &dst = insole_out->GetTriangle(idx);
		dst.n = src.n;
	}

	// For all calculations below, the insoleFlat is assumed to have Z=0
	// for all vertices.

	// Find longest distance in insoleFlat outline.
	auto dist = [](double a, double b) {
		return sqrt(a * a + b * b);
	};
	double LLast = 0;
	size_t idx0 = 0;
	size_t idx1 = 0;
	bool hasImproved = true;
	while (hasImproved) {
		hasImproved = false;
		const Geometry::Vertex &v0 = insoleFlat_out->outline.GetVertex(idx0);
		for (size_t idx = 0; idx < insoleFlat_out->outline.CountVertices();
				idx++) {
			const Geometry::Vertex &v1 = insoleFlat_out->outline.GetVertex(idx);
			const double d1 = dist(v0.x - v1.x, v0.y - v1.y);
			if (d1 > LLast + FLT_EPSILON) {
				idx1 = idx;
				LLast = d1;
				hasImproved = true;
			}
		}
		const Geometry::Vertex &v2 = insoleFlat_out->outline.GetVertex(idx1);
		for (size_t idx = 0; idx < insoleFlat_out->outline.CountVertices();
				idx++) {
			const Geometry::Vertex &v1 = insoleFlat_out->outline.GetVertex(idx);
			const double d1 = dist(v2.x - v1.x, v2.y - v1.y);
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

	const Geometry::Vertex &v0 = insoleFlat_out->outline.GetVertex(idx0);
	const Geometry::Vertex &v1 = insoleFlat_out->outline.GetVertex(idx1);
	const Geometry::Vertex dir = Normal(Diff(v1, v0));

// Find the orientation and the exact positioning of the insole by sampling
// at 0.2, 0.4, 0.5, 0.6, 0.8 of the distance between v0 and v1.

	const Polygon3::Intersections r2 = Intersect(insoleFlat_out->outline, v0,
			v1, 0.2);
	const Polygon3::Intersections r4 = Intersect(insoleFlat_out->outline, v0,
			v1, 0.4);
	const Polygon3::Intersections r5 = Intersect(insoleFlat_out->outline, v0,
			v1, 0.5);
	const Polygon3::Intersections r6 = Intersect(insoleFlat_out->outline, v0,
			v1, 0.6);
	const Polygon3::Intersections r8 = Intersect(insoleFlat_out->outline, v0,
			v1, 0.8);

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
				<< __FUNCTION__ << " - ";
		err
				<< "For the insole extracted, the outline is not clean and there are some inconsistencies, that make the analysis difficult.";
		std::cout << err.str() << '\n';
	}

	const double L2 = Dist(r2.positive[0], r2.negative[0]);
	const double L4 = Dist(r4.positive[0], r4.negative[0]);
//	const double L5 = Dist(r5.positive[0], r5.negative[0]);
	const double L6 = Dist(r6.positive[0], r6.negative[0]);
	const double L8 = Dist(r8.positive[0], r8.negative[0]);

// Check where the middle of r4.pos, r4.neg, r6.pos, r6.neg projects
// onto the line from r5.pos to r5.neg. This indicates if the
// insole is a left (positive values) or a right side (negative values).
	double sideIndicator = 0.0;
	{
		Vector3 center = (r2.positive[0] + r2.negative[0] + r8.positive[0]
				+ r8.negative[0]) / 4.0;
		const double L = Dist(r5.positive[0], r5.negative[0]);
		const Vector3 LDir = Diff(r5.positive[0], r5.negative[0]);
		const double R = (center.x - r5.negative[0].x) * LDir.y
				+ (center.y - r5.negative[0].y) * LDir.y;
		sideIndicator = 2.0 * R / (L * L) - 1.0;
	}

	if (L2 + L4 > L6 + L8) {
		// The middle of L2 is close to D and the middle of L8 is close to J
		insoleFlat_out->J = r8.negative[0].Interp(r8.positive[0], 0.5);
		// Temporary calculation of A, B, and M
		insoleFlat_out->A = v1;
		insoleFlat_out->B = v0;
		insoleFlat_out->M = r6.negative[0].Interp(r6.positive[0], 0.5);
		sideIndicator = -sideIndicator;
	} else {
		// The middle of L2 is close to J and the middle of L8 is close to D
		insoleFlat_out->J = r2.negative[0].Interp(r2.positive[0], 0.5);
		// Temporary calculation of A, B, and M
		insoleFlat_out->A = v0;
		insoleFlat_out->B = v1;
		insoleFlat_out->M = r4.negative[0].Interp(r4.positive[0], 0.5);
	}
// Temporary C
	insoleFlat_out->C = insoleFlat_out->A.Interp(insoleFlat_out->B, 0.62);

// Find P
	{
		Geometry::Vertex ntemp(
				insoleFlat_out->J.x
						- (insoleFlat_out->M.y - insoleFlat_out->J.y),
				insoleFlat_out->J.y
						+ (insoleFlat_out->M.x - insoleFlat_out->J.x));
		const Polygon3::Intersections Ip = Intersect(insoleFlat_out->outline,
				insoleFlat_out->J, ntemp, 0.0);

		insoleFlat_out->P = Ip.negative[0];
	}

// Find E-F from C
	{
		const Polygon3::Intersections Ic = Intersect(insoleFlat_out->outline,
				insoleFlat_out->C,
				insoleFlat_out->C + insoleFlat_out->M - insoleFlat_out->J, 0.0);
		if (sideIndicator < 0.0) {
			insoleFlat_out->E = Ic.negative[0];
			insoleFlat_out->F = Ic.positive[0];
		} else {
			insoleFlat_out->E = Ic.positive[0];
			insoleFlat_out->F = Ic.negative[0];
		}
	}

// Find correct C
	{
		// Formula from page 167, Alexander Besching, "Handbuch der
		// Schuhindustrie", 1990
		const double mix = 1.0 / 6.0 / (1.0 / 6.0 + 1.0 / 6.0 * 1.24);
		insoleFlat_out->C = insoleFlat_out->E.Interp(insoleFlat_out->F, mix);
	}
// Find correct J, also K and L
	{
		double r = Dist(insoleFlat_out->P, insoleFlat_out->M);
		insoleFlat_out->J = insoleFlat_out->P.Interp(insoleFlat_out->M,
				LFoot / 5.0 / r);
		const Polygon3::Intersections Ic = Intersect(insoleFlat_out->outline,
				insoleFlat_out->J, insoleFlat_out->M, 0.0);
		if (sideIndicator < 0.0) {
			insoleFlat_out->K = Ic.negative[0];
			insoleFlat_out->L = Ic.positive[0];
		} else {
			insoleFlat_out->K = Ic.positive[0];
			insoleFlat_out->L = Ic.negative[0];
		}
	}
// Find correct A and B, also D and N.
	{
		const Polygon3::Intersections Iab = Intersect(insoleFlat_out->outline,
				insoleFlat_out->J,
				insoleFlat_out->J
						+ Insole::Point::FromXY(
								-(insoleFlat_out->C.v - insoleFlat_out->J.v),
								(insoleFlat_out->C.u - insoleFlat_out->J.u)),
				0.0);
		insoleFlat_out->A = Iab.negative[0];
		insoleFlat_out->B = Iab.positive[0];
		double r = Dist(insoleFlat_out->A, insoleFlat_out->B);
		insoleFlat_out->D = insoleFlat_out->B.Interp(insoleFlat_out->A,
				extraLength->ToDouble() / r);
		double w = Dist(insoleFlat_out->K, insoleFlat_out->L);
		insoleFlat_out->N = insoleFlat_out->A.Interp(insoleFlat_out->B,
				w / 2.0 / r);
	}

// Find G
	{
		const Polygon3::Intersections Ig = Intersect(insoleFlat_out->outline,
				insoleFlat_out->A, insoleFlat_out->D, 1.0);
		if (sideIndicator < 0.0)
			insoleFlat_out->G = Ig.negative[0];
		else
			insoleFlat_out->G = Ig.positive[0];
	}
//Find Z
	{
		double r = Dist(insoleFlat_out->D, insoleFlat_out->C);
		const Polygon3::Intersections Iz = Intersect(insoleFlat_out->outline,
				insoleFlat_out->D, insoleFlat_out->C,
				footLength->ToDouble() / 5.0 / r);
		if (sideIndicator < 0.0)
			insoleFlat_out->Z = Iz.negative[0];
		else
			insoleFlat_out->Z = Iz.positive[0];
	}

// Position M correctly
	{
		Geometry::Vertex n = Normal(Diff(insoleFlat_out->M, insoleFlat_out->P));
		Geometry::Vertex rC = Diff(insoleFlat_out->C, insoleFlat_out->P);
		Geometry::Vertex rM = Diff(insoleFlat_out->M, insoleFlat_out->P);
		double extra = (rC.x * n.x + rC.y * n.y) - (rM.x * n.x + rM.y * n.y);
		insoleFlat_out->M += Insole::Point::FromXY(n.x * extra, n.y * extra);
	}

// Find H
	{
		Geometry::Vertex n = Normal(Diff(insoleFlat_out->B, insoleFlat_out->A));
		Geometry::Vertex n2 = Normal(
				Diff(insoleFlat_out->Z, insoleFlat_out->F));

		Geometry::Vertex rD = Diff(insoleFlat_out->D, insoleFlat_out->A);
		Geometry::Vertex rF = Diff(insoleFlat_out->F, insoleFlat_out->A);
		Geometry::Vertex rZ = Diff(insoleFlat_out->Z, insoleFlat_out->A);
		double m0 = Dist(insoleFlat_out->Z, insoleFlat_out->F);

		double mD = rD.x * n.x + rD.y * n.y;
		double mF = rF.x * n.x + rF.y * n.y;
		double mZ = rZ.x * n.x + rZ.y * n.y;

		double m = m0 / (mZ - mF) * (mD - mF);

		insoleFlat_out->H = insoleFlat_out->F
				+ Insole::Point::FromXY(n2.x * m, n2.y * m);
	}

	// Flag to indicate, that UV and XYZ do not have the same mathematical
	// orientation anymore.
	bool flagFlipped = false;

	// Center the UV-corrdinate systems on J. Modify the coordinate system,
	// so that U increases to the tip and V increases to the outside.
	{
		AffineTransformMatrix m;
		if (insoleFlat_out->E.u > insoleFlat_out->F.u)
			m[0] = -m[0];
		if (insoleFlat_out->A.v > insoleFlat_out->B.v)
			m[5] = -m[5];
		m.TranslateLocal(-insoleFlat_out->J.u, -insoleFlat_out->J.v, 0.0);
		m.UpdateOrientation();
		for (size_t idx = 0; idx < insole_out->CountVertices(); idx++) {
			Geometry::Vertex &v0 = insoleFlat_out->GetVertex(idx);
			Geometry::Vertex &v1 = insole_out->GetVertex(idx);
			const Vector3 temp0 = m.Transform(v0.u, v0.v);
			const Vector3 temp1 = m.Transform(v1.u, v1.v);
			v0.u = temp0.x;
			v0.v = temp0.y;
			v1.u = temp1.x;
			v1.v = temp1.y;
		}
		for (size_t idx = 0; idx < insoleFlat_out->outline.CountVertices();
				idx++) {
			Geometry::Vertex &v0 = insoleFlat_out->outline.GetVertex(idx);
			const Vector3 temp0 = m.Transform(v0.u, v0.v);
			v0.u = temp0.x;
			v0.v = temp0.y;
		}
		const Vector3 tempA = m.Transform(insoleFlat_out->A.u,
				insoleFlat_out->A.v);
		const Vector3 tempB = m.Transform(insoleFlat_out->B.u,
				insoleFlat_out->B.v);
		const Vector3 tempC = m.Transform(insoleFlat_out->C.u,
				insoleFlat_out->C.v);
		const Vector3 tempD = m.Transform(insoleFlat_out->D.u,
				insoleFlat_out->D.v);
		const Vector3 tempE = m.Transform(insoleFlat_out->E.u,
				insoleFlat_out->E.v);
		const Vector3 tempF = m.Transform(insoleFlat_out->F.u,
				insoleFlat_out->F.v);
		const Vector3 tempG = m.Transform(insoleFlat_out->G.u,
				insoleFlat_out->G.v);
		const Vector3 tempH = m.Transform(insoleFlat_out->H.u,
				insoleFlat_out->H.v);
		const Vector3 tempJ = m.Transform(insoleFlat_out->J.u,
				insoleFlat_out->J.v);
		const Vector3 tempK = m.Transform(insoleFlat_out->K.u,
				insoleFlat_out->K.v);
		const Vector3 tempL = m.Transform(insoleFlat_out->L.u,
				insoleFlat_out->L.v);
		const Vector3 tempM = m.Transform(insoleFlat_out->M.u,
				insoleFlat_out->M.v);
		const Vector3 tempN = m.Transform(insoleFlat_out->N.u,
				insoleFlat_out->N.v);
		const Vector3 tempP = m.Transform(insoleFlat_out->P.u,
				insoleFlat_out->P.v);
		const Vector3 tempZ = m.Transform(insoleFlat_out->Z.u,
				insoleFlat_out->Z.v);

		insoleFlat_out->A.u = tempA.x;
		insoleFlat_out->A.v = tempA.y;
		insoleFlat_out->B.u = tempB.x;
		insoleFlat_out->B.v = tempB.y;
		insoleFlat_out->C.u = tempC.x;
		insoleFlat_out->C.v = tempC.y;
		insoleFlat_out->D.u = tempD.x;
		insoleFlat_out->D.v = tempD.y;
		insoleFlat_out->E.u = tempE.x;
		insoleFlat_out->E.v = tempE.y;
		insoleFlat_out->F.u = tempF.x;
		insoleFlat_out->F.v = tempF.y;
		insoleFlat_out->G.u = tempG.x;
		insoleFlat_out->G.v = tempG.y;
		insoleFlat_out->H.u = tempH.x;
		insoleFlat_out->H.v = tempH.y;
		insoleFlat_out->J.u = tempJ.x;
		insoleFlat_out->J.v = tempJ.y;
		insoleFlat_out->K.u = tempK.x;
		insoleFlat_out->K.v = tempK.y;
		insoleFlat_out->L.u = tempL.x;
		insoleFlat_out->L.v = tempL.y;
		insoleFlat_out->M.u = tempM.x;
		insoleFlat_out->M.v = tempM.y;
		insoleFlat_out->N.u = tempN.x;
		insoleFlat_out->N.v = tempN.y;
		insoleFlat_out->P.u = tempP.x;
		insoleFlat_out->P.v = tempP.y;
		insoleFlat_out->Z.u = tempZ.x;
		insoleFlat_out->Z.v = tempZ.y;

		if (m.GetOrientation() == AffineTransformMatrix::Orientation::LHS)
			flagFlipped = true;
		insoleFlat_out->outline.Reverse();
	}
	insoleFlat_out->FlagUV(true, false);
	insoleFlat_out->CalculateUVCoordinateSystems();
	insole_out->FlagUV(true, false);
	insole_out->CalculateUVCoordinateSystems();

	// Sort and map the outline to the unflattened insole.
	insoleFlat_out->outline.SortLoop();

	insole_out->outline.Clear();
	for (size_t idx = 0; idx < insoleFlat_out->outline.CountVertices(); idx++) {
		Geometry::Vertex v = ClosestVertex(
				insoleFlat_out->outline.GetVertex(idx), *insole_out);
		insole_out->outline.AddEdgeToVertex(v);
	}
	insole_out->outline.CloseLoopNextGroup();

	// Map XYZ coordinates of the construction points on the outline from
	// flattened to normal insole. This is done using the UV coordinates and
	// the coordinate system in the triangles. This applies only to the points
	// not on the outline: C, D, H, J, M, N.

	// Calculate the normal vectors in UV on each side of the triangle pointing
	// outwards.
	std::vector<Vector3> triNormal(insole_out->CountTriangles() * 3);
	for (size_t idx = 0; idx < insole_out->CountTriangles(); idx++) {
		for (uint8_t eidx = 0; eidx < 3; eidx++) {
			const Geometry::Vertex &v0 = insole_out->GetTriangleVertex(idx,
					eidx);
			const Geometry::Vertex &v1 = insole_out->GetTriangleVertex(idx,
					(eidx + 1) % 3);
			Geometry::Vertex temp = Normal(Diff(v1, v0));
			if (flagFlipped)
				triNormal[idx * 3 + eidx].Set(-temp.v, temp.u); // Rotate +90 deg.
			else
				triNormal[idx * 3 + eidx].Set(temp.v, -temp.u); // Rotate -90 deg.
		}
	}

	insole_out->C = insoleFlat_out->C;
	insole_out->D = insoleFlat_out->D;
	insole_out->H = insoleFlat_out->H;
	insole_out->J = insoleFlat_out->J;
	insole_out->M = insoleFlat_out->M;
	insole_out->N = insoleFlat_out->N;

	MapUVtoXYZ(insole_out->C, *insole_out, triNormal);
	MapUVtoXYZ(insole_out->D, *insole_out, triNormal);
	MapUVtoXYZ(insole_out->H, *insole_out, triNormal);
	MapUVtoXYZ(insole_out->J, *insole_out, triNormal);
	MapUVtoXYZ(insole_out->M, *insole_out, triNormal);
	MapUVtoXYZ(insole_out->N, *insole_out, triNormal);

	// Calculate the U coordinate of the outline used for the last coordinate
	// system. (V is always 0.0.)

	MapLinear(insoleFlat_out->outline, insoleFlat_out->B, insoleFlat_out->A, 0,
	M_PI);
	MapLinear(insoleFlat_out->outline, insoleFlat_out->A, insoleFlat_out->B,
			-M_PI, 0);

	// MapLinear only sets the U values. V is set to 0.0. Copy the U values
	// to the unflattened outline.
	for (size_t idx = 0; idx < insoleFlat_out->outline.CountVertices(); idx++) {
		Geometry::Vertex &v0 = insoleFlat_out->outline.GetVertex(idx);
		Geometry::Vertex &v1 = insole_out->outline.GetVertex(idx);
		v0.v = 0.0;
		v1.u = v0.u;
		v1.v = 0.0;
	}

	// Map all construction points UV to the outline. Map the unflattened
	// construction points at the same time.

	MapPointToOutline(insoleFlat_out->A, insole_out->A, insoleFlat_out->outline,
			insole_out->outline);
	MapPointToOutline(insoleFlat_out->B, insole_out->B, insoleFlat_out->outline,
			insole_out->outline);
	MapPointToOutline(insoleFlat_out->E, insole_out->E, insoleFlat_out->outline,
			insole_out->outline);
	MapPointToOutline(insoleFlat_out->F, insole_out->F, insoleFlat_out->outline,
			insole_out->outline);
	MapPointToOutline(insoleFlat_out->G, insole_out->G, insoleFlat_out->outline,
			insole_out->outline);
	MapPointToOutline(insoleFlat_out->K, insole_out->K, insoleFlat_out->outline,
			insole_out->outline);
	MapPointToOutline(insoleFlat_out->L, insole_out->L, insoleFlat_out->outline,
			insole_out->outline);
	MapPointToOutline(insoleFlat_out->P, insole_out->P, insoleFlat_out->outline,
			insole_out->outline);
	MapPointToOutline(insoleFlat_out->Z, insole_out->Z, insoleFlat_out->outline,
			insole_out->outline);

#ifdef DEBUG
	{
		Exporter ex("/tmp/geo.mat");
		ex.Add(*insoleFlat_out, "insoleflat");
		ex.Add(insoleFlat_out->outline, "outlineflat");
		ex.Add(*insole_out, "insole");
		ex.Add(insole_out->outline, "outline");
	}
#endif

	insole_out->MarkValid(true);
	insoleFlat_out->MarkValid(true);
	insole_out->MarkNeeded(false);
	insoleFlat_out->MarkNeeded(false);
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
	const double dx = b.x - a.x;
	const double dy = b.y - a.y;
	return sqrt(dx * dx + dy * dy);
}

Geometry::Vertex InsoleAnalyze::Normal(Geometry::Vertex a) const {
	const double d = sqrt(a.x * a.x + a.y * a.y);
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
		const double L = sqrt(dir.x * dir.x + dir.y * dir.y);
		if (L > FLT_EPSILON) {
			const double F = 1.0 / (L * L);
			dir.x *= F;
			dir.y *= F;
		}
	}
	Polygon3::Intersections ret;
	for (size_t idx = 0; idx < geo.CountEdges(); ++idx) {
		const Geometry::Vertex &vert0 = geo.GetEdgeVertex(idx, 0);
		const Geometry::Vertex &vert1 = geo.GetEdgeVertex(idx, 1);
		Vector3 v0(vert0.x - a.x, vert0.y - a.y);
		Vector3 v1(vert1.x - a.x, vert1.y - a.y);
		const double h0 = v0.x * dir.x + v0.y * dir.y;
		const double h1 = v1.x * dir.x + v1.y * dir.y;
		if (h0 <= relDistance && h1 > relDistance)
			ret.positive.push_back(
					vert0.Interp(vert1, (relDistance - h0) / (h1 - h0)));
		if (h0 >= relDistance && h1 < relDistance)
			ret.negative.push_back(
					vert0.Interp(vert1, (relDistance - h0) / (h1 - h0)));
	}
	return ret;
}

Geometry::Vertex InsoleAnalyze::ClosestVertex(const Geometry::Vertex &v,
		const Geometry &dst) const {

	// Find closest triangle to point v.
	auto dist = [](double a, double b) {
		return sqrt(a * a + b * b);
	};
	size_t idx = 0;
	double dmin = DBL_MAX;
	for (size_t vidx = 0; vidx < dst.CountVertices(); vidx++) {
		const Geometry::Vertex &vdest = dst.GetVertex(vidx);
		const double d = dist(v.u - vdest.u, v.v - vdest.v);
		if (d < dmin) {
			dmin = d;
			idx = vidx;
		}
	}
	return dst.GetVertex(idx);
}

void InsoleAnalyze::MapLinear(Polygon3 &geo, const Insole::Point &p0,
		const Insole::Point &p1, double u0, double u1) const {
	size_t idx0 = 0;
	size_t idx1 = 0;
	double dMin0 = DBL_MAX;
	double dMin1 = DBL_MAX;
	double r0 = 0.0;
	double r1 = 0.0;

	for (size_t idx = 0; idx < geo.CountEdges(); idx++) {
		const Geometry::Vertex &v0 = geo.GetEdgeVertex(idx, 0);
		const Geometry::Vertex &v1 = geo.GetEdgeVertex(idx, 1);
		Geometry::Vertex dir = Diff(v1, v0);
		const double L = dir.Abs();
		Vector3 n = Vector3(dir.y, -dir.x, dir.z) / L; // Rotate -90 deg.
		Geometry::Vertex temp0 = Diff(p0, v0);
		Geometry::Vertex temp1 = Diff(p1, v0);
		const double cr0 = (temp0.x * dir.x + temp0.y * dir.y) / L;
		const double cr1 = (temp1.x * dir.x + temp1.y * dir.y) / L;
		const double cn0 = (temp0.x * n.x + temp0.y * n.y) / L;
		const double cn1 = (temp1.x * n.x + temp1.y * n.y) / L;

		// The distance is measured in a rectangle from the edge. A capsule
		// would be more precise, but all relevant points sit on the outline
		// anyway, so it does not matter. This is mainly introduced to catch
		// edge-cases that might arise from resampling the outline.

		double d0 = fabs(cn0);
		if ((cr0 - L) > 0.0 && (cr0 - L) > d0)
			d0 = cr0 - L;
		if (cr0 < 0.0 && -cr0 > d0)
			d0 = -cr0;

		double d1 = fabs(cn1);
		if ((cr1 - L) > 0.0 && (cr1 - L) > d1)
			d1 = cr1 - L;
		if (cr1 < 0.0 && -cr1 > d1)
			d1 = -cr1;

		if (d0 < dMin0) {
			dMin0 = d0;
			r0 = cr0 / L;
			idx0 = idx;
		}
		if (d1 < dMin1) {
			dMin1 = d1;
			r1 = cr1 / L;
			idx1 = idx;
		}
	}

	// Calculate the loop length between p0 and p1.
	double L = 0;
	{
		const Geometry::Vertex &v0 = geo.GetEdgeVertex(idx0, 0);
		const Geometry::Vertex &v1 = geo.GetEdgeVertex(idx0, 1);
		L += Dist(v1, v0) * (1.0 - r0);
	}
	size_t idx = idx0;
	while (idx != idx1) {
		const Geometry::Vertex &v0 = geo.GetEdgeVertex(idx, 0);
		const Geometry::Vertex &v1 = geo.GetEdgeVertex(idx, 1);
		L += Dist(v1, v0);
		idx = (idx + 1) % geo.CountVertices();
	}
	{
		const Geometry::Vertex &v0 = geo.GetEdgeVertex(idx0, 0);
		const Geometry::Vertex &v1 = geo.GetEdgeVertex(idx0, 1);
		L += Dist(v1, v0) * r1;
	}

	const Polynomial mapLU = Polynomial::ByValue(0.0, u0, L, u1);

	// Interpolate the measured distance.
	L = 0;
	{
		const Geometry::Edge &e = geo.GetEdge(idx0);
		const size_t vidx0 = e.GetVertexIndex(0);
		const size_t vidx1 = e.GetVertexIndex(1);
		Geometry::Vertex &v0 = geo.GetVertex(vidx0);
		Geometry::Vertex &v1 = geo.GetVertex(vidx1);
		if (r0 < FLT_EPSILON)
			v0.u = mapLU(L);
		L += Dist(v1, v0) * (1.0 - r0);
		v1.u = mapLU(L);
	}
	idx = idx0;
	while (idx != idx1) {
		const Geometry::Edge &e = geo.GetEdge(idx);
		const size_t vidx0 = e.GetVertexIndex(0);
		const size_t vidx1 = e.GetVertexIndex(1);
		Geometry::Vertex &v0 = geo.GetVertex(vidx0);
		Geometry::Vertex &v1 = geo.GetVertex(vidx1);
		L += Dist(v1, v0);
		v1.u = mapLU(L);
		idx = (idx + 1) % geo.CountVertices();
	}
	{
		const Geometry::Edge &e = geo.GetEdge(idx1);
		const size_t vidx0 = e.GetVertexIndex(0);
		const size_t vidx1 = e.GetVertexIndex(1);
		Geometry::Vertex &v0 = geo.GetVertex(vidx0);
		Geometry::Vertex &v1 = geo.GetVertex(vidx1);
		if (r1 >= 1.0 - FLT_EPSILON) {
			L += Dist(v1, v0) * r1;
			v1.u = mapLU(L);
		}
	}
//	DEBUGOUT << "Length of outline between p0 and p1: " << L * 100.0 << " cm\n";
}

void InsoleAnalyze::MapPointToOutline(Insole::Point &p0, Insole::Point &p1,
		Polygon3 &outline0, Polygon3 &outline1) const {

	size_t idx0 = 0;
	double dMin0 = DBL_MAX;
	double r0 = 0.0;

	for (size_t idx = 0; idx < outline0.CountEdges(); idx++) {
		const Geometry::Vertex &v0 = outline0.GetEdgeVertex(idx, 0);
		const Geometry::Vertex &v1 = outline0.GetEdgeVertex(idx, 1);
		Geometry::Vertex dir = Diff(v1, v0);
		const double L = dir.Abs();
		Vector3 n = Vector3(dir.y, -dir.x, dir.z); // Rotate -90 deg.
		Geometry::Vertex temp0 = Diff(p0, v0);
		const double cr0 = (temp0.x * dir.x + temp0.y * dir.y) / L;
		const double cn0 = (temp0.x * n.x + temp0.y * n.y) / L;

		// The distance is measured in a rectangle from the edge. A capsule
		// would be more precise, but all relevant points sit on the outline
		// anyway, so it does not matter. This is mainly introduced to catch
		// edge-cases that might arise from resampling the outline.

		double d0 = fabs(cn0);
		if ((cr0 - L) > 0.0 && (cr0 - L) > d0)
			d0 = cr0 - L;
		if (cr0 < 0.0 && -cr0 > d0)
			d0 = -cr0;

		if (d0 < dMin0) {
			dMin0 = d0;
			r0 = cr0 / L;
			idx0 = idx;
		}
	}
	// For the first (flattened) point, only adjust UV.
	{
		const Geometry::Vertex &v0 = outline0.GetEdgeVertex(idx0, 0);
		const Geometry::Vertex &v1 = outline0.GetEdgeVertex(idx0, 1);
		Geometry::Vertex temp = v0.Interp(v1, r0);
		p0.u = temp.u;
		p0.v = temp.v;
	}
	// For the second (unflattened) point, set all XYZUV.
	{
		const Geometry::Vertex &v0 = outline1.GetEdgeVertex(idx0, 0);
		const Geometry::Vertex &v1 = outline1.GetEdgeVertex(idx0, 1);
		Geometry::Vertex temp = v0.Interp(v1, r0);
		p1.x = temp.x;
		p1.y = temp.y;
		p1.z = temp.z;
		p1.u = temp.u;
		p1.v = temp.v;
	}
}

void InsoleAnalyze::MapUVtoXYZ(Insole::Point &p, const Geometry &dst,
		const std::vector<Vector3> &triNormal) const {
	// Find closest triangle to point p.

	size_t idx = 0;
	double dmin = DBL_MAX;
	for (size_t tidx = 0; tidx < dst.CountTriangles(); tidx++) {
		// Calculate the largest distance of the three sides of a triangle.
		double d = -DBL_MAX;
		for (uint_fast8_t eidx = 0; eidx < 3; eidx++) {
			const Geometry::Vertex &v = dst.GetTriangleVertex(tidx, eidx);
			const Vector3 &n = triNormal[tidx * 3 + eidx];
			const double dl = n.x * (p.u - v.u) + n.y * (p.v - v.v);
			if (dl > d)
				d = dl;
		}
		if (d < dmin) {
			dmin = d;
			idx = tidx;
		}
	}

	// The UV coordinate systems (tangent, bitrange, normal) in each triangle
	// map the UV coordinates to XYZ coordinates.
	// Calculate the inverse coordinate systems for all triangles in the
	// unflattened insole.

	const Geometry::Triangle &t = dst.GetTriangle(idx);
	AffineTransformMatrix m;
	m.SetEx(t.t);
	m.SetEy(t.b);
	m.SetEz(t.n);
	// Calculate the origin
	const Geometry::Vertex &v = dst.GetVertex(t.va);
	Vector3 temp = m.Transform(t.tua, t.tva);
	m.SetOrigin( { v.x - temp.x, v.y - temp.y, v.z - temp.z });
	Vector3 pos = m.Transform(p.u, p.v);
	p.x = pos.x;
	p.y = pos.y;
	p.z = pos.z;
}
