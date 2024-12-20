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

FootMeasurements::FootMeasurements() {

	legLengthDifference = std::make_shared<ParameterFormula>(
			"legLengthDifference", "0.0 cm",
			"Distance this leg is longer than the other."
					" Set this on only one leg. Negative numbers mean "
					" that this leg is shorter.",
			ID_MEASUREMENT_LEGLENGTHDIFFERENCE);

	footLength = std::make_shared<ParameterFormula>("footLength",
			"(39/3*2-1.5) cm", "Length of the foot (not length of the last)."
					" Measured from the longest toe to the heel.",
			ID_MEASUREMENT_FOOTLENGTH);

	ballWidth = std::make_shared<ParameterFormula>("ballWidth",
			"footLength/2.5",
			"Width of the foot from the ball of the little toe to"
					" the ball of the big toe. Measured at the"
					" widest part of the foot. The angle is"
					" normally not 90 degrees from the heel to"
					" toe line. (See also the Sole tab for this angle.)",
			ID_MEASUREMENT_BALLWIDTH);
	bigToeGirth = std::make_shared<ParameterFormula>("bigToeGirth",
			"ballWidth*2.1", "Grith around the big toe ball of the foot."
					" The measurement tape is only tensioned very lightly.",
			ID_MEASUREMENT_BIGTOEGIRTH);
	littleToeGirth = std::make_shared<ParameterFormula>("littleToeGirth",
			"ballWidth*2.1", "Measurement around the small toe ball."
					" The measurement tape is tightened  because this part"
					" has to support the foots position in the shoe.",
			ID_MEASUREMENT_LITTLETOEGIRTH);
	waistGirth = std::make_shared<ParameterFormula>("waistGirth",
			"bigToeGirth*0.9", "Measured around the middle of the foot over the"
					" metatarsalis bone. (That little dent You can feel"
					" in the middle of the outside of Your foot.)",
			ID_MEASUREMENT_WAISTGIRTH);
	heelGirth = std::make_shared<ParameterFormula>("heelGirth",
			"waistGirth*1.3", "",
			ID_MEASUREMENT_HEELGIRTH);
	heelWidth = std::make_shared<ParameterFormula>("heelWidth", "ballWidth*3/4",
			"Width of the heel at 1/6 of the length of the foot.",
			ID_MEASUREMENT_HEELWIDTH);

	angleMixing = std::make_shared<ParameterFormula>("angleMixing", "0.1",
			"Mixing determines how much of the needed bending of the foot"
					" is done by the heel and how much is spread on the"
					" forefoot. 0% is all heel; 100% is all forefoot."
					" 100% is impractical unless some serious"
					" misalignment of the foot is to be compensated.",
			ID_MEASUREMENT_ANGLEMIXING);

	belowCrutchGirth = std::make_shared<ParameterFormula>("belowCrutchGirth",
			"footLength*2.5", "Below crutch girth",
			ID_MEASUREMENT_BELOWCRUTCHGIRTH);
	belowCrutchLevel = std::make_shared<ParameterFormula>("belowCrutchLevel",
			"footLength*3", "Below crutch level",
			ID_MEASUREMENT_BELOWCRUTCHLEVEL);
	middleOfCalfGirth = std::make_shared<ParameterFormula>("middleOfCalfGirth",
			"(belowCrutchGirth+footLength)/2", "Middle of calf girth",
			ID_MEASUREMENT_MIDDLEOFCALFGIRTH);
	middleOfCalfLevel = std::make_shared<ParameterFormula>("middleOfCalfLevel",
			"belowCrutchLevel*0.825", "Middle of calf level",
			ID_MEASUREMENT_MIDDLEOFCALFLEVEL);
	aboveKneeGirth = std::make_shared<ParameterFormula>("aboveKneeGirth",
			"footLength*1.4", "Above knee girth",
			ID_MEASUREMENT_ABOVEKNEEGIRTH);
	aboveKneeLevel = std::make_shared<ParameterFormula>("aboveKneeLevel",
			"middleOfCalfLevel*0.785", "Above knee level",
			ID_MEASUREMENT_ABOVEKNEELEVEL);
	overKneeCapGirth = std::make_shared<ParameterFormula>("overKneeCapGirth",
			"footLength*1.5", "Over knee cap girth",
			ID_MEASUREMENT_OVERKNEECAPGIRTH);
	overKneeCapLevel = std::make_shared<ParameterFormula>("overKneeCapLevel",
			"aboveKneeLevel-footLength/6", "Over knee cap level",
			ID_MEASUREMENT_OVERKNEECAPLEVEL);
	belowKneeGirth = std::make_shared<ParameterFormula>("belowKneeGirth",
			"footLength*1.2", "Below knee girth",
			ID_MEASUREMENT_BELOWKNEEGIRTH);
	belowKneeLevel = std::make_shared<ParameterFormula>("belowKneeLevel",
			"overKneeCapLevel-footLength/6", "Below knee level",
			ID_MEASUREMENT_BELOWKNEELEVEL);
	middleOfShankGirth = std::make_shared<ParameterFormula>(
			"middleOfShankGirth", "footLength*1.3", "Middle of shank girth",
			ID_MEASUREMENT_MIDDLEOFSHANKGIRTH);
	middleOfShankLevel = std::make_shared<ParameterFormula>(
			"middleOfShankLevel", "overKneeCapLevel*0.7",
			"Middle of shank level",
			ID_MEASUREMENT_MIDDLEOFSHANKLEVEL);
	aboveAnkleGirth = std::make_shared<ParameterFormula>("aboveAnkleGirth",
			"footLength*0.8", "Above ankle girth",
			ID_MEASUREMENT_ABOVEANKLEGIRTH);
	aboveAnkleLevel = std::make_shared<ParameterFormula>("aboveAnkleLevel",
			"footLength/2", "Above ankle level",
			ID_MEASUREMENT_ABOVEANKLELEVEL);
	overAnkleBoneGirth = std::make_shared<ParameterFormula>(
			"overAnkleBoneGirth", "footLength", "Over ankle bone girth",
			ID_MEASUREMENT_OVERANKLEBONEGIRTH);
	overAnkleBoneLevel = std::make_shared<ParameterFormula>(
			"overAnkleBoneLevel", "footLength/3", "Over ankle bone level",
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
		out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
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

// Autogenerated code below. Do not modify, modification will be overwritten.

void FootMeasurements::Register(ParameterEvaluator &evaluator) {
	evaluator.Register(footLength);
	evaluator.Register(ballWidth);
	evaluator.Register(bigToeGirth);
	evaluator.Register(littleToeGirth);
	evaluator.Register(waistGirth);
	evaluator.Register(heelGirth);
	evaluator.Register(heelWidth);
	evaluator.Register(angleMixing);
	evaluator.Register(legLengthDifference);
	evaluator.Register(belowCrutchGirth);
	evaluator.Register(belowCrutchLevel);
	evaluator.Register(middleOfCalfGirth);
	evaluator.Register(middleOfCalfLevel);
	evaluator.Register(aboveKneeGirth);
	evaluator.Register(aboveKneeLevel);
	evaluator.Register(overKneeCapGirth);
	evaluator.Register(overKneeCapLevel);
	evaluator.Register(belowKneeGirth);
	evaluator.Register(belowKneeLevel);
	evaluator.Register(middleOfShankGirth);
	evaluator.Register(middleOfShankLevel);
	evaluator.Register(aboveAnkleGirth);
	evaluator.Register(aboveAnkleLevel);
	evaluator.Register(overAnkleBoneGirth);
	evaluator.Register(overAnkleBoneLevel);
}

bool FootMeasurements::operator==(const FootMeasurements &other) const {
	if (footLength->GetString() != other.footLength->GetString())
		return false;
	if (ballWidth->GetString() != other.ballWidth->GetString())
		return false;
	if (bigToeGirth->GetString() != other.bigToeGirth->GetString())
		return false;
	if (littleToeGirth->GetString() != other.littleToeGirth->GetString())
		return false;
	if (waistGirth->GetString() != other.waistGirth->GetString())
		return false;
	if (heelGirth->GetString() != other.heelGirth->GetString())
		return false;
	if (heelWidth->GetString() != other.heelWidth->GetString())
		return false;
	if (angleMixing->GetString() != other.angleMixing->GetString())
		return false;
	if (legLengthDifference->GetString()
			!= other.legLengthDifference->GetString())
		return false;
	if (belowCrutchGirth->GetString() != other.belowCrutchGirth->GetString())
		return false;
	if (belowCrutchLevel->GetString() != other.belowCrutchLevel->GetString())
		return false;
	if (middleOfCalfGirth->GetString() != other.middleOfCalfGirth->GetString())
		return false;
	if (middleOfCalfLevel->GetString() != other.middleOfCalfLevel->GetString())
		return false;
	if (aboveKneeGirth->GetString() != other.aboveKneeGirth->GetString())
		return false;
	if (aboveKneeLevel->GetString() != other.aboveKneeLevel->GetString())
		return false;
	if (overKneeCapGirth->GetString() != other.overKneeCapGirth->GetString())
		return false;
	if (overKneeCapLevel->GetString() != other.overKneeCapLevel->GetString())
		return false;
	if (belowKneeGirth->GetString() != other.belowKneeGirth->GetString())
		return false;
	if (belowKneeLevel->GetString() != other.belowKneeLevel->GetString())
		return false;
	if (middleOfShankGirth->GetString()
			!= other.middleOfShankGirth->GetString())
		return false;
	if (middleOfShankLevel->GetString()
			!= other.middleOfShankLevel->GetString())
		return false;
	if (aboveAnkleGirth->GetString() != other.aboveAnkleGirth->GetString())
		return false;
	if (aboveAnkleLevel->GetString() != other.aboveAnkleLevel->GetString())
		return false;
	if (overAnkleBoneGirth->GetString()
			!= other.overAnkleBoneGirth->GetString())
		return false;
	if (overAnkleBoneLevel->GetString()
			!= other.overAnkleBoneLevel->GetString())
		return false;
	return true;
}

bool FootMeasurements::IsModified() const {
	return footLength->IsModified() | ballWidth->IsModified()
			| bigToeGirth->IsModified() | littleToeGirth->IsModified()
			| waistGirth->IsModified() | heelGirth->IsModified()
			| heelWidth->IsModified() | angleMixing->IsModified()
			| legLengthDifference->IsModified() | belowCrutchGirth->IsModified()
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
	footLength->Modify(modified);
	ballWidth->Modify(modified);
	bigToeGirth->Modify(modified);
	littleToeGirth->Modify(modified);
	waistGirth->Modify(modified);
	heelGirth->Modify(modified);
	heelWidth->Modify(modified);
	angleMixing->Modify(modified);
	legLengthDifference->Modify(modified);
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
	case ID_MEASUREMENT_FOOTLENGTH:
	case ID_MEASUREMENT_BALLWIDTH:
	case ID_MEASUREMENT_BIGTOEGIRTH:
	case ID_MEASUREMENT_LITTLETOEGIRTH:
	case ID_MEASUREMENT_WAISTGIRTH:
	case ID_MEASUREMENT_HEELGIRTH:
	case ID_MEASUREMENT_HEELWIDTH:
	case ID_MEASUREMENT_ANGLEMIXING:
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
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
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
		return std::string("LegLengthDifference");
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
				std::string(__FILE__) + " : GetParameter : Passed invalid ID."));
	}
}

std::shared_ptr<ParameterFormula> FootMeasurements::GetParameter(int id) {
	switch (id) {
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
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
		return legLengthDifference;
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

const std::shared_ptr<const ParameterFormula> FootMeasurements::GetParameter(
		int id) const {
	switch (id) {
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
	case ID_MEASUREMENT_LEGLENGTHDIFFERENCE:
		return legLengthDifference;
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
	if (js.HasKey("footLength"))
		footLength->SetString(js["footLength"].GetString(""));
	if (js.HasKey("ballWidth"))
		ballWidth->SetString(js["ballWidth"].GetString(""));
	if (js.HasKey("bigToeGirth"))
		bigToeGirth->SetString(js["bigToeGirth"].GetString(""));
	if (js.HasKey("littleToeGirth"))
		littleToeGirth->SetString(js["littleToeGirth"].GetString(""));
	if (js.HasKey("waistGirth"))
		waistGirth->SetString(js["waistGirth"].GetString(""));
	if (js.HasKey("heelGirth"))
		heelGirth->SetString(js["heelGirth"].GetString(""));
	if (js.HasKey("heelWidth"))
		heelWidth->SetString(js["heelWidth"].GetString(""));
	if (js.HasKey("angleMixing"))
		angleMixing->SetString(js["angleMixing"].GetString(""));
	if (js.HasKey("legLengthDifference"))
		legLengthDifference->SetString(js["legLengthDifference"].GetString(""));
	if (js.HasKey("belowCrutchGirth"))
		belowCrutchGirth->SetString(js["belowCrutchGirth"].GetString(""));
	if (js.HasKey("belowCrutchLevel"))
		belowCrutchLevel->SetString(js["belowCrutchLevel"].GetString(""));
	if (js.HasKey("middleOfCalfGirth"))
		middleOfCalfGirth->SetString(js["middleOfCalfGirth"].GetString(""));
	if (js.HasKey("middleOfCalfLevel"))
		middleOfCalfLevel->SetString(js["middleOfCalfLevel"].GetString(""));
	if (js.HasKey("aboveKneeGirth"))
		aboveKneeGirth->SetString(js["aboveKneeGirth"].GetString(""));
	if (js.HasKey("aboveKneeLevel"))
		aboveKneeLevel->SetString(js["aboveKneeLevel"].GetString(""));
	if (js.HasKey("overKneeCapGirth"))
		overKneeCapGirth->SetString(js["overKneeCapGirth"].GetString(""));
	if (js.HasKey("overKneeCapLevel"))
		overKneeCapLevel->SetString(js["overKneeCapLevel"].GetString(""));
	if (js.HasKey("belowKneeGirth"))
		belowKneeGirth->SetString(js["belowKneeGirth"].GetString(""));
	if (js.HasKey("belowKneeLevel"))
		belowKneeLevel->SetString(js["belowKneeLevel"].GetString(""));
	if (js.HasKey("middleOfShankGirth"))
		middleOfShankGirth->SetString(js["middleOfShankGirth"].GetString(""));
	if (js.HasKey("middleOfShankLevel"))
		middleOfShankLevel->SetString(js["middleOfShankLevel"].GetString(""));
	if (js.HasKey("aboveAnkleGirth"))
		aboveAnkleGirth->SetString(js["aboveAnkleGirth"].GetString(""));
	if (js.HasKey("aboveAnkleLevel"))
		aboveAnkleLevel->SetString(js["aboveAnkleLevel"].GetString(""));
	if (js.HasKey("overAnkleBoneGirth"))
		overAnkleBoneGirth->SetString(js["overAnkleBoneGirth"].GetString(""));
	if (js.HasKey("overAnkleBoneLevel"))
		overAnkleBoneLevel->SetString(js["overAnkleBoneLevel"].GetString(""));
}

void FootMeasurements::ToJSON(JSON &js) const {
	js.SetObject(true);
	js["footLength"].SetString(footLength->GetString());
	js["ballWidth"].SetString(ballWidth->GetString());
	js["bigToeGirth"].SetString(bigToeGirth->GetString());
	js["littleToeGirth"].SetString(littleToeGirth->GetString());
	js["waistGirth"].SetString(waistGirth->GetString());
	js["heelGirth"].SetString(heelGirth->GetString());
	js["heelWidth"].SetString(heelWidth->GetString());
	js["angleMixing"].SetString(angleMixing->GetString());
	js["legLengthDifference"].SetString(legLengthDifference->GetString());
	js["belowCrutchGirth"].SetString(belowCrutchGirth->GetString());
	js["belowCrutchLevel"].SetString(belowCrutchLevel->GetString());
	js["middleOfCalfGirth"].SetString(middleOfCalfGirth->GetString());
	js["middleOfCalfLevel"].SetString(middleOfCalfLevel->GetString());
	js["aboveKneeGirth"].SetString(aboveKneeGirth->GetString());
	js["aboveKneeLevel"].SetString(aboveKneeLevel->GetString());
	js["overKneeCapGirth"].SetString(overKneeCapGirth->GetString());
	js["overKneeCapLevel"].SetString(overKneeCapLevel->GetString());
	js["belowKneeGirth"].SetString(belowKneeGirth->GetString());
	js["belowKneeLevel"].SetString(belowKneeLevel->GetString());
	js["middleOfShankGirth"].SetString(middleOfShankGirth->GetString());
	js["middleOfShankLevel"].SetString(middleOfShankLevel->GetString());
	js["aboveAnkleGirth"].SetString(aboveAnkleGirth->GetString());
	js["aboveAnkleLevel"].SetString(aboveAnkleLevel->GetString());
	js["overAnkleBoneGirth"].SetString(overAnkleBoneGirth->GetString());
	js["overAnkleBoneLevel"].SetString(overAnkleBoneLevel->GetString());
}
