///////////////////////////////////////////////////////////////////////////////
// Name               : Builder.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 17.11.2024
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
#include "Builder.h"

#include "Project.h"
#include "Configuration.h"

#include "operation/LastLoad.h"
#include "operation/LastNormalize.h"
#include "operation/LastAnalyse.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

void Builder::Setup(Project &project) {
	auto &config = project.config;
	auto &footL = project.footL;
	auto &footR = project.footR;

	operations.clear();

	LastLoad opLoad;
	opLoad.filename = config.lastFilename;
	operations.push_back(std::make_shared<LastLoad>(opLoad));

	LastNormalize opNormalize;
	opNormalize.in = opLoad.out;
	operations.push_back(std::make_shared<LastNormalize>(opNormalize));

	LastAnalyse opAnalyse;
	opAnalyse.in = opNormalize.out;
	operations.push_back(std::make_shared<LastAnalyse>(opAnalyse));

	project.lastRaw = opNormalize.out;
	project.lastNormalized = opAnalyse.out;

}

void Builder::Update(Project &project) {
	bool setup_modified = operations.empty();

	if (setup_modified)
		Setup(project);

	bool setup_complete = true;
	for (const auto &op : operations)
		setup_complete &= op->CanRun();
	if (!setup_complete) {
		std::ostringstream err;
		err << __FILE__ << ":Setup - Error in setup routine.";
		throw std::runtime_error(err.str());
	}

	bool propagation_complete = false;
	while (!propagation_complete) {
		propagation_complete = true;
		for (const auto &op : operations)
			propagation_complete &= !op->Propagate();
	}

	// Single threaded execution for debugging

	bool operations_complete = false;
	while (!operations_complete) {
		operations_complete = true;
		for (auto &op : operations)
			if (op->CanRun() && op->HasToRun()) {
				op->Run();
				operations_complete = false;
			}
	}
}

void Builder::Paint() const {
#ifdef DEBUG
	for (auto &op : operations)
		op->Paint();
#endif
}
