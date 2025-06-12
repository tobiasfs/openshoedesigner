///////////////////////////////////////////////////////////////////////////////
// Name               : Configuration.cpp
// Purpose            : 
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
#include "Configuration.h"

#include "../gui/gui.h"

#include <stdexcept>
#include <sstream>

Configuration::Configuration() {

	std::initializer_list<ParameterEnum::Option> sources = { {
			"measurementBased" }, { "scanBased" } };
	measurementSource = std::make_shared<ParameterEnum>("measurementSource",
			sources, "");
	measurementSource->SetSelection(0);

	filenameScan = std::make_shared<ParameterString>("filenameScan", "");

	std::initializer_list<ParameterEnum::Option> types = { { "construct" }, {
			"boneBased" }, { "loadFromFile" } };
	lastConstructionType = std::make_shared<ParameterEnum>(
			"lastConstructionType", types, "",
			ID_LASTCONSTRUCTIONTYPE);
	lastConstructionType->SetSelection(0);

	bigToeAngle = std::make_shared<ParameterFormula>("bigToeAngle", "6 deg", "",
	ID_BIGTOEANGLE);
	littleToeAngle = std::make_shared<ParameterFormula>("littleToeAngle",
			"2 deg", "",
			ID_LITTLETOEANGLE);
	ballMeasurementAngle = std::make_shared<ParameterFormula>(
			"ballMeasurementAngle", "10 deg", "", ID_BALLMEASUREMENTANGLE);
	heelDirectionAngle = std::make_shared<ParameterFormula>(
			"heelDirectionAngle", "10 deg", "",
			ID_HEELDIRECTIONANGLE);
	tipSharpness = std::make_shared<ParameterFormula>("tipSharpness", "0", "",
	ID_TIPSHARPNESS);
	extraLength = std::make_shared<ParameterFormula>("extraLength",
			"footLength/15",
			"Difference between foot-length and last-length. To make a"
					" shoe walkable, this should be at least 2 cm"
					" (Standard value used in the shoe industry).",
			ID_EXTRALENGTH);
	footCompression = std::make_shared<ParameterFormula>("footCompression",
			"5/100", "Pressure from the sides of the shoe onto the foot."
					" Higher values give a tighter fit of the shoe."
					" This is needed for example with high heels to"
					" prevent the foot from slipping around in the shoe."
					" Set to 0 % for diabetic footwear.",
			ID_FOOTCOMPRESSION);

	filenameBoneModel = std::make_shared<ParameterString>("filenameBoneModel",
			"");
	filenameLast = std::make_shared<ParameterString>("filenameLast", "");

	std::initializer_list<ParameterEnum::Option> boolean = { { "false" }, {
			"true" } };
	lastModify = std::make_shared<ParameterEnum>("lastModify", boolean, "",
	ID_LASTMODIFY);
	lastReorient = std::make_shared<ParameterEnum>("lastReorient", boolean, "",
	ID_LASTREORIENT);

	std::initializer_list<ParameterEnum::Option> heeltypes = { { "construct" },
			{ "loadFromFile" } };
	heelConstructionType = std::make_shared<ParameterEnum>(
			"heelConstructionType", heeltypes, "",
			ID_LASTCONSTRUCTIONTYPE);
	heelConstructionType->SetSelection(0);

	filenameHeel = std::make_shared<ParameterString>("filenameHeel", "");
	heelReorient = std::make_shared<ParameterEnum>("heelReorient", boolean, "",
	ID_HEELREORIENT);

	heelHeight = std::make_shared<ParameterFormula>("heelHeight", "3 cm", "",
	ID_HEELHEIGHT);
	ballHeight = std::make_shared<ParameterFormula>("ballHeight", "1 cm", "",
	ID_BALLHEIGHT);
	heelPitch = std::make_shared<ParameterFormula>("heelPitch", "5 deg", "",
	ID_HEELPITCH);
	toeSpring = std::make_shared<ParameterFormula>("toeSpring", "10 deg", "",
	ID_TOESPRING);

	upperLevel = std::make_shared<ParameterFormula>("upperLevel", "0.8", "",
	ID_UPPERLEVEL);

	std::initializer_list<ParameterEnum::Option> generators =
			{ { "experimental",
					"Default generator for development of algorithms" },
					{ "welted",
							"Welt-sewn shoes: Generates last, insole, sole, upper pattern and cutout" },
					{ "cemented",
							"for cemented soles (simple, glued-together shoes)" },
					{ "molded",
							"for industrial shoes, where the sole is injection-molded to the upper" },
					{ "dutch",
							"Generator for dutch wooden clogs: Generates last, insole and clog" },
					{ "geta", "Japanese Geta generator" } };
	generator = std::make_shared<ParameterEnum>("generator", generators, "",
	ID_GENERATOR);
	generator->SetSelection(0);

	weltSize = std::make_shared<ParameterFormula>("weltSize", "0 mm", "",
	ID_WELTSIZE);

	thickness = std::make_shared<ParameterFormula>("thickness", "1 cm",
			"Thickness of the outer layer of the shoe.",
			ID_THICKNESS);

	supportHeelRadius = std::make_shared<ParameterFormula>("supportHeelRadius",
			"100 cm",
			"Radius of the walking support under the heel of the shoe.",
			ID_SUPPORTHEELRADIUS);
	supportHeelOffset =
			std::make_shared<ParameterFormula>("supportHeelOffset", "0 cm",
					"Offset of the walking support from the point directly under the heel forward.",
					ID_SUPPORTHEELOFFSET);
	supportToeRadius = std::make_shared<ParameterFormula>("supportToeRadius",
			"100 cm", "Radius of the walking support under the toes.",
			ID_SUPPORTTOERADIUS);
	supportToeOffset =
			std::make_shared<ParameterFormula>("supportToeOffset", "0.0 cm",
					"Distance how far the 90-deg-point is set back from the ball of the foot. Recommended is 0 cm.",
					ID_SUPPORTTOEOFFSET);

	heelCode = std::make_shared<ParameterString>("heelCode", "return 0;",
			"Code for generating the heel of the shoe.", ID_HEELCODE);

	debugMIDI_48 = std::make_shared<ParameterValue>("debugMIDI_48", 0,
			"MIDI Channel 48", 48);
	debugMIDI_49 = std::make_shared<ParameterValue>("debugMIDI_49", 0,
			"MIDI Channel 49", 49);
	debugMIDI_50 = std::make_shared<ParameterValue>("debugMIDI_50", 0,
			"MIDI Channel 50", 50);
	debugMIDI_51 = std::make_shared<ParameterValue>("debugMIDI_51", 0,
			"MIDI Channel 51", 51);
	debugMIDI_52 = std::make_shared<ParameterValue>("debugMIDI_52", 0,
			"MIDI Channel 52", 52);
	debugMIDI_53 = std::make_shared<ParameterValue>("debugMIDI_53", 0,
			"MIDI Channel 53", 53);
	debugMIDI_54 = std::make_shared<ParameterValue>("debugMIDI_54", 0,
			"MIDI Channel 54", 54);
	debugMIDI_55 = std::make_shared<ParameterValue>("debugMIDI_55", 0,
			"MIDI Channel 55", 55);
}

// Autogenerated code below. Do not modify, modification will be overwritten.

void Configuration::Register(ParameterEvaluator &evaluator) {
	evaluator.Register(bigToeAngle);
	evaluator.Register(littleToeAngle);
	evaluator.Register(ballMeasurementAngle);
	evaluator.Register(heelDirectionAngle);
	evaluator.Register(tipSharpness);
	evaluator.Register(extraLength);
	evaluator.Register(footCompression);
	evaluator.Register(heelHeight);
	evaluator.Register(ballHeight);
	evaluator.Register(heelPitch);
	evaluator.Register(toeSpring);
	evaluator.Register(upperLevel);
	evaluator.Register(weltSize);
	evaluator.Register(thickness);
	evaluator.Register(supportHeelRadius);
	evaluator.Register(supportHeelOffset);
	evaluator.Register(supportToeRadius);
	evaluator.Register(supportToeOffset);
}

bool Configuration::IsModified() const {
	return measurementSource->IsModified() | filenameScan->IsModified()
			| lastConstructionType->IsModified() | bigToeAngle->IsModified()
			| littleToeAngle->IsModified() | ballMeasurementAngle->IsModified()
			| heelDirectionAngle->IsModified() | tipSharpness->IsModified()
			| extraLength->IsModified() | footCompression->IsModified()
			| filenameBoneModel->IsModified() | filenameLast->IsModified()
			| lastModify->IsModified() | lastReorient->IsModified()
			| heelConstructionType->IsModified() | filenameHeel->IsModified()
			| heelReorient->IsModified() | heelHeight->IsModified()
			| ballHeight->IsModified() | heelPitch->IsModified()
			| toeSpring->IsModified() | upperLevel->IsModified()
			| generator->IsModified() | weltSize->IsModified()
			| thickness->IsModified() | supportHeelRadius->IsModified()
			| supportHeelOffset->IsModified() | supportToeRadius->IsModified()
			| supportToeOffset->IsModified() | heelCode->IsModified()
			| debugMIDI_48->IsModified() | debugMIDI_49->IsModified()
			| debugMIDI_50->IsModified() | debugMIDI_51->IsModified()
			| debugMIDI_52->IsModified() | debugMIDI_53->IsModified()
			| debugMIDI_54->IsModified() | debugMIDI_55->IsModified();
}

void Configuration::Modify(bool modified) {
	measurementSource->Modify(modified);
	filenameScan->Modify(modified);
	lastConstructionType->Modify(modified);
	bigToeAngle->Modify(modified);
	littleToeAngle->Modify(modified);
	ballMeasurementAngle->Modify(modified);
	heelDirectionAngle->Modify(modified);
	tipSharpness->Modify(modified);
	extraLength->Modify(modified);
	footCompression->Modify(modified);
	filenameBoneModel->Modify(modified);
	filenameLast->Modify(modified);
	lastModify->Modify(modified);
	lastReorient->Modify(modified);
	heelConstructionType->Modify(modified);
	filenameHeel->Modify(modified);
	heelReorient->Modify(modified);
	heelHeight->Modify(modified);
	ballHeight->Modify(modified);
	heelPitch->Modify(modified);
	toeSpring->Modify(modified);
	upperLevel->Modify(modified);
	generator->Modify(modified);
	weltSize->Modify(modified);
	thickness->Modify(modified);
	supportHeelRadius->Modify(modified);
	supportHeelOffset->Modify(modified);
	supportToeRadius->Modify(modified);
	supportToeOffset->Modify(modified);
	heelCode->Modify(modified);
	debugMIDI_48->Modify(modified);
	debugMIDI_49->Modify(modified);
	debugMIDI_50->Modify(modified);
	debugMIDI_51->Modify(modified);
	debugMIDI_52->Modify(modified);
	debugMIDI_53->Modify(modified);
	debugMIDI_54->Modify(modified);
	debugMIDI_55->Modify(modified);
}

bool Configuration::IsValidID(int id) {
	switch (id) {
	case ID_MEASUREMENTSOURCE:
	case ID_SCAN:
	case ID_LASTCONSTRUCTIONTYPE:
	case ID_BIGTOEANGLE:
	case ID_LITTLETOEANGLE:
	case ID_BALLMEASUREMENTANGLE:
	case ID_HEELDIRECTIONANGLE:
	case ID_TIPSHARPNESS:
	case ID_EXTRALENGTH:
	case ID_FOOTCOMPRESSION:
	case ID_BONES:
	case ID_LAST:
	case ID_LASTMODIFY:
	case ID_LASTREORIENT:
	case ID_HEELCONSTRUCTIONTYPE:
	case ID_HEEL:
	case ID_HEELREORIENT:
	case ID_HEELHEIGHT:
	case ID_BALLHEIGHT:
	case ID_HEELPITCH:
	case ID_TOESPRING:
	case ID_UPPERLEVEL:
	case ID_GENERATOR:
	case ID_WELTSIZE:
	case ID_THICKNESS:
	case ID_SUPPORTHEELRADIUS:
	case ID_SUPPORTHEELOFFSET:
	case ID_SUPPORTTOERADIUS:
	case ID_SUPPORTTOEOFFSET:
	case ID_HEELCODE:
		// No ID for debugMIDI_48
		// No ID for debugMIDI_49
		// No ID for debugMIDI_50
		// No ID for debugMIDI_51
		// No ID for debugMIDI_52
		// No ID for debugMIDI_53
		// No ID for debugMIDI_54
		// No ID for debugMIDI_55
		return true;
	}
	return false;
}

std::string Configuration::GetName(int id) {
	switch (id) {
	case ID_MEASUREMENTSOURCE:
		return std::string("MeasurementSource");
	case ID_SCAN:
		return std::string("FilenameScan");
	case ID_LASTCONSTRUCTIONTYPE:
		return std::string("LastConstructionType");
	case ID_BIGTOEANGLE:
		return std::string("BigToeAngle");
	case ID_LITTLETOEANGLE:
		return std::string("LittleToeAngle");
	case ID_BALLMEASUREMENTANGLE:
		return std::string("BallMeasurementAngle");
	case ID_HEELDIRECTIONANGLE:
		return std::string("HeelDirectionAngle");
	case ID_TIPSHARPNESS:
		return std::string("TipSharpness");
	case ID_EXTRALENGTH:
		return std::string("ExtraLength");
	case ID_FOOTCOMPRESSION:
		return std::string("FootCompression");
	case ID_BONES:
		return std::string("FilenameBoneModel");
	case ID_LAST:
		return std::string("FilenameLast");
	case ID_LASTMODIFY:
		return std::string("LastModify");
	case ID_LASTREORIENT:
		return std::string("LastReorient");
	case ID_HEELCONSTRUCTIONTYPE:
		return std::string("HeelConstructionType");
	case ID_HEEL:
		return std::string("FilenameHeel");
	case ID_HEELREORIENT:
		return std::string("HeelReorient");
	case ID_HEELHEIGHT:
		return std::string("HeelHeight");
	case ID_BALLHEIGHT:
		return std::string("BallHeight");
	case ID_HEELPITCH:
		return std::string("HeelPitch");
	case ID_TOESPRING:
		return std::string("ToeSpring");
	case ID_UPPERLEVEL:
		return std::string("UpperLevel");
	case ID_GENERATOR:
		return std::string("Generator");
	case ID_WELTSIZE:
		return std::string("WeltSize");
	case ID_THICKNESS:
		return std::string("Thickness");
	case ID_SUPPORTHEELRADIUS:
		return std::string("SupportHeelRadius");
	case ID_SUPPORTHEELOFFSET:
		return std::string("SupportHeelOffset");
	case ID_SUPPORTTOERADIUS:
		return std::string("SupportToeRadius");
	case ID_SUPPORTTOEOFFSET:
		return std::string("SupportToeOffset");
	case ID_HEELCODE:
		return std::string("HeelCode");
	default:
		throw(std::invalid_argument(
				std::string(__FILE__) + " : GetParameter : Passed invalid ID."));
	}
}

std::shared_ptr<ParameterFormula> Configuration::GetParameter(int id) {
	switch (id) {
	case ID_MEASUREMENTSOURCE:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'measurementSource' is a ParameterEnum."));
	case ID_SCAN:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'filenameScan' is a ParameterString."));
	case ID_LASTCONSTRUCTIONTYPE:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'lastConstructionType' is a ParameterEnum."));
	case ID_BIGTOEANGLE:
		return bigToeAngle;
	case ID_LITTLETOEANGLE:
		return littleToeAngle;
	case ID_BALLMEASUREMENTANGLE:
		return ballMeasurementAngle;
	case ID_HEELDIRECTIONANGLE:
		return heelDirectionAngle;
	case ID_TIPSHARPNESS:
		return tipSharpness;
	case ID_EXTRALENGTH:
		return extraLength;
	case ID_FOOTCOMPRESSION:
		return footCompression;
	case ID_BONES:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'filenameBoneModel' is a ParameterString."));
	case ID_LAST:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'filenameLast' is a ParameterString."));
	case ID_LASTMODIFY:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'lastModify' is a ParameterEnum."));
	case ID_LASTREORIENT:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'lastReorient' is a ParameterEnum."));
	case ID_HEELCONSTRUCTIONTYPE:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'heelConstructionType' is a ParameterEnum."));
	case ID_HEEL:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'filenameHeel' is a ParameterString."));
	case ID_HEELREORIENT:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'heelReorient' is a ParameterEnum."));
	case ID_HEELHEIGHT:
		return heelHeight;
	case ID_BALLHEIGHT:
		return ballHeight;
	case ID_HEELPITCH:
		return heelPitch;
	case ID_TOESPRING:
		return toeSpring;
	case ID_UPPERLEVEL:
		return upperLevel;
	case ID_GENERATOR:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'generator' is a ParameterEnum."));
	case ID_WELTSIZE:
		return weltSize;
	case ID_THICKNESS:
		return thickness;
	case ID_SUPPORTHEELRADIUS:
		return supportHeelRadius;
	case ID_SUPPORTHEELOFFSET:
		return supportHeelOffset;
	case ID_SUPPORTTOERADIUS:
		return supportToeRadius;
	case ID_SUPPORTTOEOFFSET:
		return supportToeOffset;
	case ID_HEELCODE:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'heelCode' is a ParameterString."));
	default:
		throw(std::invalid_argument(
				std::string(__FILE__) + " : GetParameter : Passed invalid ID."));
	}
}

const std::shared_ptr<const ParameterFormula> Configuration::GetParameter(
		int id) const {
	switch (id) {
	case ID_MEASUREMENTSOURCE:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'measurementSource' is a ParameterEnum."));
	case ID_SCAN:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'filenameScan' is a ParameterString."));
	case ID_LASTCONSTRUCTIONTYPE:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'lastConstructionType' is a ParameterEnum."));
	case ID_BIGTOEANGLE:
		return bigToeAngle;
	case ID_LITTLETOEANGLE:
		return littleToeAngle;
	case ID_BALLMEASUREMENTANGLE:
		return ballMeasurementAngle;
	case ID_HEELDIRECTIONANGLE:
		return heelDirectionAngle;
	case ID_TIPSHARPNESS:
		return tipSharpness;
	case ID_EXTRALENGTH:
		return extraLength;
	case ID_FOOTCOMPRESSION:
		return footCompression;
	case ID_BONES:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'filenameBoneModel' is a ParameterString."));
	case ID_LAST:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'filenameLast' is a ParameterString."));
	case ID_LASTMODIFY:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'lastModify' is a ParameterEnum."));
	case ID_LASTREORIENT:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'lastReorient' is a ParameterEnum."));
	case ID_HEELCONSTRUCTIONTYPE:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'heelConstructionType' is a ParameterEnum."));
	case ID_HEEL:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'filenameHeel' is a ParameterString."));
	case ID_HEELREORIENT:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'heelReorient' is a ParameterEnum."));
	case ID_HEELHEIGHT:
		return heelHeight;
	case ID_BALLHEIGHT:
		return ballHeight;
	case ID_HEELPITCH:
		return heelPitch;
	case ID_TOESPRING:
		return toeSpring;
	case ID_UPPERLEVEL:
		return upperLevel;
	case ID_GENERATOR:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'generator' is a ParameterEnum."));
	case ID_WELTSIZE:
		return weltSize;
	case ID_THICKNESS:
		return thickness;
	case ID_SUPPORTHEELRADIUS:
		return supportHeelRadius;
	case ID_SUPPORTHEELOFFSET:
		return supportHeelOffset;
	case ID_SUPPORTTOERADIUS:
		return supportToeRadius;
	case ID_SUPPORTTOEOFFSET:
		return supportToeOffset;
	case ID_HEELCODE:
		throw(std::invalid_argument(
				std::string(__FILE__)
						+ " : GetParameter : Only ParameterFormulas can be returned. 'heelCode' is a ParameterString."));
	default:
		throw(std::invalid_argument(
				std::string(__FILE__) + " : GetParameter : Passed invalid ID."));
	}
}

void Configuration::FromJSON(const JSON &js) {
	if (!js.IsObject()) {
		std::ostringstream out;
		out << __FILE__ << ":" << __func__ << ":";
		out << " The json does not contain an object with measurements.";
		throw std::runtime_error(out.str());
	}
	if (js.HasKey("measurementSource"))
		measurementSource->SetString(js["measurementSource"].GetString(""));
	if (js.HasKey("filenameScan"))
		filenameScan->SetString(js["filenameScan"].GetString(""));
	if (js.HasKey("lastConstructionType"))
		lastConstructionType->SetString(
				js["lastConstructionType"].GetString(""));
	if (js.HasKey("bigToeAngle"))
		bigToeAngle->SetString(js["bigToeAngle"].GetString(""));
	if (js.HasKey("littleToeAngle"))
		littleToeAngle->SetString(js["littleToeAngle"].GetString(""));
	if (js.HasKey("ballMeasurementAngle"))
		ballMeasurementAngle->SetString(
				js["ballMeasurementAngle"].GetString(""));
	if (js.HasKey("heelDirectionAngle"))
		heelDirectionAngle->SetString(js["heelDirectionAngle"].GetString(""));
	if (js.HasKey("tipSharpness"))
		tipSharpness->SetString(js["tipSharpness"].GetString(""));
	if (js.HasKey("extraLength"))
		extraLength->SetString(js["extraLength"].GetString(""));
	if (js.HasKey("footCompression"))
		footCompression->SetString(js["footCompression"].GetString(""));
	if (js.HasKey("filenameBoneModel"))
		filenameBoneModel->SetString(js["filenameBoneModel"].GetString(""));
	if (js.HasKey("filenameLast"))
		filenameLast->SetString(js["filenameLast"].GetString(""));
	if (js.HasKey("lastModify"))
		lastModify->SetString(js["lastModify"].GetString(""));
	if (js.HasKey("lastReorient"))
		lastReorient->SetString(js["lastReorient"].GetString(""));
	if (js.HasKey("heelConstructionType"))
		heelConstructionType->SetString(
				js["heelConstructionType"].GetString(""));
	if (js.HasKey("filenameHeel"))
		filenameHeel->SetString(js["filenameHeel"].GetString(""));
	if (js.HasKey("heelReorient"))
		heelReorient->SetString(js["heelReorient"].GetString(""));
	if (js.HasKey("heelHeight"))
		heelHeight->SetString(js["heelHeight"].GetString(""));
	if (js.HasKey("ballHeight"))
		ballHeight->SetString(js["ballHeight"].GetString(""));
	if (js.HasKey("heelPitch"))
		heelPitch->SetString(js["heelPitch"].GetString(""));
	if (js.HasKey("toeSpring"))
		toeSpring->SetString(js["toeSpring"].GetString(""));
	if (js.HasKey("upperLevel"))
		upperLevel->SetString(js["upperLevel"].GetString(""));
	if (js.HasKey("generator"))
		generator->SetString(js["generator"].GetString(""));
	if (js.HasKey("weltSize"))
		weltSize->SetString(js["weltSize"].GetString(""));
	if (js.HasKey("thickness"))
		thickness->SetString(js["thickness"].GetString(""));
	if (js.HasKey("supportHeelRadius"))
		supportHeelRadius->SetString(js["supportHeelRadius"].GetString(""));
	if (js.HasKey("supportHeelOffset"))
		supportHeelOffset->SetString(js["supportHeelOffset"].GetString(""));
	if (js.HasKey("supportToeRadius"))
		supportToeRadius->SetString(js["supportToeRadius"].GetString(""));
	if (js.HasKey("supportToeOffset"))
		supportToeOffset->SetString(js["supportToeOffset"].GetString(""));
	if (js.HasKey("heelCode"))
		heelCode->SetString(js["heelCode"].GetString(""));
	if (js.HasKey("debugMIDI_48"))
		debugMIDI_48->SetString(js["debugMIDI_48"].GetString(""));
	if (js.HasKey("debugMIDI_49"))
		debugMIDI_49->SetString(js["debugMIDI_49"].GetString(""));
	if (js.HasKey("debugMIDI_50"))
		debugMIDI_50->SetString(js["debugMIDI_50"].GetString(""));
	if (js.HasKey("debugMIDI_51"))
		debugMIDI_51->SetString(js["debugMIDI_51"].GetString(""));
	if (js.HasKey("debugMIDI_52"))
		debugMIDI_52->SetString(js["debugMIDI_52"].GetString(""));
	if (js.HasKey("debugMIDI_53"))
		debugMIDI_53->SetString(js["debugMIDI_53"].GetString(""));
	if (js.HasKey("debugMIDI_54"))
		debugMIDI_54->SetString(js["debugMIDI_54"].GetString(""));
	if (js.HasKey("debugMIDI_55"))
		debugMIDI_55->SetString(js["debugMIDI_55"].GetString(""));
}

void Configuration::ToJSON(JSON &js) const {
	js.SetObject(true);
	js["measurementSource"].SetString(measurementSource->GetString());
	js["filenameScan"].SetString(filenameScan->GetString());
	js["lastConstructionType"].SetString(lastConstructionType->GetString());
	js["bigToeAngle"].SetString(bigToeAngle->GetString());
	js["littleToeAngle"].SetString(littleToeAngle->GetString());
	js["ballMeasurementAngle"].SetString(ballMeasurementAngle->GetString());
	js["heelDirectionAngle"].SetString(heelDirectionAngle->GetString());
	js["tipSharpness"].SetString(tipSharpness->GetString());
	js["extraLength"].SetString(extraLength->GetString());
	js["footCompression"].SetString(footCompression->GetString());
	js["filenameBoneModel"].SetString(filenameBoneModel->GetString());
	js["filenameLast"].SetString(filenameLast->GetString());
	js["lastModify"].SetString(lastModify->GetString());
	js["lastReorient"].SetString(lastReorient->GetString());
	js["heelConstructionType"].SetString(heelConstructionType->GetString());
	js["filenameHeel"].SetString(filenameHeel->GetString());
	js["heelReorient"].SetString(heelReorient->GetString());
	js["heelHeight"].SetString(heelHeight->GetString());
	js["ballHeight"].SetString(ballHeight->GetString());
	js["heelPitch"].SetString(heelPitch->GetString());
	js["toeSpring"].SetString(toeSpring->GetString());
	js["upperLevel"].SetString(upperLevel->GetString());
	js["generator"].SetString(generator->GetString());
	js["weltSize"].SetString(weltSize->GetString());
	js["thickness"].SetString(thickness->GetString());
	js["supportHeelRadius"].SetString(supportHeelRadius->GetString());
	js["supportHeelOffset"].SetString(supportHeelOffset->GetString());
	js["supportToeRadius"].SetString(supportToeRadius->GetString());
	js["supportToeOffset"].SetString(supportToeOffset->GetString());
	js["heelCode"].SetString(heelCode->GetString());
}
