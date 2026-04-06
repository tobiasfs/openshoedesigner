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

	auto variable = std::make_shared<ParameterFormula>(variablename,
			description, formula, id, group);
	Register(variable, group);

	return variable;
}

void ParameterEvaluator::Register(std::shared_ptr<ParameterFormula> &variable,
		size_t group) {
	if (group == (size_t) -1) {
		group = currentGroup;
	} else {
		if (std::find(groupIdx.begin(), groupIdx.end(), group)
				== groupIdx.end()) {
			groupIdx.push_back(group);
			lookupGroupIdx[group] = (groupIdx.size() - 1);
		}
	}

	variable->group = group;
	parameter.push_back(variable);
	parameter.back()->base = (group == (size_t) -1);
}

std::shared_ptr<ParameterFormula> ParameterEvaluator::GetParameter(
		const size_t id, const size_t group) {
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
	const size_t NVar = parameter[n]->parser.vm.heap.size();
	parameter[n]->parser.vm.externalvariables.resize(NVar);
	const size_t neededGroup = parameter[n]->group;
	for (size_t idxNeededVar = 0; idxNeededVar < NVar; idxNeededVar++) {
		parameter[n]->parser.vm.externalvariables[idxNeededVar].reset();
		const auto &neededVar = parameter[n]->parser.vm.heap[idxNeededVar];
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
				for (size_t i = 1; i < groupIdx.size(); i++) {
					// Clone for every group >= 1
					std::shared_ptr<ParameterFormula> temp = std::make_shared<
							ParameterFormula>(*parameter[n]);
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
			parameter[n]->parser.vm.externalvariables[idxNeededVar] =
					parameter[m];
			// Rewrite the opcodes to reference an external variable instead
			// of an internal one.
			parameter[n]->parser.vm.ConvertToExternal(idxNeededVar,
					idxNeededVar);
			found = true;
			break;
		}
		if (!found) {
			skip = true;
			break;
		}
	}
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
//		param->errorFlag = false;
//		param->errorStr.clear();

		// Reset the parameter groups for the global parameters
		if (param->base)
			param->group = (size_t) (-1);
	}
}

void ParameterEvaluator::Update() {
	std::set<size_t> open;

	// Reset some flags in the the parameters
	Reset();

	// Note that the parsers are initialized in
	// the ParameterFormula::SetFormula()-function. Calling the Init()-function
	// on every Update() would be too expensive.

	// Connect externals ad determine the execution order of the formula
	// evaluations. This has to be done in one go, because global parameters
	// need to be split after descending into variants and coming back to
	// global parameters.

	for (size_t n = 0; n < parameter.size(); n++)
		open.insert(n);
	evaluationOrder.clear();

	size_t N0 = open.size();
	do {
		for (size_t n = 0; n < parameter.size(); n++) {
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
			// The size of the open list did not reduce, so either a
			// reference was not found or a loop exists, that cannot be
			// resolved.
			std::ostringstream err;

			bool hasMissingReferences = false;
			for (size_t idx : open) {
				const size_t neededGroup = parameter[idx]->group;
				for (size_t idxNeededVar = 0;
						idxNeededVar < parameter[idx]->parser.vm.heap.size();
						idxNeededVar++) {
					const auto &neededVar =
							parameter[idx]->parser.vm.heap[idxNeededVar];
					if (!neededVar.isinput)
						continue;
					auto varcmp = [neededGroup, name = neededVar.name](
							const std::shared_ptr<ParameterFormula> &p) {
						if (neededGroup != (size_t) (-1)
								&& p->group != (size_t) (-1)
								&& neededGroup != p->group)
							return false;
						return (name.compare(p->name) == 0);
					};
					const auto found = std::find_if(parameter.begin(),
							parameter.end(), varcmp);
					if (found == parameter.end()) {
						err << "The variable ";
						err << "\"" << parameter[idx]->name << "\"";
						if (parameter[idx]->group != (size_t) -1)
							err << " in group " << parameter[idx]->group;
						err << " has a reference to \"";
						err << neededVar.name << "\" which does not exist. ";
						hasMissingReferences = true;
						parameter[idx]->errorFlag = true;
						if (parameter[idx]->errorStr.empty()) {
							parameter[idx]->errorStr =
									"These variables do not exist: ";
							parameter[idx]->errorStr += neededVar.name;
						} else {
							parameter[idx]->errorStr += ", " + neededVar.name;
						}
					}
				}
			}
			if (!hasMissingReferences) {
				for (size_t idx : open) {
					err << "The formulas entered contain a reference loop ";
					err << "over ";
					bool first = true;
					if (first)
						first = false;
					else
						err << ", ";
					err << parameter[idx]->name;
				}
				err << '.';
			}
			throw std::runtime_error(err.str());
		}
		N0 = open.size();

	} while (!open.empty());
}

void ParameterEvaluator::Calculate() {
	for (size_t idx : evaluationOrder)
		parameter[idx]->Calculate();
}

void ParameterEvaluator::Clear() {
	parameter.clear();
	groupIdx.clear();
	lookupGroupIdx.clear();
	evaluationOrder.clear();
	currentGroup = (size_t) -1;
}
