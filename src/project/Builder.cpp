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

#include <iostream>
#include <sstream>
#include <stdexcept>

bool Builder::IsSetup() const {
	return !operations.empty();
}

void Builder::Setup(Project &project) {
	if (operations.empty()) {

		auto &config = project.config;
		auto &footL = project.footL;
		auto &footR = project.footR;

		if (!opCoordinateSystemConstruct) {
			opCoordinateSystemConstruct = std::make_shared<
					CoordinateSystemConstruct>();
			operations.push_back(opCoordinateSystemConstruct);
		}
		if (!opFootModelLoad) {
			opFootModelLoad = std::make_shared<FootModelLoad>();
			opFootModelLoad->filename = config.filenameBoneModel;
			operations.push_back(opFootModelLoad);
		}
		if (!opFootModelUpdate) {
			opFootModelUpdate = std::make_shared<FootModelUpdate>();
			opFootModelUpdate->heelPitch = config.heelPitch;
			opFootModelUpdate->toeSpring = config.toeSpring;
			opFootModelUpdate->heelHeight = config.heelHeight;
			opFootModelUpdate->ballHeight = config.ballHeight;
			opFootModelUpdate->legLengthDifference = footL.legLengthDifference;
			operations.push_back(opFootModelUpdate);
		}
		if (!opFootScanLoad) {
			opFootScanLoad = std::make_shared<FootScanLoad>();
			opFootScanLoad->filename = config.filenameScan;
			operations.push_back(opFootScanLoad);
		}
		if (!opHeelExtractInsole) {
			opHeelExtractInsole = std::make_shared<HeelExtractInsole>();
			opHeelExtractInsole->footLength = footL.footLength;
			opHeelExtractInsole->ballMeasurementAngle =
					config.ballMeasurementAngle;
			opHeelExtractInsole->heelDirectionAngle = config.heelDirectionAngle;
			opHeelExtractInsole->littleToeAngle = config.littleToeAngle;
			opHeelExtractInsole->bigToeAngle = config.bigToeAngle;
			opHeelExtractInsole->extraLength = config.extraLength;
			operations.push_back(opHeelExtractInsole);
		}
		if (!opHeelLoad) {
			opHeelLoad = std::make_shared<ObjectLoad>();
			opHeelLoad->filename = config.filenameHeel;
			operations.push_back(opHeelLoad);
		}
		if (!opHeelNormalize) {
			opHeelNormalize = std::make_shared<HeelNormalize>();
			opHeelNormalize->heelReorient = config.heelReorient;
			operations.push_back(opHeelNormalize);
		}
		if (!opInsoleConstruct) {
			opInsoleConstruct = std::make_shared<InsoleConstruct>();
			opInsoleConstruct->footLength = footL.footLength;
			opInsoleConstruct->ballMeasurementAngle =
					config.ballMeasurementAngle;
			opInsoleConstruct->heelDirectionAngle = config.heelDirectionAngle;
			opInsoleConstruct->littleToeAngle = config.littleToeAngle;
			opInsoleConstruct->bigToeAngle = config.bigToeAngle;
			opInsoleConstruct->ballWidth = footL.ballWidth;
			opInsoleConstruct->heelWidth = footL.heelWidth;
			opInsoleConstruct->extraLength = config.extraLength;
			operations.push_back(opInsoleConstruct);
		}
		if (!opInsoleFlatten) {
			opInsoleFlatten = std::make_shared<InsoleFlatten>();
			operations.push_back(opInsoleFlatten);
		}
		if (!opInsoleTransform) {
			opInsoleTransform = std::make_shared<InsoleTransform>();
			opInsoleTransform->heelPitch = config.heelPitch;
			opInsoleTransform->toeSpring = config.toeSpring;
			opInsoleTransform->heelHeight = config.heelHeight;
			opInsoleTransform->ballHeight = config.ballHeight;
			opInsoleTransform->legLengthDifference = footL.legLengthDifference;
			operations.push_back(opInsoleTransform);
		}
		if (!opLastAnalyse) {
			opLastAnalyse = std::make_shared<LastAnalyse>();
			opLastAnalyse->lastReorient = config.lastReorient;
			operations.push_back(opLastAnalyse);
		}
		if (!opLastLoad) {
			opLastLoad = std::make_shared<ObjectLoad>();
			opLastLoad->filename = config.filenameLast;
			operations.push_back(opLastLoad);
		}
		if (!opLastNormalize) {
			opLastNormalize = std::make_shared<LastNormalize>();
			opLastNormalize->lastReorient = config.lastReorient;
			operations.push_back(opLastNormalize);
		}
		if (!opLastUpdate) {
			opLastUpdate = std::make_shared<LastUpdate>();
			opLastUpdate->lastModify = config.lastModify;
			opLastUpdate->heelPitch = config.heelPitch;
			opLastUpdate->toeSpring = config.toeSpring;
			opLastUpdate->heelHeight = config.heelHeight;
			opLastUpdate->ballHeight = config.ballHeight;
			opLastUpdate->legLengthDifference = footL.legLengthDifference;
			operations.push_back(opLastUpdate);
		}
	}
	Connect(project);
}

void Builder::Connect(Project &project) {
	auto &config = project.config;

	const bool symmetric = (project.footL == project.footR);

	if (config.heelConstructionType->IsSelection("construct")) {
		opInsoleTransform->in = opInsoleConstruct->out;
		project.insoleFlatL = opInsoleConstruct->out;
		project.insoleL = opInsoleTransform->out;
		project.insoleFlatR = project.insoleFlatL;
		project.insoleR = project.insoleL;
	}

	if (config.heelConstructionType->IsSelection("loadFromFile")) {
		opHeelNormalize->in = opHeelLoad->out;
		project.heelL = opHeelNormalize->out;
		opHeelExtractInsole->in = opHeelNormalize->out;
		project.insoleL = opHeelExtractInsole->out;
		opInsoleFlatten->in = project.insoleL;
		project.insoleFlatL = opInsoleFlatten->out;

		project.heelR = project.heelL;
		project.insoleFlatR = project.insoleFlatL;
		project.insoleR = project.insoleL;
	}

	if (config.lastConstructionType->IsSelection("construct")) {

	}

	if (config.lastConstructionType->IsSelection("boneBased")) {
		opFootModelUpdate->in = opFootModelLoad->out;

	}
	if (config.lastConstructionType->IsSelection("loadFromFile")) {
		opLastNormalize->in = opLastLoad->out;
		opLastAnalyse->in = opLastNormalize->out;
		project.lastR = opLastAnalyse->out;
		project.lastL = opLastAnalyse->out;
	}

	opCoordinateSystemConstruct->in = project.insoleL;
	project.csL = opCoordinateSystemConstruct->out;
	project.csR = project.csL;
}

void Builder::Update(Project &project) {
	error.clear();

	Setup(project);

	bool propagation_complete = false;
	while (!propagation_complete) {
		propagation_complete = true;
		for (const auto &op : operations)
			propagation_complete &= !op->Propagate();
	}

	bool setup_complete = true;
	std::ostringstream err;
	for (const auto &op : operations) {
		if (op->HasToRun())
			setup_complete &= op->CanRun();
		if (!op->error.empty()) {
			err << op->GetName() << ": " << op->error << "\n";
		}
	}
	if (!setup_complete) {
		err << __FILE__ << ":Setup - Error in setup routine.";
		error = err.str();
		return;
	}

	// Single threaded execution for debugging
	DEBUGOUT << "----- Updating -----\n";
	bool operations_complete = false;
	while (!operations_complete) {
		operations_complete = true;
		for (auto &op : operations)
			if (op->HasToRun() && op->CanRun()) {
				DEBUGOUT << "-> running: " << op->GetName() << "\n";
				op->Run();
				operations_complete = false;
			}
	}
	DEBUGOUT << "----- done -----\n";
}

void Builder::Paint() const {
#ifdef DEBUG
	for (auto &op : operations)
		op->Paint();
#endif
}
