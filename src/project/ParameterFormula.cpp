///////////////////////////////////////////////////////////////////////////////
// Name               : ParameterFormula.cpp
// Purpose            : Store a parameter with a formula
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 09.11.2024
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
#include "ParameterFormula.h"

#include <cfloat>
#include <cmath>

ParameterFormula::ParameterFormula(const std::string &name,
		const std::string &description, const std::string &formula,
		const size_t id, const size_t group) :
		Parameter(name, description, id, group), formula(formula) {

	// If a parameter belongs to the global group, it is considered a base
	// variable. This is never changed afterwards, but the group might change
	// later on.
	base = (group == (size_t) -1);

	Init();
}

void ParameterFormula::SetFormula(const std::string &newFormula) {
	if (formula.compare(newFormula) == 0)
		return;
	modified = true;
	formula = newFormula;
	Init();
}

std::string ParameterFormula::GetFormula() const {
	return formula;
}

void ParameterFormula::Init() {
	parser.ParseExpression(formula);
}

double ParameterFormula::Calculate() {
	const double oldValue = value;
	const Unit oldUnit = unit;
	parser.InitMachine();
	try {
		parser.Run();
		errorFlag = false;
		errorStr.clear();
		value = parser.stack.back().ToDouble();
		unit = parser.stack.back().GetUnit();
		modified |= (std::fabs(value - oldValue) > FLT_EPSILON)
				|| (unit != oldUnit);
	} catch (const std::exception &ex) {
		errorFlag = true;
		errorStr = ex.what();
		modified = true;
	}
	return this->value;
}
