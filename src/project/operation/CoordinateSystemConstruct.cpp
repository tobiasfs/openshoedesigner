///////////////////////////////////////////////////////////////////////////////
// Name               : CoordinateSystemConstruct.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 29.12.2024
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
#include "CoordinateSystemConstruct.h"

#include "../../3D/Ellipse.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

CoordinateSystemConstruct::CoordinateSystemConstruct() {
	out = std::make_shared<CoordinateSystem>();
}

std::string CoordinateSystemConstruct::GetName() const {
	return "CoordinateSystemConstruct";
}

bool CoordinateSystemConstruct::CanRun() {
	std::string missing;

	if (!in)
		missing += missing.empty() ? "\"in\"" : ", \"in\"";
	if (!out)
		missing += missing.empty() ? "\"out\"" : ", \"out\"";
	if (!belowCrotchGirth)
		missing +=
				missing.empty() ?
						"\"belowCrotchGirth\"" : ", \"belowCrotchGirth\"";
	if (!belowCrotchLevel)
		missing +=
				missing.empty() ?
						"\"belowCrotchLevel\"" : ", \"belowCrotchLevel\"";
	if (!middleOfThighGirth)
		missing +=
				missing.empty() ?
						"\"middleOfThighGirth\"" : ", \"middleOfThighGirth\"";
	if (!middleOfThighLevel)
		missing +=
				missing.empty() ?
						"\"middleOfThighLevel\"" : ", \"middleOfThighLevel\"";
	if (!aboveKneeGirth)
		missing +=
				missing.empty() ? "\"aboveKneeGirth\"" : ", \"aboveKneeGirth\"";
	if (!aboveKneeLevel)
		missing +=
				missing.empty() ? "\"aboveKneeLevel\"" : ", \"aboveKneeLevel\"";
	if (!overKneeCapGirth)
		missing +=
				missing.empty() ?
						"\"overKneeCapGirth\"" : ", \"overKneeCapGirth\"";
	if (!overKneeCapLevel)
		missing +=
				missing.empty() ?
						"\"overKneeCapLevel\"" : ", \"overKneeCapLevel\"";
	if (!belowKneeGirth)
		missing +=
				missing.empty() ? "\"belowKneeGirth\"" : ", \"belowKneeGirth\"";
	if (!belowKneeLevel)
		missing +=
				missing.empty() ? "\"belowKneeLevel\"" : ", \"belowKneeLevel\"";
	if (!middleOfShankGirth)
		missing +=
				missing.empty() ?
						"\"middleOfShankGirth\"" : ", \"middleOfShankGirth\"";
	if (!middleOfShankLevel)
		missing +=
				missing.empty() ?
						"\"middleOfShankLevel\"" : ", \"middleOfShankLevel\"";
	if (!aboveAnkleGirth)
		missing +=
				missing.empty() ?
						"\"aboveAnkleGirth\"" : ", \"aboveAnkleGirth\"";
	if (!aboveAnkleLevel)
		missing +=
				missing.empty() ?
						"\"aboveAnkleLevel\"" : ", \"aboveAnkleLevel\"";
	if (!overAnkleBoneGirth)
		missing +=
				missing.empty() ?
						"\"overAnkleBoneGirth\"" : ", \"overAnkleBoneGirth\"";
	if (!overAnkleBoneLevel)
		missing +=
				missing.empty() ?
						"\"overAnkleBoneLevel\"" : ", \"overAnkleBoneLevel\"";

	if (!missing.empty()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " -";
		err << "The variables " << missing << " are not connected.";
		error = err.str();
		throw std::logic_error(err.str());
	}

	error.clear();

	if (belowCrotchGirth->GetString().empty()) {
		error +=
				" Input \"belowCrotchGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (belowCrotchLevel->GetString().empty()) {
		error +=
				" Input \"belowCrotchLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (middleOfThighGirth->GetString().empty()) {
		error +=
				" Input \"middleOfThighGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (middleOfThighLevel->GetString().empty()) {
		error +=
				" Input \"middleOfThighLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (aboveKneeGirth->GetString().empty()) {
		error +=
				" Input \"aboveKneeGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (aboveKneeLevel->GetString().empty()) {
		error +=
				" Input \"aboveKneeLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (overKneeCapGirth->GetString().empty()) {
		error +=
				" Input \"overKneeCapGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (overKneeCapLevel->GetString().empty()) {
		error +=
				" Input \"overKneeCapLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (belowKneeGirth->GetString().empty()) {
		error +=
				" Input \"belowKneeGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (belowKneeLevel->GetString().empty()) {
		error +=
				" Input \"belowKneeLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (middleOfShankGirth->GetString().empty()) {
		error +=
				" Input \"middleOfShankGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (middleOfShankLevel->GetString().empty()) {
		error +=
				" Input \"middleOfShankLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (aboveAnkleGirth->GetString().empty()) {
		error +=
				" Input \"aboveAnkleGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (aboveAnkleLevel->GetString().empty()) {
		error +=
				" Input \"aboveAnkleLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (overAnkleBoneGirth->GetString().empty()) {
		error +=
				" Input \"overAnkleBoneGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (overAnkleBoneLevel->GetString().empty()) {
		error +=
				" Input \"overAnkleBoneLevel\" for CoordinateSystemConstruct is empty.";
	}

	return error.empty();
}

bool CoordinateSystemConstruct::Propagate() {
	if (!in || !out || !belowCrotchGirth || !belowCrotchLevel
			|| !middleOfThighGirth || !middleOfThighLevel || !aboveKneeGirth
			|| !aboveKneeLevel || !overKneeCapGirth || !overKneeCapLevel
			|| !belowKneeGirth || !belowKneeLevel || !middleOfShankGirth
			|| !middleOfShankLevel || !aboveAnkleGirth || !aboveAnkleLevel
			|| !overAnkleBoneGirth || !overAnkleBoneLevel)
		return false;

	bool parameterModified = false;
	parameterModified |= !in->IsValid();
	parameterModified |= belowCrotchGirth->IsModified();
	parameterModified |= belowCrotchLevel->IsModified();
	parameterModified |= middleOfThighGirth->IsModified();
	parameterModified |= middleOfThighLevel->IsModified();
	parameterModified |= aboveKneeGirth->IsModified();
	parameterModified |= aboveKneeLevel->IsModified();
	parameterModified |= overKneeCapGirth->IsModified();
	parameterModified |= overKneeCapLevel->IsModified();
	parameterModified |= belowKneeGirth->IsModified();
	parameterModified |= belowKneeLevel->IsModified();
	parameterModified |= middleOfShankGirth->IsModified();
	parameterModified |= middleOfShankLevel->IsModified();
	parameterModified |= aboveAnkleGirth->IsModified();
	parameterModified |= aboveAnkleLevel->IsModified();
	parameterModified |= overAnkleBoneGirth->IsModified();
	parameterModified |= overAnkleBoneLevel->IsModified();

	bool modify = false;
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

bool CoordinateSystemConstruct::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void CoordinateSystemConstruct::InitLeg() {
	// Main point is the ankle and all ellipses are placed relative to this
	// point.
	const double ankleHeight = overAnkleBoneLevel->ToDouble();
	const double ankleRotation = atan2(in->J.n.x, in->J.n.z);
	const Vector3 anklePoint = in->J + in->J.n * ankleHeight;
	double xFront = 0.0;
	ellipses.clear();
	ellipsesV.clear();
	{
		// Ankle

		Ellipse el;
		el *= AffineTransformMatrix::RotationAroundVector( { 0, 1, 0 },
				ankleRotation / 3.0);
		el.SetOrigin(anklePoint);
		el.ScaleLocal(1.0, 0.75, 1.0);
		const double temp = overAnkleBoneGirth->ToDouble() / el.Circumfence();
		el.ScaleLocal(temp, temp, temp);
		ellipses.push_back(el.GetBezierSpline(4));
		ellipsesV.push_back(1.0);
	}

	const double L12 = overKneeCapLevel->ToDouble()
			- overAnkleBoneLevel->ToDouble();
	const double L23 = belowCrotchLevel->ToDouble()
			- overKneeCapLevel->ToDouble();
	{
		// Above ankle
		Ellipse el;
		el *= AffineTransformMatrix::RotationAroundVector( { 0, 1, 0 },
				ankleRotation / 4.0);
		el.SetOrigin(
				anklePoint
						+ Vector3(0, 0,
								aboveAnkleLevel->ToDouble() - ankleHeight));
		el.ScaleLocal(1.0, 1.0, 1.0);
		const double temp = aboveAnkleGirth->ToDouble() / el.Circumfence();
		el.ScaleLocal(temp, temp, temp);
		ellipses.push_back(el.GetBezierSpline(4));
		ellipsesV.push_back(
				1.0 + (aboveAnkleLevel->ToDouble() - ankleHeight) / L12);
		xFront = el.GetOrigin().x + el.GetEx().x;
	}

	{
		// Middle of shank
		Ellipse el;
		el.SetOrigin(
				anklePoint
						+ Vector3(0, 0,
								middleOfShankLevel->ToDouble() - ankleHeight));
		el.ScaleLocal(1.0, 1.0, 1.0);
		const double temp = middleOfShankGirth->ToDouble() / el.Circumfence();
		el.ScaleLocal(temp, temp, temp);
		el.TranslateGlobal(xFront - el.GetOrigin().x - el.GetEx().x, 0, 0);
		ellipses.push_back(el.GetBezierSpline(4));
		ellipsesV.push_back(
				1.0 + (middleOfShankLevel->ToDouble() - ankleHeight) / L12);
	}
	{
		// Below the knee
		Ellipse el;
		el.SetOrigin(
				anklePoint
						+ Vector3(0, 0,
								belowKneeLevel->ToDouble() - ankleHeight));
		el.ScaleLocal(1.0, 1.0, 1.0);
		const double temp = belowKneeGirth->ToDouble() / el.Circumfence();
		el.ScaleLocal(temp, temp, temp);
		el.TranslateGlobal(xFront - el.GetOrigin().x - el.GetEx().x, 0, 0);
		ellipses.push_back(el.GetBezierSpline(4));
		ellipsesV.push_back(
				1.0 + (belowKneeLevel->ToDouble() - ankleHeight) / L12);
	}
	{
		// Over the knee-cap
		Ellipse el;
		el.SetOrigin(
				anklePoint
						+ Vector3(0, 0,
								overKneeCapLevel->ToDouble() - ankleHeight));
		el.ScaleLocal(1.0, 1.0, 1.0);
		const double temp = overKneeCapGirth->ToDouble() / el.Circumfence();
		el.ScaleLocal(temp, temp, temp);
		el.TranslateGlobal((xFront - el.GetOrigin().x - el.GetEx().x) * 0.9, 0,
				0);
		ellipses.push_back(el.GetBezierSpline(4));
		ellipsesV.push_back(2.0);
	}

	{
		// Above the knee
		Ellipse el;
		el.SetOrigin(
				anklePoint
						+ Vector3(0, 0,
								aboveKneeLevel->ToDouble() - ankleHeight));
		el.ScaleLocal(1.0, 1.0, 1.0);
		const double temp = aboveKneeGirth->ToDouble() / el.Circumfence();
		el.ScaleLocal(temp, temp, temp);
		el.TranslateGlobal(xFront - el.GetOrigin().x - el.GetEx().x, 0, 0);
		ellipses.push_back(el.GetBezierSpline(4));
		ellipsesV.push_back(2.1);
	}
	{
		// Middle of thigh
		Ellipse el;
		el.SetOrigin(
				anklePoint
						+ Vector3(0, 0,
								middleOfThighLevel->ToDouble() - ankleHeight));
		el.ScaleLocal(1.0, 1.0, 1.0);
		const double temp = middleOfThighGirth->ToDouble() / el.Circumfence();
		el.ScaleLocal(temp, temp, temp);
		el.TranslateGlobal((xFront - el.GetOrigin().x - el.GetEx().x) * 1.0, 0,
				0);
		ellipses.push_back(el.GetBezierSpline(4));
		ellipsesV.push_back(2.5);
	}
	{
		// Below the crotch
		Ellipse el;
		el.SetOrigin(
				anklePoint
						+ Vector3(0, 0,
								belowCrotchLevel->ToDouble() - ankleHeight));
		el.ScaleLocal(1.0, 0.9, 1.0);
		const double temp = belowCrotchGirth->ToDouble() / el.Circumfence();
		el.ScaleLocal(temp, temp, temp);
		el.TranslateGlobal((xFront - el.GetOrigin().x - el.GetEx().x) * 1.0, 0,
				0);
		ellipses.push_back(el.GetBezierSpline(4));
		ellipsesV.push_back(3.0);
	}
	{
		// Extension
		Ellipse el;
		el.SetOrigin(
				anklePoint
						+ Vector3(0, 0,
								overKneeCapLevel->ToDouble()
										+ 2.0
												* (belowCrotchLevel->ToDouble()
														- overKneeCapLevel->ToDouble())
										- ankleHeight));
		el.ScaleLocal(1.0, 0.8, 1.0);
		const double temp = 1.25 * belowCrotchGirth->ToDouble()
				/ el.Circumfence();
		el.ScaleLocal(temp, temp, temp);
		el.TranslateGlobal((xFront - el.GetOrigin().x - el.GetEx().x) * 1.0, 0,
				0);
		ellipses.push_back(el.GetBezierSpline(4));
		ellipsesV.push_back(4.0);
	}

	if (ellipses.size() != ellipsesV.size()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " -";
		err << "Programming error: 'ellipses' has " << ellipses.size()
				<< " values and 'ellipsesV' has " << ellipsesV.size()
				<< " values. This should be the same.";
		error = err.str();
		throw std::logic_error(err.str());
	}
}

void CoordinateSystemConstruct::Run() {

	InitLeg();
	out->Clear();
	out->Setup(4, 1, -M_PI, M_PI, 0, 1);
	out->Setup(4, 1, -M_PI, M_PI, 1, 2);
	out->Setup(4, 1, -M_PI, M_PI, 2, 3);
	out->Setup(4, 1, -M_PI, M_PI, 3, 4);

	out->SetOrder(4, 4, -M_PI, M_PI, 0, 1);
	out->SetOrder(4, 5, -M_PI, M_PI, 1, 2);
	out->SetOrder(4, 4, -M_PI, M_PI, 2, 3);
	out->SetOrder(4, 2, -M_PI, M_PI, 3, 4);

	out->ClearBoundaries();
	out->HardBoundaries();

	out->AddStitching(-M_PI, M_PI, -DBL_MAX, 0.99, 1, true, false, true);
	out->AddStitching(-M_PI, M_PI, 0.99, 1.01, 0, true, false, false);
	out->AddStitching(-M_PI, M_PI, 1.01, 2.99, 1, true, false, true);
	out->AddStitching(-M_PI, M_PI, 2.99, DBL_MAX, 1, true, false, false);

	out->SoftBoundaries();
	for (size_t idx = 0; idx < in->outline.CountVertices(); idx++) {
		out->AddPoint(in->outline.GetVertex(idx));
	}

	for (size_t idx = 0; idx < ellipses.size(); idx++) {
		double dummy;
		double frac = std::modf(ellipsesV[idx], &dummy);
		if (fabs(frac) <= FLT_EPSILON)
			out->HardBoundaries();
		else
			out->SoftBoundaries();

		out->AddPolynomial(-M_PI, -M_PI_2, ellipsesV[idx], ellipsesV[idx],
				ellipses[idx].GetSegment(0));
		out->AddPolynomial(-M_PI_2, 0.0, ellipsesV[idx], ellipsesV[idx],
				ellipses[idx].GetSegment(1));
		out->AddPolynomial(0.0, M_PI_2, ellipsesV[idx], ellipsesV[idx],
				ellipses[idx].GetSegment(2));
		out->AddPolynomial(M_PI_2, M_PI, ellipsesV[idx], ellipsesV[idx],
				ellipses[idx].GetSegment(3));
	}

	out->SoftBoundaries();

	const double scale = overAnkleBoneLevel->ToDouble() * 0.8;
	for (size_t idx = 0; idx < in->outline.CountVertices(); idx++) {
		const Geometry::Vertex &v = in->outline.GetVertex(idx);
		out->AdddPdV(v.u, v.v, v.n * scale);
	}

	out->Calculate();
	out->Update();

//	double L = outline0.MapU(false); //FIXME This should be true, probably.
//	Polynomial m = Polynomial::ByValue(0, 0, L, 2.0 * M_PI);
//	auto inter = outline0.Intersect( { 0, 1, 0 }, 0);
//	if (!inter.positive.empty())
//		m.ShiftX(inter.positive[0].u);
//	for (size_t n = 0; n < outline0.CountVertices(); n++) {
//		auto &vert = outline0.GetVertex(n);
//		vert.u = m(vert.u);
//		while (vert.u > M_PI)
//			vert.u -= 2.0 * M_PI;
//		while (vert.u < -M_PI)
//			vert.u += 2.0 * M_PI;
//	}
//
//	{
//		Vector3 center = outline0.GetCenter();
//		auto inter2 = outline0.Intersect( { 0, 1, 0 }, center.y);
//		if (!inter2.positive.empty())
//			out->Add(inter2.positive[0]);
//		if (!inter2.negative.empty())
//			out->Add(inter2.negative[0]);
//	}
//	{

//
//		out->HardBoundaries();
//		Polynomial mapu = Polynomial::ByValue(0, -M_PI, 4, M_PI);
//		for (size_t n = 0; n < 4; n++)
//			out->AddPolynomial(mapu(n), mapu(n + 1), 1, 1, elb.GetSegment(n));
//		out->SoftBoundaries();
//	}
//	for (size_t n = 0; n < outline0.CountVertices(); n++)
//		out->Add(outline0.GetVertex(n), 0.05);
//	out->Calculate();
//	out->Update();

	out->MarkValid(true);
	out->MarkNeeded(false);
}

void CoordinateSystemConstruct::Paint() const {
#ifdef DEBUG
//	for (const Bezier3 &ellipse : ellipses)
//		ellipse.Paint();
#endif
}
