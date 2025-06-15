///////////////////////////////////////////////////////////////////////////////
// Name               : LastUpdate.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 27.12.2024
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
#ifndef SRC_PROJECT_OPERATION_LASTUPDATE_H_
#define SRC_PROJECT_OPERATION_LASTUPDATE_H_

/**\class LastUpdate
 * \brief Update last
 * \ingroup ObjectOperations
 * \code #include "LastUpdate.h"\endcode
 *
 * This last is the rescaled and adapted to the foot measurements and the
 * shoe form.
 */

#include "../../math/NelderMeadOptimizer.h"
#include "../object/LastModel.h"
#include "../ParameterEnum.h"
#include "../ParameterFormula.h"
#include "Operation.h"

#include <memory>

class LastUpdate: public Operation {
public:
	LastUpdate();
	virtual ~LastUpdate() = default;

	virtual std::string GetName() const override;
	virtual bool CanRun() override;
	virtual bool Propagate() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

public:
	std::shared_ptr<ParameterEnum> lastModify;

	std::shared_ptr<ParameterFormula> heelPitch;
	std::shared_ptr<ParameterFormula> toeSpring;
	std::shared_ptr<ParameterFormula> heelHeight;
	std::shared_ptr<ParameterFormula> ballHeight;
	std::shared_ptr<ParameterFormula> legLengthDifference;

	std::shared_ptr<LastModel> in;
	std::shared_ptr<LastModel> out;

protected:
	NelderMeadOptimizer optiPos;

};

#endif /* SRC_PROJECT_OPERATION_LASTUPDATE_H_ */
