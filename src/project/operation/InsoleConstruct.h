///////////////////////////////////////////////////////////////////////////////
// Name               : InsoleConstruct.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 12.11.2024
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
#ifndef SRC_PROJECT_OPERATION_INSOLECONSTRUCT_H_
#define SRC_PROJECT_OPERATION_INSOLECONSTRUCT_H_

/** \class InsoleConstruct
 * 	\code #include "InsoleConstruct.h"\endcode
 * 	\ingroup GroupName
 *  \brief Description
 *
 * Text
 */

#include "Operation.h"

#include "../FootMeasurements.h"
#include "../Insole.h"
class InsoleConstruct: public Operation {
public:
	InsoleConstruct() = default;
	virtual ~InsoleConstruct() = default;

	virtual bool CanRun() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

private:
	void Construct();
	void FinishConstruction(const size_t N);

	double RatX(const double x, const bool yPositive) const;
	double RatY(const double y, const bool xPositive) const;

public:

	std::shared_ptr<ParameterFormula> footLength;

	std::shared_ptr<ParameterFormula> ballMeasurementAngle;
	std::shared_ptr<ParameterFormula> heelDirectionAngle;
	std::shared_ptr<ParameterFormula> littleToeAngle;
	std::shared_ptr<ParameterFormula> bigToeAngle;
	std::shared_ptr<ParameterFormula> ballWidth;
	std::shared_ptr<ParameterFormula> heelWidth;

	std::shared_ptr<ParameterFormula> extraLength;

	std::shared_ptr<FootMeasurements> measurements;
	std::shared_ptr<Insole> insole;



};

#endif /* SRC_PROJECT_OPERATION_INSOLECONSTRUCT_H_ */
