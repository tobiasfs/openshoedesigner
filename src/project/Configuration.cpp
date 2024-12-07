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

void Configuration::Register(ParameterEvaluator &evaluator)
//		: heelHeight("heelHeight"), ballHeight("ballHeight"), heelPitch(
//				"heelPitch"), toeSpring("toeSpring"), bigToeAngle(
//				"bigToeAngle"), littleToeAngle("littleToeAngle"), ballMeasurementAngle(
//				"ballMeasurementAngle"), heelDirectionAngle(
//				"heelDirectionAngle"), upperLevel("upperLevel"), extraLength(
//				"extraLength"), footCompression("footCompression")
		{

	lastFilename = std::make_shared<ParameterString>("lastFilename",
			"data/Last_Default_Normalized.stl");

	std::initializer_list<std::string> types = { "boneBased", "lastBased" };
	modelType = std::make_shared<ParameterEnum>("modelType", types);

	heelHeight = evaluator.Register("heelHeight", "", "3 cm", ID_HEELHEIGHT);
	ballHeight = evaluator.Register("ballHeight", "", "1 cm", ID_BALLHEIGHT);
	heelPitch = evaluator.Register("heelPitch", "", "5 deg", ID_HEELPITCH);
	toeSpring = evaluator.Register("toeSpring", "", "10 deg", ID_TOESPRING);
	upperLevel = evaluator.Register("upperLevel", "", "0.8", ID_UPPERLEVEL);

	bigToeAngle = evaluator.Register("bigToeAngle", "", "6 deg",
	ID_BIGTOEANGLE);
	littleToeAngle = evaluator.Register("littleToeAngle", "", "2 deg",
	ID_LITTLETOEANGLE);
	ballMeasurementAngle = evaluator.Register("ballMeasurementAngle", "",
			"10 deg", ID_BALLMEASUREMENTANGLE);
	heelDirectionAngle = evaluator.Register("heelDirectionAngle", "", "10 deg",
	ID_HEELDIRECTIONANGLE);
	tipSharpness = evaluator.Register("tipSharpness", "", "0.0",
	ID_TIPSHARPNESS);
	extraLength = evaluator.Register("extraLength",
			"Difference between foot-length and last-length. To make a"
					" shoe walkable, this should be at least 2 cm"
					" (Standard value used in the shoe industry).",
			"footLength/15", ID_EXTRALENGTH);
	footCompression = evaluator.Register("footCompression",
			"Pressure from the sides of the shoe onto the foot."
					" Higher values give a tighter fit of the shoe."
					" This is needed for example with high heels to"
					" prevent the foot from slipping around in the shoe."
					" Set to 0 % for diabetic footwear.", "5/100",
			ID_FOOTCOMPRESSION);
}

bool Configuration::IsModified(void) const {
	return bigToeAngle->IsModified() | littleToeAngle->IsModified()
			| ballMeasurementAngle->IsModified()
			| heelDirectionAngle->IsModified() | upperLevel->IsModified()
			| extraLength->IsModified() | footCompression->IsModified()
			| heelHeight->IsModified() | ballHeight->IsModified()
			| heelPitch->IsModified() | toeSpring->IsModified()
			| tipSharpness->IsModified();
}

void Configuration::Modify(bool modified) {
	bigToeAngle->Modify(modified);
	littleToeAngle->Modify(modified);
	ballMeasurementAngle->Modify(modified);
	heelDirectionAngle->Modify(modified);
	upperLevel->Modify(modified);
	extraLength->Modify(modified);
	footCompression->Modify(modified);
	heelHeight->Modify(modified);
	ballHeight->Modify(modified);
	heelPitch->Modify(modified);
	toeSpring->Modify(modified);
	tipSharpness->Modify(modified);
}

bool Configuration::IsValidID(int id) {
	switch (id) {
	case ID_BIGTOEANGLE:
	case ID_LITTLETOEANGLE:
	case ID_BALLMEASUREMENTANGLE:
	case ID_HEELDIRECTIONANGLE:
	case ID_UPPERLEVEL:
	case ID_EXTRALENGTH:
	case ID_FOOTCOMPRESSION:
	case ID_HEELHEIGHT:
	case ID_BALLHEIGHT:
	case ID_HEELPITCH:
	case ID_TOESPRING:
	case ID_TIPSHARPNESS:
		return true;
	}
	return false;
}

std::string Configuration::GetName(int id) {
	switch (id) {
	case ID_BIGTOEANGLE:
		return std::string("BigToeAngle");
	case ID_LITTLETOEANGLE:
		return std::string("LittleToeAngle");
	case ID_BALLMEASUREMENTANGLE:
		return std::string("BallMeasurementAngle");
	case ID_HEELDIRECTIONANGLE:
		return std::string("HeelDirectionAngle");
	case ID_UPPERLEVEL:
		return std::string("UpperLevel");
	case ID_EXTRALENGTH:
		return std::string("ExtraLength");
	case ID_FOOTCOMPRESSION:
		return std::string("FootCompression");
	case ID_HEELHEIGHT:
		return std::string("HeelHeight");
	case ID_BALLHEIGHT:
		return std::string("BallHeight");
	case ID_HEELPITCH:
		return std::string("HeelPitch");
	case ID_TOESPRING:
		return std::string("ToeSpring");
	case ID_TIPSHARPNESS:
		return std::string("TipSharpness");
	default:
		throw(std::invalid_argument(
				std::string(__FILE__) + " : GetName : Passed invalid ID."));
	}
}

std::shared_ptr<Parameter> Configuration::GetParameter(int id) {
	switch (id) {
	case ID_BIGTOEANGLE:
		return bigToeAngle;
	case ID_LITTLETOEANGLE:
		return littleToeAngle;
	case ID_BALLMEASUREMENTANGLE:
		return ballMeasurementAngle;
	case ID_HEELDIRECTIONANGLE:
		return heelDirectionAngle;
	case ID_UPPERLEVEL:
		return upperLevel;
	case ID_EXTRALENGTH:
		return extraLength;
	case ID_FOOTCOMPRESSION:
		return footCompression;
	case ID_HEELHEIGHT:
		return heelHeight;
	case ID_BALLHEIGHT:
		return ballHeight;
	case ID_HEELPITCH:
		return heelPitch;
	case ID_TOESPRING:
		return toeSpring;
	case ID_TIPSHARPNESS:
		return tipSharpness;
	default:
		throw(std::invalid_argument(
				std::string(__FILE__) + " : GetParameter : Passed invalid ID."));
	}
}

const std::shared_ptr<const Parameter> Configuration::GetParameter(
		int id) const {
	switch (id) {
	case ID_BIGTOEANGLE:
		return bigToeAngle;
	case ID_LITTLETOEANGLE:
		return littleToeAngle;
	case ID_BALLMEASUREMENTANGLE:
		return ballMeasurementAngle;
	case ID_HEELDIRECTIONANGLE:
		return heelDirectionAngle;
	case ID_UPPERLEVEL:
		return upperLevel;
	case ID_EXTRALENGTH:
		return extraLength;
	case ID_FOOTCOMPRESSION:
		return footCompression;
	case ID_HEELHEIGHT:
		return heelHeight;
	case ID_BALLHEIGHT:
		return ballHeight;
	case ID_HEELPITCH:
		return heelPitch;
	case ID_TOESPRING:
		return toeSpring;
	case ID_TIPSHARPNESS:
		return tipSharpness;
	default:
		throw(std::invalid_argument(
				std::string(__FILE__) + " : GetParameter : Passed invalid ID."));
	}
}

void Configuration::FromJSON(const JSON &js) {
	if (!js.IsObject()) {
		std::ostringstream out;
		out << __FILE__ << ":" << __LINE__ << ":" << __func__ << " - ";
		out << " The json does not contain an object with measurements.";
		throw std::runtime_error(out.str());
	}
	if (js.HasKey("bigToeAngle"))
		bigToeAngle->SetFormula(js["bigToeAngle"].GetString(""));
	if (js.HasKey("littleToeAngle"))
		littleToeAngle->SetFormula(js["littleToeAngle"].GetString(""));
	if (js.HasKey("ballMeasurementAngle"))
		ballMeasurementAngle->SetFormula(
				js["ballMeasurementAngle"].GetString(""));
	if (js.HasKey("heelDirectionAngle"))
		heelDirectionAngle->SetFormula(js["heelDirectionAngle"].GetString(""));
	if (js.HasKey("upperLevel"))
		upperLevel->SetFormula(js["upperLevel"].GetString(""));
	if (js.HasKey("extraLength"))
		extraLength->SetFormula(js["extraLength"].GetString(""));
	if (js.HasKey("footCompression"))
		footCompression->SetFormula(js["footCompression"].GetString(""));
	if (js.HasKey("heelHeight"))
		heelHeight->SetFormula(js["heelHeight"].GetString(""));
	if (js.HasKey("ballHeight"))
		ballHeight->SetFormula(js["ballHeight"].GetString(""));
	if (js.HasKey("heelPitch"))
		heelPitch->SetFormula(js["heelPitch"].GetString(""));
	if (js.HasKey("toeSpring"))
		toeSpring->SetFormula(js["toeSpring"].GetString(""));
	if (js.HasKey("tipSharpness"))
		tipSharpness->SetFormula(js["tipSharpness"].GetString(""));
}

void Configuration::ToJSON(JSON &js) const {
	js.SetObject(true);
	js["bigToeAngle"].SetString(bigToeAngle->GetFormula());
	js["littleToeAngle"].SetString(littleToeAngle->GetFormula());
	js["ballMeasurementAngle"].SetString(ballMeasurementAngle->GetFormula());
	js["heelDirectionAngle"].SetString(heelDirectionAngle->GetFormula());
	js["upperLevel"].SetString(upperLevel->GetFormula());
	js["extraLength"].SetString(extraLength->GetFormula());
	js["footCompression"].SetString(footCompression->GetFormula());
	js["heelHeight"].SetString(heelHeight->GetFormula());
	js["ballHeight"].SetString(ballHeight->GetFormula());
	js["heelPitch"].SetString(heelPitch->GetFormula());
	js["toeSpring"].SetString(toeSpring->GetFormula());
	js["tipSharpness"].SetString(tipSharpness->GetFormula());
}
