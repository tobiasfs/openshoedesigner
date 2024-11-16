///////////////////////////////////////////////////////////////////////////////
// Name               : LinearOptimizer.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 13.09.2020
// Copyright          : (C) 2018 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "LinearOptimizer.h"

#include "../math/Polynomial.h"

#include <cfloat>
#include <cmath>
#include <stdexcept>

void LinearOptimizer::SetFirstTest(double xFirst) {
	state = 1;
	x0 = xFirst;
}

void LinearOptimizer::Start() {
	if (state == 0) {
		x0 = param[0];
		x1 = x0 + spread;
	} else {
		x1 = x0;
		x0 = param[0];
	}
	state = 2;
	y0 = DBL_MAX;
	y1 = DBL_MAX;
	evaluationCount = 0;
}

bool LinearOptimizer::IsRunning() {
	if (y0 < errorLimit || evaluationCount >= evalLimit) {
		state = 0;
		return false;
	}
	switch (state) {
	case 0:
	case 1:
	case 3:
	case 5: {
		throw(std::logic_error(
				"LinearOptimizer::IsRunning - Start() was not called."));
	}
	case 2: {
		param[0] = x1;
		state = 3;
		return true;
	}
	case 4: {
		param[0] = x0;
		state = 5;
		return true;
	}
	case 6: {
		Polynomial p = Polynomial::ByValue(x0, y0, x1, y1);
		param[0] = p.FindZero((x0 + x1) / 2.0);
		state = 7;
		return true;
	}
	case 10: {
		return false;
	}
	default:
		throw(std::logic_error(
				"LinearOptimizer::IsRunning - State machine reached invalid state."));
	}
	return false;
}

void LinearOptimizer::SetError(double error) {
	evaluationCount++;
	lastError = error;
	switch (state) {
	case 3:
		y1 = error;
		y0 = error;
		state = 4;
		break;
	case 5:
		y0 = error;
		state = 6;
		break;
	case 7:
		if (fabs(param[0] - x0) < fabs(param[0] - x1)) {
			x1 = param[0];
			y1 = error;
		} else {
			x0 = param[0];
			y0 = error;
		}
		state = 6;
		break;
	default:
		throw(std::logic_error(
				"LinearOptimizer::SetError was called outside the main loop."));
	}
}

void LinearOptimizer::Stop() {
	state = 10;
}

