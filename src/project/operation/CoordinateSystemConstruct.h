///////////////////////////////////////////////////////////////////////////////
// Name               : CoordinateSystemConstruct.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 29.12.2024
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
#ifndef SRC_PROJECT_OPERATION_COORDINATESYSTEMCONSTRUCT_H_
#define SRC_PROJECT_OPERATION_COORDINATESYSTEMCONSTRUCT_H_

/** \class CoordinateSystemConstruct
 * 	\code #include "CoordinateSystemConstruct.h"\endcode
 * 	\ingroup ObjectOperations
 *  \brief Construct a coordinate-system based on an Insole
 *
 * This last is used as the starting point for the upper pattern and the
 * heel/sole generation.
 */

#include "Operation.h"

#include "../../3D/Bezier3.h"
#include "../CoordinateSystem.h"
#include "../object/Insole.h"
#include "../ParameterFormula.h"

#include <memory>
class CoordinateSystemConstruct: public Operation {
public:
	CoordinateSystemConstruct();
	virtual ~CoordinateSystemConstruct() = default;

	virtual std::string GetName() const override;
	virtual bool CanRun() override;
	virtual bool Propagate() override;
	virtual bool HasToRun() override;
	virtual void Run() override;
	virtual void Paint() const override;

public:
	std::shared_ptr<Insole> in;
	std::shared_ptr<CoordinateSystem> out;

	std::shared_ptr<ParameterFormula> belowCrotchGirth;
	std::shared_ptr<ParameterFormula> belowCrotchLevel;
	std::shared_ptr<ParameterFormula> middleOfThighGirth;
	std::shared_ptr<ParameterFormula> middleOfThighLevel;
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

private:

	std::vector<Bezier3> ellipses;
	std::vector<double> ellipsesV;

	/**\brief Calculate the ellipses that model the leg
	 */
	void InitLeg();
};

#endif /* SRC_PROJECT_OPERATION_COORDINATESYSTEMCONSTRUCT_H_ */
