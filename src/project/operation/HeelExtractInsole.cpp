///////////////////////////////////////////////////////////////////////////////
// Name               : HeelExtractInsole.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 27.12.2024
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
#include "HeelExtractInsole.h"

#include <stdexcept>
#include <sstream>

HeelExtractInsole::HeelExtractInsole() {
	out = std::make_shared<Insole>();
}

std::string HeelExtractInsole::GetName() const {
	return "HeelExtractInsole";
}

bool HeelExtractInsole::CanRun() {
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
		error += " Input \"footLength\" for HeelExtractInsole is empty.";
	}
	if (ballMeasurementAngle->GetString().empty()) {
		error +=
				" Input \"ballMeasurementAngle\" for HeelExtractInsole is empty.";
	}
	if (heelDirectionAngle->GetString().empty()) {
		error +=
				" Input \"heelDirectionAngle\" for HeelExtractInsole is empty.";
	}
	if (littleToeAngle->GetString().empty()) {
		error += " Input \"littleToeAngle\" for HeelExtractInsole is empty.";
	}
	if (bigToeAngle->GetString().empty()) {
		error += " Input \"bigToeAngle\" for HeelExtractInsole is empty.";
	}
	if (extraLength->GetString().empty()) {
		error += " Input \"extraLength\" for HeelExtractInsole is empty.";
	}

	return error.empty();
}

bool HeelExtractInsole::Propagate() {
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

bool HeelExtractInsole::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void HeelExtractInsole::Run() {

	in->CalcGroups(22.5 / 180.0 * M_PI);
	std::map<size_t, double> groupdir;
	for (size_t n = 0; n < in->TriangleCount(); ++n) {
		const Geometry::Triangle &tri = in->GetTriangle(n);
		auto xi = groupdir.find(tri.group);
		if (xi == groupdir.end()) {
			groupdir[tri.group] = tri.n.z * in->GetTriangleArea(n);
		} else {
			xi->second += tri.n.z * in->GetTriangleArea(n);
		}
	}

	auto it = std::max_element(groupdir.begin(), groupdir.end(),
			[](const auto &lhs, const auto &rhs) {
				return lhs.second < rhs.second;
			});
	if (it == groupdir.end()) {
		error += GetName() + ": Could not find top surface of heel.";
		out->MarkValid(true);
		out->MarkNeeded(false);
		return;
	}

	in->SelectByGroup(it->first);
	out->Clear();
	out->AddSelectedFrom(*in);

	// Extract the outline of the sole
	Polygon3 outline;
	const Vector3 center = out->GetCenter();
	for (size_t n = 0; n < out->EdgeCount(); ++n) {
		const auto &e = out->GetEdge(n);
		if (e.trianglecount >= 2)
			continue;
		const auto &v0 = out->GetEdgeVertex(n, 0);
		const auto &v1 = out->GetEdgeVertex(n, 1);
		Vector3 rot = (v0 - center) * (v1 - v0);
		if (rot.z > 0.0)
			outline.AddEdge(v0, v1);
		else
			outline.AddEdge(v1, v0);
	}
	outline.Join();
	outline.SortLoop();

	// Find the center line A-B
	const Vector3 side = { 0, 1, 0 };

	auto intersect = outline.Intersect(side, 0);
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

	auto int2 = outline.Intersect(n, out->C.Dot(n));

	if (!int2.positive.empty())
		out->E = int2.positive[0];
	if (!int2.negative.empty())
		out->F = int2.negative[0];

	const double fl = footLength->ToDouble();
	const double ll = fl + extraLength->ToDouble();

	out->C = (out->E + out->F) / 2.0;
	out->J = out->A.Interp(out->C, fl / ll / 0.62 / 6);

	n = (out->J - out->A).Normal();
	rot = n * side;
	n = AffineTransformMatrix::RotationAroundVector(rot,
			heelDirectionAngle->ToDouble()).Transform(n);
	//	n.Set(1, 0, 0);

	auto int3 = outline.Intersect(n, out->J.Dot(n));

	if (!int3.positive.empty())
		out->K = int3.positive[0];
	if (!int3.negative.empty())
		out->L = int3.negative[0];

	out->J = (out->K + out->L) / 2.0;

	n = AffineTransformMatrix::RotationAroundVector(rot, -M_PI_2).Transform(n);
	auto int4 = outline.Intersect(n, out->J.Dot(n));
	if (!int4.positive.empty())
		out->N = int4.positive[0];

// Calculate foot/last relation

	out->D = out->C.Interp(out->B, fl / ll);

	n = (out->D - out->C).Normal();
	auto int5 = outline.Intersect(n, out->D.Dot(n));

	if (!int5.positive.empty())
		out->G = int5.positive[0];
	if (!int5.negative.empty())
		out->H = int5.negative[0];

	double fsl = (out->B - out->C).Abs() / 0.38 * fl / ll;
	Vector3 hz = out->D.Interp(out->C, (fsl / 5) / (out->B - out->C).Abs());
	auto int6 = outline.Intersect(n, hz.Dot(n));
	if (!int6.positive.empty())
		out->Z = int6.negative[0];

	out->H.y = out->F.y
			+ (out->Z.y - out->F.y) / (out->Z.x - out->F.x)
					* (out->H.x - out->F.x);

	out->MarkValid(true);
	out->MarkNeeded(false);
}
