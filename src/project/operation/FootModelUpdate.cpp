///////////////////////////////////////////////////////////////////////////////
// Name               : FootModelUpdate.cpp
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
#include "FootModelUpdate.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

FootModelUpdate::FootModelUpdate() {
	out = std::make_shared<FootModel>();
}

std::string FootModelUpdate::GetName() const {
	return "FootModelUpdate";
}

bool FootModelUpdate::CanRun() {
	std::string missing;

	if (!heelPitch)
		missing += missing.empty() ? "\"heelPitch\"" : ", \"heelPitch\"";
	if (!toeSpring)
		missing += missing.empty() ? "\"toeSpring\"" : ", \"toeSpring\"";
	if (!heelHeight)
		missing += missing.empty() ? "\"heelHeight\"" : ", \"heelHeight\"";
	if (!ballHeight)
		missing += missing.empty() ? "\"ballHeight\"" : ", \"ballHeight\"";
	if (!legLengthDifference)
		missing +=
				missing.empty() ?
						"\"legLengthDifference\"" : ", \"legLengthDifference\"";
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

	if (heelPitch->GetString().empty()) {
		error += " Input \"heelPitch\" for FootModelUpdate is empty.";
	}
	if (toeSpring->GetString().empty()) {
		error += " Input \"toeSpring\" for FootModelUpdate is empty.";
	}
	if (heelHeight->GetString().empty()) {
		error += " Input \"heelHeight\" for FootModelUpdate is empty.";
	}
	if (ballHeight->GetString().empty()) {
		error += " Input \"ballHeight\" for FootModelUpdate is empty.";
	}
	if (legLengthDifference->GetString().empty()) {
		error += " Input \"legLengthDifference\" for FootModelUpdate is empty.";
	}

	return error.empty();
}

bool FootModelUpdate::Propagate() {
	if (!heelPitch || !toeSpring || !heelHeight || !ballHeight
			|| !legLengthDifference || !in || !out)
		return false;

	bool modify = false;
	bool parameterModified = false;
	parameterModified |= !in->IsValid();
	parameterModified |= heelPitch->IsModified();
	parameterModified |= toeSpring->IsModified();
	parameterModified |= heelHeight->IsModified();
	parameterModified |= ballHeight->IsModified();
	parameterModified |= legLengthDifference->IsModified();

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

bool FootModelUpdate::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void FootModelUpdate::Run() {
	*out = *in;

	throw std::runtime_error("Not connected to \"UpdatePosition(...)\".");

	out->MarkValid(true);
	out->MarkNeeded(false);
}

void FootModelUpdate::UpdatePosition(const Shoe &shoe, double offset,
		double mixing) {

	std::shared_ptr<Bone> PhalanxI1 = out->GetBone("PhalanxI1");
	std::shared_ptr<Bone> PhalanxI2 = out->GetBone("PhalanxI2");
	std::shared_ptr<Bone> PhalanxI3 = out->GetBone("PhalanxI3");
	std::shared_ptr<Bone> PhalanxI4 = out->GetBone("PhalanxI4");
	std::shared_ptr<Bone> PhalanxI5 = out->GetBone("PhalanxI5");
	std::shared_ptr<Bone> Talus = out->GetBone("Talus");
	std::shared_ptr<Bone> Calcaneus = out->GetBone("Calcaneus");
	std::shared_ptr<Bone> Cuboideum = out->GetBone("Cuboideum");
	std::shared_ptr<Bone> Cuneiforme1 = out->GetBone("Cuneiforme1");
	std::shared_ptr<Bone> Cuneiforme2 = out->GetBone("Cuneiforme2");
	std::shared_ptr<Bone> Cuneiforme3 = out->GetBone("Cuneiforme3");

	double heelHeight_ = heelHeight->ToDouble();
	double ballHeight_ = ballHeight->ToDouble();
	double toeAngle_ = toeSpring->ToDouble();
	heelHeight_ += offset; // Compensation for legs of different length

	optiPos.reevalBest = true;
	optiPos.keepSimplex = true;
	optiPos.simplexSpread = 0.1;
	optiPos.evalLimit = 30;
	optiPos.errorLimit = 0.0005;
	optiPos.param.resize(1, 0.0);
	optiPos.param[0] = std::fmin(std::fmax(toeAngle_ - PhalanxI1->roty, -0.5),
			1.2);
	optiPos.Start();

	while (optiPos.IsRunning()) {
		const double ang = optiPos.param[0];

		Talus->roty = ang * (1 - mixing);
		Talus->rotz = 0;

		Calcaneus->rotz = 0;

		Cuboideum->roty = ang * mixing;
		Cuneiforme1->roty = ang * mixing;
		Cuneiforme2->roty = ang * mixing;
		Cuneiforme3->roty = ang * mixing;

		PhalanxI1->roty = -ang + toeAngle_;
		PhalanxI2->roty = -ang + toeAngle_;
		PhalanxI3->roty = -ang + toeAngle_;
		PhalanxI4->roty = -ang + toeAngle_;
		PhalanxI5->roty = -ang + toeAngle_;

		out->Update();

		if (ang > 1.2)
			optiPos.SetError(ang * 1000);
		if (ang < -0.5)
			optiPos.SetError(-ang * 1000);
		if (ang <= 1.2 && ang >= -0.5) {
			const double h = Calcaneus->GetZMin() - PhalanxI5->GetZMin();
			optiPos.SetError(fabs(h - heelHeight_ + ballHeight_));
		}
	}
	out->origin = AffineTransformMatrix::Identity();
	out->origin.TranslateLocal(0, 0, -Calcaneus->GetZMin() + heelHeight_);
	out->ModifyPosition(true);
}

