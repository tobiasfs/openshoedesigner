///////////////////////////////////////////////////////////////////////////////
// Name               : FootMeasurements.h
// Purpose            : Measurements for the foot (and the leg)
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 21.11.2018
// Copyright          : (C) 2018 Tobias Schaefer <tobiassch@users.sourceforge.net>
// License            : GNU General Public License version 3.0 (GPLv3)
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

#ifndef PROJECT_FOOT_FOOTMEASUREMENTS_H
#define PROJECT_FOOT_FOOTMEASUREMENTS_H

/*!\class FootMeasurements
 * \brief Measurements for the foot and the leg
 *
 * This class contains all measurements for the foot and the leg. It is also
 * used to calculate default values for all measurements.
 * In the case that only some basic measurements are recorded, this class uses
 * functions (MathParser) supplied for each measurement to calculate all
 * missing values.
 *
 * \image html "LegMeasurements.svg"
 *
 * \image html "FootMeasurements.svg"
 *
 */

#include "../system/JSON.h"
#include "Parameter.h"
#include "ParameterEvaluator.h"
#include "ParameterFormula.h"

#include <memory>

class FootMeasurements {

public:
	enum class Type {
		EU, //!< European shoe size in Paris Point
		US, //!< US shoe size, Brannok scale
		CN, //!< Chinese shoe size
		UK, //!< United Kingdom shoe size
		JP, //!< Japanese shoe size
		AU, //!< Australian shoe size
		mm, //!< Foot length in mm
		cm, //!< Foot length in cm
		in, //!< Foot length in inch
		ft  //!< Foot length in feet
	};

public:
	FootMeasurements() = default;
	void Register(ParameterEvaluator &evaluator);

	void Modify(bool modified = true);
	bool IsModified() const; //!< Returns \b true if the measurements have been modified.

	void LoadJSON(std::string filename);
	void SaveJSON(std::string filename) const;

	static std::string GetName(int id);
	static bool IsValidID(int id);
	std::shared_ptr<Parameter> GetParameter(int id);
	const std::shared_ptr<const Parameter> GetParameter(int id) const;

	double GetSize(Type type) const;

	void FromJSON(const JSON &js);
	void ToJSON(JSON &js) const;

public:

	/**\brief Leg-length difference
	 *
	 * This value is stored in the Variable legLengthDifference.
	 *
	 * The two additional variables legLengthDifference_L and
	 * legLengthDifference_R contain calculation for the absolute values of the
	 * leg-length difference for both sides. It is up to the writer of the
	 * formulas to use this value in the calculation of the heelHeight and
	 * maybe the ballHeight as well.
	 */
	std::shared_ptr<ParameterFormula> legLengthDifference;

	// Measurements for the foot
	std::shared_ptr<ParameterFormula> footLength;
	std::shared_ptr<ParameterFormula> ballWidth;
	std::shared_ptr<ParameterFormula> bigToeGirth;
	std::shared_ptr<ParameterFormula> littleToeGirth;
	std::shared_ptr<ParameterFormula> waistGirth;
	std::shared_ptr<ParameterFormula> heelGirth;
	std::shared_ptr<ParameterFormula> heelWidth;

	std::shared_ptr<ParameterFormula> angleMixing;

	// Measurements for the leg
	std::shared_ptr<ParameterFormula> belowCrutchGirth;
	std::shared_ptr<ParameterFormula> belowCrutchLevel;
	std::shared_ptr<ParameterFormula> middleOfCalfGirth;
	std::shared_ptr<ParameterFormula> middleOfCalfLevel;
	std::shared_ptr<ParameterFormula> aboveKneeGirth;
	std::shared_ptr<ParameterFormula> aboveKneeLevel;
	std::shared_ptr<ParameterFormula> overKneeCapGirth;
	std::shared_ptr<ParameterFormula> overKneeCapLevel;
	std::shared_ptr<ParameterFormula> belowKneeGirth;
	std::shared_ptr<ParameterFormula> belowKneeLevel;
	std::shared_ptr<ParameterFormula> middleOfShankGirth;
	std::shared_ptr<ParameterFormula> middleOfShankLevel;
	std::shared_ptr<ParameterFormula> aboveAnkleGirth;
	std::shared_ptr<ParameterFormula> aboveAnkleLevel;
	std::shared_ptr<ParameterFormula> overAnkleBoneGirth;
	std::shared_ptr<ParameterFormula> overAnkleBoneLevel;

};

#endif /* PROJECT_FOOT_FOOTMEASUREMENTS_H */
