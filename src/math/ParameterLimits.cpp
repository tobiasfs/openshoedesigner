///////////////////////////////////////////////////////////////////////////////
// Name               : ParameterLimits.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 02.07.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "ParameterLimits.h"

void ParameterLimits::Clear() {
	limits.clear();
}

void ParameterLimits::AddLimit(size_t index, double min, double max, double a,
		double b) {
	Limit temp;
	temp.index = index;
	temp.minval = min;
	temp.maxval = max;
	temp.a = a;
	temp.b = b;
	limits.push_back(temp);
}

void ParameterLimits::AddLimitMin(size_t index, double min, double a,
		double b) {
	Limit temp;
	temp.index = index;
	temp.minval = min;
	temp.a = a;
	temp.b = b;
	limits.push_back(temp);
}

void ParameterLimits::AddLimitMax(size_t index, double max, double a,
		double b) {
	Limit temp;
	temp.index = index;
	temp.maxval = max;
	temp.a = a;
	temp.b = b;
	limits.push_back(temp);
}

double ParameterLimits::Evaluate(const std::vector<double> &param) const {
	double temp = 0.0;
	for (size_t n = 0; n < limits.size(); n++) {
		if (limits[n].index >= param.size())
			continue;
		if (param[limits[n].index] > limits[n].maxval) {
			temp += (param[limits[n].index] - limits[n].maxval) * limits[n].a
					+ limits[n].b;
		}
		if (param[limits[n].index] < limits[n].minval) {
			temp += (param[limits[n].minval - limits[n].index]) * limits[n].a
					+ limits[n].b;
		}
	}
	return temp;
}
