///////////////////////////////////////////////////////////////////////////////
// Name               : FootMeasurements.cpp
// Purpose            : Measurements for the foot (and the leg)
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 21.11.2018
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

#include "FootMeasurements.h"

#include "../gui/gui.h"
#include "../system/JSON.h"

#include <cmath>
#include <stdexcept>
#include <sstream>

void FootMeasurements::Register(ParameterEvaluator &evaluator) {

	legLengthDifference = evaluator.Register("legLengthDifferenceRef",
			"Distance the right leg is longer than the left (positive number)."
					" If the right leg is shorter than the left leg"
					" the number has to be negative.", "0.0 cm",
			ID_MEASUREMENT_LEGLENGTHDIFFERENCE);

	footLength = evaluator.Register("footLength",
			"Length of the foot (not length of the last)."
					" Measured from the longest toe to the heel.",
			"(39/3*2-1.5) cm", ID_MEASUREMENT_FOOTLENGTH);

	ballWidth = evaluator.Register("ballWidth",
			"Width of the foot from the ball of the little toe to"
					" the ball of the big toe. Measured at the"
					" widest part of the foot. The angle is"
					" normally not 90 degrees from the heel to"
					" toe line. (See also the Sole tab for this angle.)",
			"footLength/2.5", ID_MEASUREMENT_BALLWIDTH);
	bigToeGirth = evaluator.Register("bigToeGirth",
			"Grith around the big toe ball of the foot."
					" The measurement tape is only tensioned very lightly.",
			"ballWidth*2.1", ID_MEASUREMENT_BIGTOEGIRTH);
	littleToeGirth = evaluator.Register("littleToeGirth",
			"Measurement around the small toe ball."
					" The measurement tape is tightened, because this part"
					" has to support the foots position in the shoe.",
			"ballWidth*2.1",
			ID_MEASUREMENT_LITTLETOEGIRTH);
	waistGirth = evaluator.Register("waistGirth",
			"Measured around the middle of the foot over the"
					" metatarsalis bone. (That little dent You can feel"
					" in the middle of the outside of Your foot.)",
			"bigToeGirth*0.9", ID_MEASUREMENT_WAISTGIRTH);
	heelGirth = evaluator.Register("heelGirth", "", "waistGirth*1.3",
	ID_MEASUREMENT_HEELGIRTH);
	heelWidth = evaluator.Register("heelWidth",
			"Width of the heel at 1/6 of the length of the foot.",
			"ballWidth*3/4", ID_MEASUREMENT_HEELWIDTH);

	angleMixing = evaluator.Register("angleMixing",
			"Mixing determines how much of the needed bending of the foot"
					" is done by the heel and how much is spread on the"
					" forefoot. 0% is all heel, 100% is all forefoot."
					" 100% is impractical unless some serious"
					" misalignment of the foot is to be compensated.", "0.1",
			ID_MEASUREMENT_ANGLEMIXING);

	belowCrutchGirth = evaluator.Register("belowCrutchGirth",
			"Below crutch girth", "footLength*2.5",
			ID_MEASUREMENT_BELOWCRUTCHGIRTH);
	belowCrutchLevel = evaluator.Register("belowCrutchLevel",
			"Below crutch level", "footLength*3",
			ID_MEASUREMENT_BELOWCRUTCHLEVEL);
	middleOfCalfGirth = evaluator.Register("middleOfCalfGirth",
			"Middle of calf girth", "(belowCrutchGirth+footLength)/2",
			ID_MEASUREMENT_MIDDLEOFCALFGIRTH);
	middleOfCalfLevel = evaluator.Register("middleOfCalfLevel",
			"Middle of calf level", "belowCrutchLevel*0.825",
			ID_MEASUREMENT_MIDDLEOFCALFLEVEL);
	aboveKneeGirth = evaluator.Register("aboveKneeGirth", "Above knee girth",
			"footLength*1.4",
			ID_MEASUREMENT_ABOVEKNEEGIRTH);
	aboveKneeLevel = evaluator.Register("aboveKneeLevel", "Above knee level",
			"middleOfCalfLevel*0.785", ID_MEASUREMENT_ABOVEKNEELEVEL);
	overKneeCapGirth = evaluator.Register("overKneeCapGirth",
			"Over knee cap girth", "footLength*1.5",
			ID_MEASUREMENT_OVERKNEECAPGIRTH);
	overKneeCapLevel = evaluator.Register("overKneeCapLevel",
			"Over knee cap level", "aboveKneeLevel-footLength/6",
			ID_MEASUREMENT_OVERKNEECAPLEVEL);
	belowKneeGirth = evaluator.Register("belowKneeGirth", "Below knee girth",
			"footLength*1.2",
			ID_MEASUREMENT_BELOWKNEEGIRTH);
	belowKneeLevel = evaluator.Register("belowKneeLevel", "Below knee level",
			"overKneeCapLevel-footLength/6", ID_MEASUREMENT_BELOWKNEELEVEL);
	middleOfShankGirth = evaluator.Register("middleOfShankGirth",
			"Middle of shank girth", "footLength*1.3",
			ID_MEASUREMENT_MIDDLEOFSHANKGIRTH);
	middleOfShankLevel = evaluator.Register("middleOfShankLevel",
			"Middle of shank level", "overKneeCapLevel*0.7",
			ID_MEASUREMENT_MIDDLEOFSHANKLEVEL);
	aboveAnkleGirth = evaluator.Register("aboveAnkleGirth", "Above ankle girth",
			"footLength*0.8",
			ID_MEASUREMENT_ABOVEANKLEGIRTH);
	aboveAnkleLevel = evaluator.Register("aboveAnkleLevel", "Above ankle level",
			"footLength/2",
			ID_MEASUREMENT_ABOVEANKLELEVEL);
	overAnkleBoneGirth = evaluator.Register("overAnkleBoneGirth",
			"Over ankle bone girth", "footLength",
			ID_MEASUREMENT_OVERANKLEBONEGIRTH);
	overAnkleBoneLevel = evaluator.Register("overAnkleBoneLevel",
			"Over ankle bone level", "footLength/3",
			ID_MEASUREMENT_OVERANKLEBONELEVEL);
}

double FootMeasurements::GetSize(Type type) const {
	switch (type) {
	case Type::EU:
		return (footLength->ToDouble() + 1.5e-2) * 150;
	case Type::US:
		return (footLength->ToDouble()) / 2.54e-2 * 3 - 21.5;
	case Type::CN:
		return std::round((footLength->ToDouble() * 100.0) / 0.5) * 0.5;
	case Type::UK:
		return (footLength->ToDouble() + 1.5e-2) / 8.46e-3 - 25;
	case Type::JP:
		return std::round((footLength->ToDouble() * 1000.0) / 5) * 5;
	case Type::AU:
		return (footLength->ToDouble() + 1.5e-2) / 8.46e-3 - 25;
	case Type::mm:
		return footLength->ToDouble() * 1000.0;
	case Type::cm:
		return footLength->ToDouble() * 100.0;
	case Type::in:
		return footLength->ToDouble() / 0.0254;
	case Type::ft:
		return footLength->ToDouble() / 0.3048;
	}
	return 0;
}

void FootMeasurements::LoadJSON(std::string filename) {
	JSON js;
	try {
		js = JSON::Load(filename);
	} catch (std::exception &e) {
		std::ostringstream out;
		out << __FILE__ << ":" << __LINE__ << ":" << __func__ << " - ";
		out << " Error opening file:" << filename << ' ';
		out << e.what();
		throw std::runtime_error(out.str());
	}

	FromJSON(js);
}

void FootMeasurements::SaveJSON(std::string filename) const {
	JSON js;
	ToJSON(js);
	js.Save(filename);
}

bool FootMeasurements::IsModified(void) const {
	return legLengthDifference->IsModified() | footLength->IsModified()
			| ballWidth->IsModified() | bigToeGirth->IsModified()
			| littleToeGirth->IsModified() | waistGirth->IsModified()
			| heelGirth->IsModified() | heelWidth->IsModified()
			| angleMixing->IsModified() | belowCrutchGirth->IsModified()
			| belowCrutchLevel->IsModified() | middleOfCalfGirth->IsModified()
			| middleOfCalfLevel->IsModified() | aboveKneeGirth->IsModified()
			| aboveKneeLevel->IsModified() | overKneeCapGirth->IsModified()
			| overKneeCapLevel->IsModified() | belowKneeGirth->IsModified()
			| belowKneeLevel->IsModified() | middleOfShankGirth->IsModified()
			| middleOfShankLevel->IsModified() | aboveAnkleGirth->IsModified()
			| aboveAnkleLevel->IsModified() | overAnkleBoneGirth->IsModified()
			| overAnkleBoneLevel->IsModified();
}

void FootMeasurements::Modify(bool modified) {
	legLengthDifference->Modify(modified);
	footLength->Modify(modified);
	ballWidth->Modify(modified);
	bigToeGirth->Modify(modified);
	littleToeGirth->Modify(modified);
	waistGirth->Modify(modified);
	heelGirth->Modify(modified);
	heelWidth->Modify(modified);
	angleMixing->Modify(modified);
	belowCrutchGirth->Modify(modified);
	belowCrutchLevel->Modify(modified);
	middleOfCalfGirth->Modify(modified);
	middleOfCalfLevel->Modify(modified);
	aboveKneeGirth->Modify(modified);
	aboveKneeLevel->Modify(modified);
	overKneeCapGirth->Modify(modified);
	overKneeCapLevel->Modify(modified);
	belowKneeGirth->Modify(modified);
	belowKneeLevel->Modify(modified);
	middleOfShankGirth->Modify(modified);
	middleOfShankLevel->Modify(modified);
	aboveAnkleGirth->Modify(modified);
	aboveAnkleLevel->Modify(modified);
	overAnkleBoneGirth->Modify(modified);
	overAnkleBoneLevel->Modify(modified);
}

bool FootMeasurements::IsValidID(int id) {
	switch (id) {
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
	case ID_MEASUREMENT_FOOTLENGTH:
	case ID_MEASUREMENT_BALLWIDTH:
	case ID_MEASUREMENT_BIGTOEGIRTH:
	case ID_MEASUREMENT_LITTLETOEGIRTH:
	case ID_MEASUREMENT_WAISTGIRTH:
	case ID_MEASUREMENT_HEELGIRTH:
	case ID_MEASUREMENT_HEELWIDTH:
	case ID_MEASUREMENT_ANGLEMIXING:
	case ID_MEASUREMENT_BELOWCRUTCHGIRTH:
	case ID_MEASUREMENT_BELOWCRUTCHLEVEL:
	case ID_MEASUREMENT_MIDDLEOFCALFGIRTH:
	case ID_MEASUREMENT_MIDDLEOFCALFLEVEL:
	case ID_MEASUREMENT_ABOVEKNEEGIRTH:
	case ID_MEASUREMENT_ABOVEKNEELEVEL:
	case ID_MEASUREMENT_OVERKNEECAPGIRTH:
	case ID_MEASUREMENT_OVERKNEECAPLEVEL:
	case ID_MEASUREMENT_BELOWKNEEGIRTH:
	case ID_MEASUREMENT_BELOWKNEELEVEL:
	case ID_MEASUREMENT_MIDDLEOFSHANKGIRTH:
	case ID_MEASUREMENT_MIDDLEOFSHANKLEVEL:
	case ID_MEASUREMENT_ABOVEANKLEGIRTH:
	case ID_MEASUREMENT_ABOVEANKLELEVEL:
	case ID_MEASUREMENT_OVERANKLEBONEGIRTH:
	case ID_MEASUREMENT_OVERANKLEBONELEVEL:
		return true;
	}
	return false;
}

std::string FootMeasurements::GetName(int id) {
	switch (id) {
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
		return std::string("LegLengthDifference");
	case ID_MEASUREMENT_FOOTLENGTH:
		return std::string("FootLength");
	case ID_MEASUREMENT_BALLWIDTH:
		return std::string("BallWidth");
	case ID_MEASUREMENT_BIGTOEGIRTH:
		return std::string("BigToeGirth");
	case ID_MEASUREMENT_LITTLETOEGIRTH:
		return std::string("LittleToeGirth");
	case ID_MEASUREMENT_WAISTGIRTH:
		return std::string("WaistGirth");
	case ID_MEASUREMENT_HEELGIRTH:
		return std::string("HeelGirth");
	case ID_MEASUREMENT_HEELWIDTH:
		return std::string("HeelWidth");
	case ID_MEASUREMENT_ANGLEMIXING:
		return std::string("AngleMixing");
	case ID_MEASUREMENT_BELOWCRUTCHGIRTH:
		return std::string("BelowCrutchGirth");
	case ID_MEASUREMENT_BELOWCRUTCHLEVEL:
		return std::string("BelowCrutchLevel");
	case ID_MEASUREMENT_MIDDLEOFCALFGIRTH:
		return std::string("MiddleOfCalfGirth");
	case ID_MEASUREMENT_MIDDLEOFCALFLEVEL:
		return std::string("MiddleOfCalfLevel");
	case ID_MEASUREMENT_ABOVEKNEEGIRTH:
		return std::string("AboveKneeGirth");
	case ID_MEASUREMENT_ABOVEKNEELEVEL:
		return std::string("AboveKneeLevel");
	case ID_MEASUREMENT_OVERKNEECAPGIRTH:
		return std::string("OverKneeCapGirth");
	case ID_MEASUREMENT_OVERKNEECAPLEVEL:
		return std::string("OverKneeCapLevel");
	case ID_MEASUREMENT_BELOWKNEEGIRTH:
		return std::string("BelowKneeGirth");
	case ID_MEASUREMENT_BELOWKNEELEVEL:
		return std::string("BelowKneeLevel");
	case ID_MEASUREMENT_MIDDLEOFSHANKGIRTH:
		return std::string("MiddleOfShankGirth");
	case ID_MEASUREMENT_MIDDLEOFSHANKLEVEL:
		return std::string("MiddleOfShankLevel");
	case ID_MEASUREMENT_ABOVEANKLEGIRTH:
		return std::string("AboveAnkleGirth");
	case ID_MEASUREMENT_ABOVEANKLELEVEL:
		return std::string("AboveAnkleLevel");
	case ID_MEASUREMENT_OVERANKLEBONEGIRTH:
		return std::string("OverAnkleBoneGirth");
	case ID_MEASUREMENT_OVERANKLEBONELEVEL:
		return std::string("OverAnkleBoneLevel");
	default:
		throw(std::invalid_argument(
				std::string(__FILE__) + " : GetName : Passed invalid ID."));
	}
}

std::shared_ptr<Parameter> FootMeasurements::GetParameter(int id) {
	switch (id) {
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
		return legLengthDifference;
	case ID_MEASUREMENT_FOOTLENGTH:
		return footLength;
	case ID_MEASUREMENT_BALLWIDTH:
		return ballWidth;
	case ID_MEASUREMENT_BIGTOEGIRTH:
		return bigToeGirth;
	case ID_MEASUREMENT_LITTLETOEGIRTH:
		return littleToeGirth;
	case ID_MEASUREMENT_WAISTGIRTH:
		return waistGirth;
	case ID_MEASUREMENT_HEELGIRTH:
		return heelGirth;
	case ID_MEASUREMENT_HEELWIDTH:
		return heelWidth;
	case ID_MEASUREMENT_ANGLEMIXING:
		return angleMixing;
	case ID_MEASUREMENT_BELOWCRUTCHGIRTH:
		return belowCrutchGirth;
	case ID_MEASUREMENT_BELOWCRUTCHLEVEL:
		return belowCrutchLevel;
	case ID_MEASUREMENT_MIDDLEOFCALFGIRTH:
		return middleOfCalfGirth;
	case ID_MEASUREMENT_MIDDLEOFCALFLEVEL:
		return middleOfCalfLevel;
	case ID_MEASUREMENT_ABOVEKNEEGIRTH:
		return aboveKneeGirth;
	case ID_MEASUREMENT_ABOVEKNEELEVEL:
		return aboveKneeLevel;
	case ID_MEASUREMENT_OVERKNEECAPGIRTH:
		return overKneeCapGirth;
	case ID_MEASUREMENT_OVERKNEECAPLEVEL:
		return overKneeCapLevel;
	case ID_MEASUREMENT_BELOWKNEEGIRTH:
		return belowKneeGirth;
	case ID_MEASUREMENT_BELOWKNEELEVEL:
		return belowKneeLevel;
	case ID_MEASUREMENT_MIDDLEOFSHANKGIRTH:
		return middleOfShankGirth;
	case ID_MEASUREMENT_MIDDLEOFSHANKLEVEL:
		return middleOfShankLevel;
	case ID_MEASUREMENT_ABOVEANKLEGIRTH:
		return aboveAnkleGirth;
	case ID_MEASUREMENT_ABOVEANKLELEVEL:
		return aboveAnkleLevel;
	case ID_MEASUREMENT_OVERANKLEBONEGIRTH:
		return overAnkleBoneGirth;
	case ID_MEASUREMENT_OVERANKLEBONELEVEL:
		return overAnkleBoneLevel;
	default:
		throw(std::invalid_argument(
				std::string(__FILE__) + " : GetParameter : Passed invalid ID."));
	}
}

const std::shared_ptr<const Parameter> FootMeasurements::GetParameter(
		int id) const {
	switch (id) {
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
		return legLengthDifference;
	case ID_MEASUREMENT_FOOTLENGTH:
		return footLength;
	case ID_MEASUREMENT_BALLWIDTH:
		return ballWidth;
	case ID_MEASUREMENT_BIGTOEGIRTH:
		return bigToeGirth;
	case ID_MEASUREMENT_LITTLETOEGIRTH:
		return littleToeGirth;
	case ID_MEASUREMENT_WAISTGIRTH:
		return waistGirth;
	case ID_MEASUREMENT_HEELGIRTH:
		return heelGirth;
	case ID_MEASUREMENT_HEELWIDTH:
		return heelWidth;
	case ID_MEASUREMENT_ANGLEMIXING:
		return angleMixing;
	case ID_MEASUREMENT_BELOWCRUTCHGIRTH:
		return belowCrutchGirth;
	case ID_MEASUREMENT_BELOWCRUTCHLEVEL:
		return belowCrutchLevel;
	case ID_MEASUREMENT_MIDDLEOFCALFGIRTH:
		return middleOfCalfGirth;
	case ID_MEASUREMENT_MIDDLEOFCALFLEVEL:
		return middleOfCalfLevel;
	case ID_MEASUREMENT_ABOVEKNEEGIRTH:
		return aboveKneeGirth;
	case ID_MEASUREMENT_ABOVEKNEELEVEL:
		return aboveKneeLevel;
	case ID_MEASUREMENT_OVERKNEECAPGIRTH:
		return overKneeCapGirth;
	case ID_MEASUREMENT_OVERKNEECAPLEVEL:
		return overKneeCapLevel;
	case ID_MEASUREMENT_BELOWKNEEGIRTH:
		return belowKneeGirth;
	case ID_MEASUREMENT_BELOWKNEELEVEL:
		return belowKneeLevel;
	case ID_MEASUREMENT_MIDDLEOFSHANKGIRTH:
		return middleOfShankGirth;
	case ID_MEASUREMENT_MIDDLEOFSHANKLEVEL:
		return middleOfShankLevel;
	case ID_MEASUREMENT_ABOVEANKLEGIRTH:
		return aboveAnkleGirth;
	case ID_MEASUREMENT_ABOVEANKLELEVEL:
		return aboveAnkleLevel;
	case ID_MEASUREMENT_OVERANKLEBONEGIRTH:
		return overAnkleBoneGirth;
	case ID_MEASUREMENT_OVERANKLEBONELEVEL:
		return overAnkleBoneLevel;
	default:
		throw(std::invalid_argument(
				std::string(__FILE__) + " : GetParameter : Passed invalid ID."));
	}
}

void FootMeasurements::FromJSON(const JSON &js) {
	if (!js.IsObject()) {
		std::ostringstream out;
		out << __FILE__ << ":" << __func__ << ":";
		out << " The json does not contain an object with measurements.";
		throw std::runtime_error(out.str());
	}
	if (js.HasKey("legLengthDifference"))
		legLengthDifference->SetFormula(
				js["legLengthDifference"].GetString(""));
	if (js.HasKey("footLength"))
		footLength->SetFormula(js["footLength"].GetString(""));
	if (js.HasKey("ballWidth"))
		ballWidth->SetFormula(js["ballWidth"].GetString(""));
	if (js.HasKey("bigToeGirth"))
		bigToeGirth->SetFormula(js["bigToeGirth"].GetString(""));
	if (js.HasKey("littleToeGirth"))
		littleToeGirth->SetFormula(js["littleToeGirth"].GetString(""));
	if (js.HasKey("waistGirth"))
		waistGirth->SetFormula(js["waistGirth"].GetString(""));
	if (js.HasKey("heelGirth"))
		heelGirth->SetFormula(js["heelGirth"].GetString(""));
	if (js.HasKey("heelWidth"))
		heelWidth->SetFormula(js["heelWidth"].GetString(""));
	if (js.HasKey("angleMixing"))
		angleMixing->SetFormula(js["angleMixing"].GetString(""));
	if (js.HasKey("belowCrutchGirth"))
		belowCrutchGirth->SetFormula(js["belowCrutchGirth"].GetString(""));
	if (js.HasKey("belowCrutchLevel"))
		belowCrutchLevel->SetFormula(js["belowCrutchLevel"].GetString(""));
	if (js.HasKey("middleOfCalfGirth"))
		middleOfCalfGirth->SetFormula(js["middleOfCalfGirth"].GetString(""));
	if (js.HasKey("middleOfCalfLevel"))
		middleOfCalfLevel->SetFormula(js["middleOfCalfLevel"].GetString(""));
	if (js.HasKey("aboveKneeGirth"))
		aboveKneeGirth->SetFormula(js["aboveKneeGirth"].GetString(""));
	if (js.HasKey("aboveKneeLevel"))
		aboveKneeLevel->SetFormula(js["aboveKneeLevel"].GetString(""));
	if (js.HasKey("overKneeCapGirth"))
		overKneeCapGirth->SetFormula(js["overKneeCapGirth"].GetString(""));
	if (js.HasKey("overKneeCapLevel"))
		overKneeCapLevel->SetFormula(js["overKneeCapLevel"].GetString(""));
	if (js.HasKey("belowKneeGirth"))
		belowKneeGirth->SetFormula(js["belowKneeGirth"].GetString(""));
	if (js.HasKey("belowKneeLevel"))
		belowKneeLevel->SetFormula(js["belowKneeLevel"].GetString(""));
	if (js.HasKey("middleOfShankGirth"))
		middleOfShankGirth->SetFormula(js["middleOfShankGirth"].GetString(""));
	if (js.HasKey("middleOfShankLevel"))
		middleOfShankLevel->SetFormula(js["middleOfShankLevel"].GetString(""));
	if (js.HasKey("aboveAnkleGirth"))
		aboveAnkleGirth->SetFormula(js["aboveAnkleGirth"].GetString(""));
	if (js.HasKey("aboveAnkleLevel"))
		aboveAnkleLevel->SetFormula(js["aboveAnkleLevel"].GetString(""));
	if (js.HasKey("overAnkleBoneGirth"))
		overAnkleBoneGirth->SetFormula(js["overAnkleBoneGirth"].GetString(""));
	if (js.HasKey("overAnkleBoneLevel"))
		overAnkleBoneLevel->SetFormula(js["overAnkleBoneLevel"].GetString(""));
}

void FootMeasurements::ToJSON(JSON &js) const {
	js.SetObject(true);
	js["legLengthDifference"].SetString(legLengthDifference->GetFormula());
	js["footLength"].SetString(footLength->GetFormula());
	js["ballWidth"].SetString(ballWidth->GetFormula());
	js["bigToeGirth"].SetString(bigToeGirth->GetFormula());
	js["littleToeGirth"].SetString(littleToeGirth->GetFormula());
	js["waistGirth"].SetString(waistGirth->GetFormula());
	js["heelGirth"].SetString(heelGirth->GetFormula());
	js["heelWidth"].SetString(heelWidth->GetFormula());
	js["angleMixing"].SetString(angleMixing->GetFormula());
	js["belowCrutchGirth"].SetString(belowCrutchGirth->GetFormula());
	js["belowCrutchLevel"].SetString(belowCrutchLevel->GetFormula());
	js["middleOfCalfGirth"].SetString(middleOfCalfGirth->GetFormula());
	js["middleOfCalfLevel"].SetString(middleOfCalfLevel->GetFormula());
	js["aboveKneeGirth"].SetString(aboveKneeGirth->GetFormula());
	js["aboveKneeLevel"].SetString(aboveKneeLevel->GetFormula());
	js["overKneeCapGirth"].SetString(overKneeCapGirth->GetFormula());
	js["overKneeCapLevel"].SetString(overKneeCapLevel->GetFormula());
	js["belowKneeGirth"].SetString(belowKneeGirth->GetFormula());
	js["belowKneeLevel"].SetString(belowKneeLevel->GetFormula());
	js["middleOfShankGirth"].SetString(middleOfShankGirth->GetFormula());
	js["middleOfShankLevel"].SetString(middleOfShankLevel->GetFormula());
	js["aboveAnkleGirth"].SetString(aboveAnkleGirth->GetFormula());
	js["aboveAnkleLevel"].SetString(aboveAnkleLevel->GetFormula());
	js["overAnkleBoneGirth"].SetString(overAnkleBoneGirth->GetFormula());
	js["overAnkleBoneLevel"].SetString(overAnkleBoneLevel->GetFormula());
}
