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

#include "../system/StopWatch.h"
#include <iostream>
#include <fstream>
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
			opCoordinateSystemConstruct->belowCrotchGirth =
					footL.belowCrotchGirth;
			opCoordinateSystemConstruct->belowCrotchLevel =
					footL.belowCrotchLevel;
			opCoordinateSystemConstruct->middleOfThighGirth =
					footL.middleOfThighGirth;
			opCoordinateSystemConstruct->middleOfThighLevel =
					footL.middleOfThighLevel;
			opCoordinateSystemConstruct->aboveKneeGirth = footL.aboveKneeGirth;
			opCoordinateSystemConstruct->aboveKneeLevel = footL.aboveKneeLevel;
			opCoordinateSystemConstruct->overKneeCapGirth =
					footL.overKneeCapGirth;
			opCoordinateSystemConstruct->overKneeCapLevel =
					footL.overKneeCapLevel;
			opCoordinateSystemConstruct->belowKneeGirth = footL.belowKneeGirth;
			opCoordinateSystemConstruct->belowKneeLevel = footL.belowKneeLevel;
			opCoordinateSystemConstruct->middleOfShankGirth =
					footL.middleOfShankGirth;
			opCoordinateSystemConstruct->middleOfShankLevel =
					footL.middleOfShankLevel;
			opCoordinateSystemConstruct->aboveAnkleGirth =
					footL.aboveAnkleGirth;
			opCoordinateSystemConstruct->aboveAnkleLevel =
					footL.aboveAnkleLevel;
			opCoordinateSystemConstruct->overAnkleBoneGirth =
					footL.overAnkleBoneGirth;
			opCoordinateSystemConstruct->overAnkleBoneLevel =
					footL.overAnkleBoneLevel;
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
		if (!opHeelCenter) {
			opHeelCenter = std::make_shared<HeelCenter>();
			opHeelCenter->overAnkleBoneLevel = footL.overAnkleBoneLevel;
			operations.push_back(opHeelCenter);
		}
		if (!opHeelConstruct) {
			opHeelConstruct = std::make_shared<HeelConstruct>();
			opHeelConstruct->heelCode = config.heelCode;
			operations.push_back(opHeelConstruct);
		}
		if (!opHeelExtractInsole) {
			opHeelExtractInsole = std::make_shared<HeelExtractInsole>();
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
		if (!opInsoleAnalyze) {
			opInsoleAnalyze = std::make_shared<InsoleAnalyze>();
			opInsoleAnalyze->footLength = footL.footLength;
			opInsoleAnalyze->ballMeasurementAngle = config.ballMeasurementAngle;
			opInsoleAnalyze->heelDirectionAngle = config.heelDirectionAngle;
			opInsoleAnalyze->littleToeAngle = config.littleToeAngle;
			opInsoleAnalyze->bigToeAngle = config.bigToeAngle;
			opInsoleAnalyze->extraLength = config.extraLength;
			operations.push_back(opInsoleAnalyze);
		}
		if (!opInsoleConstruct) {
			opInsoleConstruct = std::make_shared<InsoleConstruct>();
			opInsoleConstruct->footLength = footL.footLength;
			opInsoleConstruct->ballWidth = footL.ballWidth;
			opInsoleConstruct->heelWidth = footL.heelWidth;
			opInsoleConstruct->ballMeasurementAngle =
					config.ballMeasurementAngle;
			opInsoleConstruct->heelDirectionAngle = config.heelDirectionAngle;
			opInsoleConstruct->littleToeAngle = config.littleToeAngle;
			opInsoleConstruct->bigToeAngle = config.bigToeAngle;
			opInsoleConstruct->tipSharpness = config.tipSharpness;
			opInsoleConstruct->extraLength = config.extraLength;
			operations.push_back(opInsoleConstruct);
		}
		if (!opInsoleFlatten) {
			opInsoleFlatten = std::make_shared<InsoleFlatten>();
			opInsoleFlatten->debugMIDI_48 = config.debugMIDI_48;
			opInsoleFlatten->debugMIDI_49 = config.debugMIDI_49;
			opInsoleFlatten->debugMIDI_50 = config.debugMIDI_50;
			opInsoleFlatten->debugMIDI_51 = config.debugMIDI_51;
			opInsoleFlatten->debugMIDI_52 = config.debugMIDI_52;
			opInsoleFlatten->debugMIDI_53 = config.debugMIDI_53;
			opInsoleFlatten->debugMIDI_54 = config.debugMIDI_54;
			opInsoleFlatten->debugMIDI_55 = config.debugMIDI_55;
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
		if (!opLastConstruct) {
			opLastConstruct = std::make_shared<LastConstruct>();
			opLastConstruct->upperLevel = config.upperLevel;
			operations.push_back(opLastConstruct);
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
		if (!opUpperConstruct) {
			opUpperConstruct = std::make_shared<UpperConstruct>();
			operations.push_back(opUpperConstruct);
		}
		if (!opUpperFlatten) {
			opUpperFlatten = std::make_shared<UpperFlatten>();
			operations.push_back(opUpperFlatten);
		}
	}
	Connect(project);
}

#ifdef DEBUG
void Builder::ToDot(std::ostream &out, const Project &project) const {
	out << "digraph{\n";
	out << "rankdir=TB;\n";
	out
			<< "opCoordinateSystemConstruct [shape=record,label=\"{ <i1> in }| opCoordinateSystemConstruct | { <o1> out }\"];\n";
	out << "\"" << opCoordinateSystemConstruct->in
			<< "\" -> opCoordinateSystemConstruct:i1;\n";
	out << "opCoordinateSystemConstruct:o1 -> \""
			<< opCoordinateSystemConstruct->out << "\";\n";
	out
			<< "opFootModelLoad [shape=record,label=\"{  }| opFootModelLoad | { <o1> out }\"];\n";
	out << "opFootModelLoad:o1 -> \"" << opFootModelLoad->out << "\";\n";
	out
			<< "opFootModelUpdate [shape=record,label=\"{ <i1> in }| opFootModelUpdate | { <o1> out }\"];\n";
	out << "\"" << opFootModelUpdate->in << "\" -> opFootModelUpdate:i1;\n";
	out << "opFootModelUpdate:o1 -> \"" << opFootModelUpdate->out << "\";\n";
	out
			<< "opFootScanLoad [shape=record,label=\"{  }| opFootScanLoad | { <o1> out }\"];\n";
	out << "opFootScanLoad:o1 -> \"" << opFootScanLoad->out << "\";\n";
	out
			<< "opHeelCenter [shape=record,label=\"{ <i1> heel_in | <i2> insole_in }| opHeelCenter | { <o1> heel_out | <o2> insole_out }\"];\n";
	out << "\"" << opHeelCenter->heel_in << "\" -> opHeelCenter:i1;\n";
	out << "\"" << opHeelCenter->insole_in << "\" -> opHeelCenter:i2;\n";
	out << "opHeelCenter:o1 -> \"" << opHeelCenter->heel_out << "\";\n";
	out << "opHeelCenter:o2 -> \"" << opHeelCenter->insole_out << "\";\n";
	out
			<< "opHeelConstruct [shape=record,label=\"{ <i1> in }| opHeelConstruct | { <o1> out }\"];\n";
	out << "\"" << opHeelConstruct->in << "\" -> opHeelConstruct:i1;\n";
	out << "opHeelConstruct:o1 -> \"" << opHeelConstruct->out << "\";\n";
	out
			<< "opHeelExtractInsole [shape=record,label=\"{ <i1> in }| opHeelExtractInsole | { <o1> out }\"];\n";
	out << "\"" << opHeelExtractInsole->in << "\" -> opHeelExtractInsole:i1;\n";
	out << "opHeelExtractInsole:o1 -> \"" << opHeelExtractInsole->out
			<< "\";\n";
	out
			<< "opHeelLoad [shape=record,label=\"{  }| opHeelLoad | { <o1> out }\"];\n";
	out << "opHeelLoad:o1 -> \"" << opHeelLoad->out << "\";\n";
	out
			<< "opHeelNormalize [shape=record,label=\"{ <i1> in }| opHeelNormalize | { <o1> out }\"];\n";
	out << "\"" << opHeelNormalize->in << "\" -> opHeelNormalize:i1;\n";
	out << "opHeelNormalize:o1 -> \"" << opHeelNormalize->out << "\";\n";
	out
			<< "opInsoleAnalyze [shape=record,label=\"{ <i1> insole_in | <i2> insoleFlat_in }| opInsoleAnalyze | { <o1> insole_out | <o2> insoleFlat_out }\"];\n";
	out << "\"" << opInsoleAnalyze->insole_in << "\" -> opInsoleAnalyze:i1;\n";
	out << "\"" << opInsoleAnalyze->insoleFlat_in
			<< "\" -> opInsoleAnalyze:i2;\n";
	out << "opInsoleAnalyze:o1 -> \"" << opInsoleAnalyze->insole_out << "\";\n";
	out << "opInsoleAnalyze:o2 -> \"" << opInsoleAnalyze->insoleFlat_out
			<< "\";\n";
	out
			<< "opInsoleConstruct [shape=record,label=\"{  }| opInsoleConstruct | { <o1> out }\"];\n";
	out << "opInsoleConstruct:o1 -> \"" << opInsoleConstruct->out << "\";\n";
	out
			<< "opInsoleFlatten [shape=record,label=\"{ <i1> in }| opInsoleFlatten | { <o1> out }\"];\n";
	out << "\"" << opInsoleFlatten->in << "\" -> opInsoleFlatten:i1;\n";
	out << "opInsoleFlatten:o1 -> \"" << opInsoleFlatten->out << "\";\n";
	out
			<< "opInsoleTransform [shape=record,label=\"{ <i1> in }| opInsoleTransform | { <o1> out }\"];\n";
	out << "\"" << opInsoleTransform->in << "\" -> opInsoleTransform:i1;\n";
	out << "opInsoleTransform:o1 -> \"" << opInsoleTransform->out << "\";\n";
	out
			<< "opLastAnalyse [shape=record,label=\"{ <i1> in }| opLastAnalyse | { <o1> out }\"];\n";
	out << "\"" << opLastAnalyse->in << "\" -> opLastAnalyse:i1;\n";
	out << "opLastAnalyse:o1 -> \"" << opLastAnalyse->out << "\";\n";
	out
			<< "opLastConstruct [shape=record,label=\"{ <i1> insole | <i2> cs }| opLastConstruct | { <o1> out }\"];\n";
	out << "\"" << opLastConstruct->insole << "\" -> opLastConstruct:i1;\n";
	out << "\"" << opLastConstruct->cs << "\" -> opLastConstruct:i2;\n";
	out << "opLastConstruct:o1 -> \"" << opLastConstruct->out << "\";\n";
	out
			<< "opLastLoad [shape=record,label=\"{  }| opLastLoad | { <o1> out }\"];\n";
	out << "opLastLoad:o1 -> \"" << opLastLoad->out << "\";\n";
	out
			<< "opLastNormalize [shape=record,label=\"{ <i1> in }| opLastNormalize | { <o1> out }\"];\n";
	out << "\"" << opLastNormalize->in << "\" -> opLastNormalize:i1;\n";
	out << "opLastNormalize:o1 -> \"" << opLastNormalize->out << "\";\n";
	out
			<< "opLastUpdate [shape=record,label=\"{ <i1> in }| opLastUpdate | { <o1> out }\"];\n";
	out << "\"" << opLastUpdate->in << "\" -> opLastUpdate:i1;\n";
	out << "opLastUpdate:o1 -> \"" << opLastUpdate->out << "\";\n";
	out
			<< "opUpperConstruct [shape=record,label=\"{ <i1> design_in | <i2> cs_in }| opUpperConstruct | { <o1> out }\"];\n";
	out << "\"" << opUpperConstruct->design_in
			<< "\" -> opUpperConstruct:i1;\n";
	out << "\"" << opUpperConstruct->cs_in << "\" -> opUpperConstruct:i2;\n";
	out << "opUpperConstruct:o1 -> \"" << opUpperConstruct->out << "\";\n";
	out
			<< "opUpperFlatten [shape=record,label=\"{ <i1> in }| opUpperFlatten | { <o1> out }\"];\n";
	out << "\"" << opUpperFlatten->in << "\" -> opUpperFlatten:i1;\n";
	out << "opUpperFlatten:o1 -> \"" << opUpperFlatten->out << "\";\n";
	out
			<< "project [shape=record,label=\"{ <i1> insoleFlatL | <i2> insoleL | <i3> heelL | <i4> lastL | <i5> csL}| project | { }\"];\n";
	out << "\"" << project.insoleFlatL << "\" -> project:i1;\n";
	out << "\"" << project.insoleL << "\" -> project:i2;\n";
	out << "\"" << project.heelL << "\" -> project:i3;\n";
	out << "\"" << project.lastL << "\" -> project:i4;\n";
	out << "\"" << project.csL << "\" -> project:i5;\n";
	out << "}\n";
}

void Builder::ToCSV(std::ostream &out) const {
	out << "----------- Input ---------------\n";
	out << std::left << std::setw(25) << opCoordinateSystemConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : "
			<< (opCoordinateSystemConstruct->in->IsNeeded() ?
					"needed" : "   -  ");
	out << " : "
			<< (opCoordinateSystemConstruct->in->IsValid() ?
					"   OK  " : "invalid") << "\n";
	out << std::left << std::setw(25) << opFootModelUpdate->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : " << (opFootModelUpdate->in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opFootModelUpdate->in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelCenter->GetName();
	out << std::left << " : " << std::setw(15) << "heel_in";
	out << " : " << (opHeelCenter->heel_in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opHeelCenter->heel_in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelCenter->GetName();
	out << std::left << " : " << std::setw(15) << "insole_in";
	out << " : " << (opHeelCenter->insole_in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opHeelCenter->insole_in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : " << (opHeelConstruct->in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opHeelConstruct->in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelExtractInsole->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : " << (opHeelExtractInsole->in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opHeelExtractInsole->in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelNormalize->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : " << (opHeelNormalize->in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opHeelNormalize->in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opInsoleAnalyze->GetName();
	out << std::left << " : " << std::setw(15) << "insole_in";
	out << " : "
			<< (opInsoleAnalyze->insole_in->IsNeeded() ? "needed" : "   -  ");
	out << " : "
			<< (opInsoleAnalyze->insole_in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opInsoleAnalyze->GetName();
	out << std::left << " : " << std::setw(15) << "insoleFlat_in";
	out << " : "
			<< (opInsoleAnalyze->insoleFlat_in->IsNeeded() ? "needed" : "   -  ");
	out << " : "
			<< (opInsoleAnalyze->insoleFlat_in->IsValid() ?
					"   OK  " : "invalid") << "\n";
	out << std::left << std::setw(25) << opInsoleFlatten->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : " << (opInsoleFlatten->in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opInsoleFlatten->in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opInsoleTransform->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : " << (opInsoleTransform->in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opInsoleTransform->in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opLastAnalyse->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : " << (opLastAnalyse->in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opLastAnalyse->in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opLastConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "insole";
	out << " : " << (opLastConstruct->insole->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opLastConstruct->insole->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opLastConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "cs";
	out << " : " << (opLastConstruct->cs->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opLastConstruct->cs->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opLastNormalize->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : " << (opLastNormalize->in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opLastNormalize->in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opLastUpdate->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : " << (opLastUpdate->in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opLastUpdate->in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opUpperConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "design_in";
	out << " : "
			<< (opUpperConstruct->design_in->IsNeeded() ? "needed" : "   -  ");
	out << " : "
			<< (opUpperConstruct->design_in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opUpperConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "cs_in";
	out << " : " << (opUpperConstruct->cs_in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opUpperConstruct->cs_in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opUpperFlatten->GetName();
	out << std::left << " : " << std::setw(15) << "in";
	out << " : " << (opUpperFlatten->in->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opUpperFlatten->in->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << "----------- Output --------------\n";
	out << std::left << std::setw(25) << opCoordinateSystemConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : "
			<< (opCoordinateSystemConstruct->out->IsNeeded() ?
					"needed" : "   -  ");
	out << " : "
			<< (opCoordinateSystemConstruct->out->IsValid() ?
					"   OK  " : "invalid") << "\n";
	out << std::left << std::setw(25) << opFootModelLoad->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opFootModelLoad->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opFootModelLoad->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opFootModelUpdate->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opFootModelUpdate->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opFootModelUpdate->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opFootScanLoad->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opFootScanLoad->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opFootScanLoad->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelCenter->GetName();
	out << std::left << " : " << std::setw(15) << "heel_out";
	out << " : " << (opHeelCenter->heel_out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opHeelCenter->heel_out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelCenter->GetName();
	out << std::left << " : " << std::setw(15) << "insole_out";
	out << " : "
			<< (opHeelCenter->insole_out->IsNeeded() ? "needed" : "   -  ");
	out << " : "
			<< (opHeelCenter->insole_out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opHeelConstruct->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opHeelConstruct->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelExtractInsole->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : "
			<< (opHeelExtractInsole->out->IsNeeded() ? "needed" : "   -  ");
	out << " : "
			<< (opHeelExtractInsole->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelLoad->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opHeelLoad->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opHeelLoad->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opHeelNormalize->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opHeelNormalize->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opHeelNormalize->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opInsoleAnalyze->GetName();
	out << std::left << " : " << std::setw(15) << "insole_out";
	out << " : "
			<< (opInsoleAnalyze->insole_out->IsNeeded() ? "needed" : "   -  ");
	out << " : "
			<< (opInsoleAnalyze->insole_out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opInsoleAnalyze->GetName();
	out << std::left << " : " << std::setw(15) << "insoleFlat_out";
	out << " : "
			<< (opInsoleAnalyze->insoleFlat_out->IsNeeded() ?
					"needed" : "   -  ");
	out << " : "
			<< (opInsoleAnalyze->insoleFlat_out->IsValid() ?
					"   OK  " : "invalid") << "\n";
	out << std::left << std::setw(25) << opInsoleConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opInsoleConstruct->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opInsoleConstruct->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opInsoleFlatten->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opInsoleFlatten->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opInsoleFlatten->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opInsoleTransform->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opInsoleTransform->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opInsoleTransform->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opLastAnalyse->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opLastAnalyse->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opLastAnalyse->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opLastConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opLastConstruct->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opLastConstruct->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opLastLoad->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opLastLoad->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opLastLoad->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opLastNormalize->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opLastNormalize->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opLastNormalize->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opLastUpdate->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opLastUpdate->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opLastUpdate->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opUpperConstruct->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opUpperConstruct->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opUpperConstruct->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
	out << std::left << std::setw(25) << opUpperFlatten->GetName();
	out << std::left << " : " << std::setw(15) << "out";
	out << " : " << (opUpperFlatten->out->IsNeeded() ? "needed" : "   -  ");
	out << " : " << (opUpperFlatten->out->IsValid() ? "   OK  " : "invalid")
			<< "\n";
}

#endif

void Builder::ResetState() {
	opCoordinateSystemConstruct->out->MarkNeeded(false);
	opFootModelLoad->out->MarkNeeded(false);
	opFootModelUpdate->out->MarkNeeded(false);
	opFootScanLoad->out->MarkNeeded(false);
	opHeelCenter->heel_out->MarkNeeded(false);
	opHeelCenter->insole_out->MarkNeeded(false);
	opHeelConstruct->out->MarkNeeded(false);
	opHeelExtractInsole->out->MarkNeeded(false);
	opHeelLoad->out->MarkNeeded(false);
	opHeelNormalize->out->MarkNeeded(false);
	opInsoleAnalyze->insole_out->MarkNeeded(false);
	opInsoleAnalyze->insoleFlat_out->MarkNeeded(false);
	opInsoleConstruct->out->MarkNeeded(false);
	opInsoleFlatten->out->MarkNeeded(false);
	opInsoleTransform->out->MarkNeeded(false);
	opLastAnalyse->out->MarkNeeded(false);
	opLastConstruct->out->MarkNeeded(false);
	opLastLoad->out->MarkNeeded(false);
	opLastNormalize->out->MarkNeeded(false);
	opLastUpdate->out->MarkNeeded(false);
	opUpperConstruct->out->MarkNeeded(false);
	opUpperFlatten->out->MarkNeeded(false);
}

void Builder::Connect(Project &project) {
	auto &config = project.config;

	const bool symmetric = (project.footL == project.footR);

	opFootModelUpdate->in = opFootModelLoad->out;

	opLastNormalize->in = opLastLoad->out;
	opLastAnalyse->in = opLastNormalize->out;
	opLastUpdate->in = opLastAnalyse->out;

	opHeelNormalize->in = opHeelLoad->out;
	opHeelExtractInsole->in = opHeelNormalize->out;
	opInsoleFlatten->in = opHeelExtractInsole->out;
	opInsoleAnalyze->insoleFlat_in = opInsoleFlatten->out;
	opInsoleAnalyze->insole_in = opHeelExtractInsole->out;
	opHeelCenter->heel_in = opHeelNormalize->out;
	opHeelCenter->insole_in = opInsoleAnalyze->insole_out;
	project.heelL = opHeelCenter->heel_out;

	if (config.heelConstructionType->IsSelection("construct")) {
		opInsoleTransform->in = opInsoleConstruct->out;
		project.insoleFlatL = opInsoleConstruct->out;
		project.insoleL = opInsoleTransform->out;
		opHeelConstruct->in = project.insoleL;
		project.heelL = opHeelConstruct->out;

		project.insoleFlatR = project.insoleFlatL;
		project.insoleR = project.insoleL;
		project.heelR = project.heelL;
	}

	if (config.heelConstructionType->IsSelection("loadFromFile")) {
		project.insoleL = opHeelCenter->insole_out;
		project.insoleFlatL = opInsoleAnalyze->insoleFlat_out;

		project.insoleFlatR = project.insoleFlatL;
		project.insoleR = project.insoleL;
		project.heelR = project.heelL;
	}

	opCoordinateSystemConstruct->in = project.insoleL;
	opUpperConstruct->design_in = project.design;
	opUpperConstruct->cs_in = opCoordinateSystemConstruct->out;
	opUpperFlatten->in = opUpperConstruct->out;
	project.upperL = opUpperFlatten->out;
	project.upperR = opUpperFlatten->out;
	project.csL = opCoordinateSystemConstruct->out;
	project.csR = opCoordinateSystemConstruct->out;

	if (config.lastConstructionType->IsSelection("construct")) {
		opLastConstruct->cs = project.csL;
		opLastConstruct->insole = project.insoleL;
		opLastAnalyse->in = opLastConstruct->out;
		project.lastL = opLastAnalyse->out;
	}

	if (config.lastConstructionType->IsSelection("boneBased")) {
		opFootModelUpdate->in = opFootModelLoad->out;

	}
	if (config.lastConstructionType->IsSelection("loadFromFile")) {

		project.lastR = opLastUpdate->out;
		project.lastL = opLastUpdate->out;
	}

	if (config.heelConstructionType->IsModified()) {
		project.insoleFlatL->MarkValid(false);
		project.insoleFlatR->MarkValid(false);
		project.insoleL->MarkValid(false);
		project.insoleR->MarkValid(false);
		project.heelL->MarkValid(false);
		project.heelR->MarkValid(false);
		DEBUGOUT << __FUNCTION__ << "heelConstructionType = "
				<< config.heelConstructionType->GetString() << "\n";
		DEBUGOUT << __FUNCTION__ << "lastConstructionType = "
				<< config.lastConstructionType->GetString() << "\n";
	}

#ifdef DEBUG
	{
		std::ofstream out("/tmp/graph.dot");
		ToDot(out, project);
	}
#endif

}

void Builder::Update(Project &project) {
	error.clear();

	project.design->UpdateEdges();
	Setup(project);

#ifdef DEBUG
	debug_count++;
//	std::string prefix = std::to_string(debug_count);
//	while (prefix.size() < 4)
//		prefix = std::string("0") + prefix;
//	{
//		std::ofstream out("/tmp/state_" + prefix + "_A__pre_check.txt");
//		ToCSV(out);
//	}
#endif

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
		ResetState();
		return;
	}

// Single threaded execution for debugging

	bool hasToRun = false;
	for (auto &op : operations)
		hasToRun |= op->HasToRun();
	if (!hasToRun) {
		ResetState();
		return;
	}

#ifdef DEBUG
	{
//		std::ofstream out("/tmp/state_" + prefix + "_B__pre_update.txt");
//		ToCSV(out);
	}
#endif

	DEBUGOUT << "----- Updating -----\n";
	StopWatch sw;
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
	sw.Stop();
	DEBUGOUT << "----- Updating done in " << sw.GetSecondsCPU() << "s -----\n";

#ifdef DEBUG
	{
//		std::ofstream out("/tmp/state_" + prefix + "_C__post_update.txt");
//		ToCSV(out);
	}
#endif
	ResetState();
}

void Builder::Paint() const {
#ifdef DEBUG
	for (auto &op : operations)
		op->Paint();
#endif
}

