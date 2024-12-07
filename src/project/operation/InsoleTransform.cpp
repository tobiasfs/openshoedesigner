///////////////////////////////////////////////////////////////////////////////
// Name               : InsoleTransform.cpp
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
#include "InsoleTransform.h"

#include "../../3D/Bender.h"
#include "../../3D/Vector3.h"

#include <sstream>
#include <stdexcept>

bool InsoleTransform::CanRun() {
	std::ostringstream err;
	err << __FILE__ << ":" << __LINE__ << ":" << __func__ << " -";
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
	if (hasMissingConnection)
		throw std::runtime_error(err.str());
	return true;
}

bool InsoleTransform::Propagate() {
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

bool InsoleTransform::HasToRun() {
	if (!CanRun())
		return false;
	return in->IsValid() && !out->IsValid() && out->IsNeeded();
}

void InsoleTransform::Run() {
	*out = *in;

	Shape();

	out->MarkValid(true);
	out->MarkNeeded(false);
}

void InsoleTransform::Shape() {
	// Calculate gamma0 and gamma1
	const double alpha0 = heelPitch->ToDouble();
	const double alpha1 = toeSpring->ToDouble();
	const double s0 = out->F.p.x - out->J.p.x;
	const double s1 = out->Z.p.x - out->F.p.x;
	const double dt = heelHeight->ToDouble() - ballHeight->ToDouble()
			+ legLengthDifference->ToDouble();

	const size_t NSample = 100;
	Polynomial gammasample = Polynomial::ByValue(0, 0, 100,
	M_PI * 100 / 180 - alpha0);
	double optg0 = 0;
	double optg1 = -1;
	double optd0 = 0;
	double optd1 = -1;
	for (size_t n = 0; n < NSample; ++n) {
		const double gamma0 = gammasample(n);
		const double gamma1 = alpha0 + alpha1 + gamma0;
		double d0 = 0.0;
		if (fabs(gamma0) < 1e-9) {
			d0 = s0 * sin(alpha0);
		} else {
			d0 = s0 * (cos(alpha0) - cos(gamma0 + alpha0)) / gamma0;
		}
		double d1 = 0.0;
		if (fabs(gamma1) < 1e-9) {
			d1 = s1 * sin(-alpha1);
		} else {
			d1 = s1 * (cos(alpha1) - cos(gamma0 + alpha0)) / gamma1;
		}
		const double d = d0 + d1;
		optg1 = gamma0;
		optd1 = d;
		if (d > dt || n + 1 == NSample) {
			break;
		}
		optg0 = gamma0;
		optd0 = d;
	}
	Polynomial opt = Polynomial::ByValue(optg0, optd0, optg1, optd1) - dt;
	const double gamma0 = opt.FindZero((optg0 + optg1) / 2);
	const double gamma1 = alpha0 + alpha1 + gamma0;

	// Bend toes
	{
		Bender b;
		b.from0.SetOrigin(Vector3(out->F.p.x, 0, out->F.p.z));
		b.from1.SetOrigin(Vector3(out->Z.p.x, 0, out->Z.p.z));
		b.to0 = b.from0;
		b.to1 = b.to0;
		b.to1 *= AffineTransformMatrix::RotationXYZ(0, -gamma1 / 2, 0);
		double L = s1;
		if (fabs(gamma1) > 1e-9) {
			L = 2 * s1 / gamma1 * sin(gamma1 / 2.0);
		}
		b.to1.TranslateLocal(L, 0, 0);
		b.to1 *= AffineTransformMatrix::RotationXYZ(0, -gamma1 / 2, 0);
		b.Calculate();
		out->Transform(b);
	}

	// Bend heel and mid-section
	{
		Bender b;
		b.from0.SetOrigin(Vector3(out->J.p.x, 0, out->J.p.z));
		b.from1.SetOrigin(Vector3(out->F.p.x, 0, out->F.p.z));
		b.to0 = b.from0;
		b.to0 *= AffineTransformMatrix::RotationXYZ(0, alpha0, 0);
		b.to1 = b.to0;
		b.to1 *= AffineTransformMatrix::RotationXYZ(0, gamma0 / 2, 0);
		double L = s1;
		if (fabs(gamma0) > 1e-9) {
			L = 2 * s0 / gamma0 * sin(gamma0 / 2.0);
		}
		b.to1.TranslateLocal(L, 0, 0);
		b.to1 *= AffineTransformMatrix::RotationXYZ(0, gamma0 / 2, 0);
		b.Calculate();
		out->Transform(b);
	}

	// Set heel/platform height
	{
		AffineTransformMatrix m;
		m.TranslateGlobal(0, 0,
				heelHeight->ToDouble() + legLengthDifference->ToDouble());
		out->Transform(m);
	}
}

