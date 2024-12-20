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

bool FootModelUpdate::CanRun() {
	std::ostringstream err;
	err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " -";
	bool hasMissingConnection = false;

	if (!config) {
		hasMissingConnection = true;
		err << " Input \"config\" not connected.";
	}
	if (!heelPitch) {
		hasMissingConnection = true;
		err << " Input \"heelPitch\" not connected.";
	}
	if (!toeSpring) {
		hasMissingConnection = true;
		err << " Input \"toeSpring\" not connected.";
	}
	if (!heelHeight) {
		hasMissingConnection = true;
		err << " Input \"heelHeight\" not connected.";
	}
	if (!ballHeight) {
		hasMissingConnection = true;
		err << " Input \"ballHeight\" not connected.";
	}
	if (!legLengthDifference) {
		hasMissingConnection = true;
		err << " Input \"legLengthDifference\" not connected.";
	}

	if (!in) {
		hasMissingConnection = true;
		err << " Input \"in\" not connected.";
	}
	if (!out) {
		hasMissingConnection = true;
		err << " Output \"out\" not set.";
	}
	if (hasMissingConnection){
		error = err.str();
		throw std::logic_error(err.str());
	}
	error.clear();
	return true;
}

bool FootModelUpdate::Propagate() {
	bool modify = false;
	if (!CanRun())
		return modify;

	bool modified = false;
	modified |= config->IsModified();
	modified |= heelPitch->IsModified();
	modified |= toeSpring->IsModified();
	modified |= heelHeight->IsModified();
	modified |= ballHeight->IsModified();
	modified |= legLengthDifference->IsModified();

	if (!in->IsValid() || modified) {
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
	if (!CanRun())
		return false;
	return in->IsValid() && !out->IsValid() && out->IsNeeded();
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

std::string FootModelUpdate::GetName() const {
	return "FootModelUpdate";
}
