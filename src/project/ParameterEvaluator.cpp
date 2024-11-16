///////////////////////////////////////////////////////////////////////////////
// Name               : ParameterEvaluator.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 02.12.2022
// Copyright          : (C) 2022 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "ParameterEvaluator.h"

#include "Parameter.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

ParameterEvaluator::ParameterEvaluator() {
//	maxGroupIdx = (size_t) -1;
}

void ParameterEvaluator::SetGroup(const size_t group) {
	currentGroup = group;
	if (group == (size_t) -1)
		return;
	if (std::find(groupIdx.begin(), groupIdx.end(), group) == groupIdx.end()) {
		groupIdx.push_back(group);
		lookupGroupIdx[group] = (groupIdx.size() - 1);
	}
}

std::shared_ptr<ParameterFormula> ParameterEvaluator::Register(
		const std::string &variablename, const std::string &description,
		const std::string &formula, size_t id, size_t group) {

	if (group == (size_t) -1) {
		group = currentGroup;
	} else {
		if (std::find(groupIdx.begin(), groupIdx.end(), group)
				== groupIdx.end()) {
			groupIdx.push_back(group);
			lookupGroupIdx[group] = (groupIdx.size() - 1);
		}
	}
	auto var = std::make_shared<ParameterFormula>(variablename, description, formula,
			id, group);
	parameter.push_back(var);

//	const size_t idx = parameter.size();
//	lookup[variablename] = idx;
//	lookupID[id] = idx;
//	parameterNames.insert(variablename);

	return var;
}

std::shared_ptr<ParameterFormula> ParameterEvaluator::GetParameter(const size_t id,
		const size_t group) {
	for (std::shared_ptr<ParameterFormula> &param : parameter) {
		if (param->id != id)
			continue;
		if (param->base || param->group == group)
			return param;
	}
	if (group == (size_t) -1)
		throw(new std::runtime_error(
				std::string("ParameterFormula with id = ") + std::to_string(id)
						+ " not found."));
	throw(new std::runtime_error(
			std::string("ParameterFormula with id = ") + std::to_string(id)
					+ " and group = " + std::to_string(group) + " not found."));
}

bool ParameterEvaluator::HasID(const size_t id, const size_t group) const {
	for (const auto &param : parameter)
		if (param->id == id && (param->base || param->group == group))
			return true;
	return false;
}

bool ParameterEvaluator::ConnectExternal(size_t n, std::set<size_t> &open) {

	bool skip = false;
	const size_t NVar = parameter[n]->parser.variables.size();
	parameter[n]->parser.externalvariables.resize(NVar);
	const size_t neededGroup = parameter[n]->group;
	for (size_t idxNeededVar = 0; idxNeededVar < NVar; ++idxNeededVar) {
		parameter[n]->parser.externalvariables[idxNeededVar].reset();
		const auto &neededVar = parameter[n]->parser.variables[idxNeededVar];
		if (!neededVar.isinput)
			continue;

		bool found = false;
		for (const auto m : evaluationOrder) {
			if (neededGroup != (size_t) (-1)
					&& parameter[m]->group != (size_t) (-1)
					&& neededGroup != parameter[m]->group)
				continue;

			if (neededVar.name.compare(parameter[m]->name) != 0)
				continue;

			if (neededGroup == (size_t) (-1)
					&& parameter[m]->group != (size_t) (-1)) {
				// This is the special case, where parameter splitting
				// has to happen: a global parameter is using a
				// parameter from a variant. In this case the global
				// parameter has to be split into two by cloning.
				parameter[n]->group = groupIdx[0]; // Modify the original
				if (groupIdx.size() > 1) {
					parameter[n]->unstable = true;
				}
				for (size_t i = 1; i < groupIdx.size(); ++i) {
					// Clone for every group >= 1
					std::shared_ptr<ParameterFormula> temp =
							std::make_shared<ParameterFormula>(*parameter[n]);
					temp->group = groupIdx[i];
					temp->extra = true;
					parameter.push_back(temp);
					open.insert(parameter.size() - 1);
				}
				if (parameter[m]->group != parameter[n]->group)
					continue;
			}
			// The parameter to be referenced was found. It is connected
			// via a std::weak_ptr.
			parameter[n]->parser.externalvariables[idxNeededVar] = parameter[m];
			for (auto &op : parameter[n]->parser.instructions) {
				if (op.idx != idxNeededVar)
					continue;
				if (op.opcode == MathParser::OpCode::FETCH_I) {
					op.opcode = MathParser::OpCode::FETCH_EXT_I;
				}
				if (op.opcode == MathParser::OpCode::STORE_I) {
					op.opcode = MathParser::OpCode::STORE_EXT_I;
				}
			}
			found = true;
			break;
		}
		if (!found) {
			skip = true;
			break;
		}
	}

	//				std::string name = var.name;
	//			if (!var.isinput)
	//				throw std::runtime_error(
	//						"The variable " + name
	//								+ " should be an input to the parameter "
	//								+ param->name + ".");
	//			if (!var.isoutput)
	//				throw std::runtime_error(
	//						"The variable " + name
	//								+ " should be set in the parameter "
	//								+ param->name + ".");
	//
	//			auto res = parameter.end();
	//			for (auto &x : parameter) {
	//				if (x->name.compare(name) == 0) {
	////					res = *x;
	//					break;
	//				}
	//			}
	//
	//			if (res == parameter.end())
	//				throw std::runtime_error(
	//						"The variable " + name
	//								+ " was not found. It is needed to calculate the parameter "
	//								+ param->name + ".");
	//
	//

	return skip;
}

void ParameterEvaluator::Reset() {

	// Remove all additionally created ParameterFormula in one go using the
	// Erase-Remove-idiom (https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom)
	parameter.erase(
			std::remove_if(parameter.begin(), parameter.end(),
					[](const std::shared_ptr<ParameterFormula> &x) {
						return x->extra;
					}), parameter.end());

	// Reset some flags in the the parameters
	for (auto &param : parameter) {
		param->unstable = false;
		// Reset the parameter groups for the global parameters
		if (param->base)
			param->group = (size_t) (-1);
	}
}

void ParameterEvaluator::Update() {

	// Reset some flags in the the parameters
	Reset();

	// Initialize all parser
	// TODO Remove if not necessary (check IsModified of parameter?)
	//	for (auto &param : parameter)
	//		param->Init();

	// Connect externals ad determine the execution order of the formula
	// evaluations. This has to be done in one go, because global parameters
	// need to be split after descending into variants and coming back to
	// global parameters.

	std::set<size_t> open;
	for (size_t n = 0; n < parameter.size(); ++n)
		open.insert(n);
	evaluationOrder.clear();

	size_t N0 = open.size();
	do {
		for (size_t n = 0; n < parameter.size(); ++n) {
			//TODO Change the check into iterator magic.
			if (open.find(n) == open.end())
				continue;
			bool skip = ConnectExternal(n, open);
			if (skip)
				continue;
			evaluationOrder.push_back(n);
			open.erase(n);
		}

		if (N0 == open.size()) {
			std::ostringstream err;
			err << "The formulas entered contain a reference loop over ";
			bool first = true;
			for (size_t idx : open) {
				if (first)
					first = false;
				else
					err << ", ";
				err << parameter[idx]->name;
			}
			err << '.';
			throw std::runtime_error(err.str());
		}
		N0 = open.size();

	} while (!open.empty());
}

void ParameterEvaluator::Calculate() {
	for (size_t idx : evaluationOrder)
		parameter[idx]->Calculate();
}

