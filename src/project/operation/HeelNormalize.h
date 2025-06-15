///////////////////////////////////////////////////////////////////////////////
// Name               : HeelNormalize.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 26.12.2024
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
#ifndef SRC_PROJECT_OPERATION_HEELNORMALIZE_H_
#define SRC_PROJECT_OPERATION_HEELNORMALIZE_H_

/** \class HeelNormalize
 *  \brief Normalize the positon of a heel/sole loaded from a file.
 * 	\ingroup ObjectOperations
 * 	\code #include "HeelNormalize.h"\endcode
 *
 * This operation reorients the heel so that the very sole of the heel is at
 * Z = 0. The final orientation of the heel is done by analysing the insole
 * at the upper part of the heel.
 */

#include "../object/ObjectGeometry.h"
#include "../ParameterEnum.h"
#include "Operation.h"

#include <memory>

class HeelNormalize: public Operation {
public:
	HeelNormalize();
	virtual ~HeelNormalize() = default;

	virtual std::string GetName() const override;
	virtual bool CanRun() override;
	virtual bool Propagate() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

public:
	std::shared_ptr<ParameterEnum> heelReorient; //< Allow the rotation of the geometry

	std::shared_ptr<ObjectGeometry> in;
	std::shared_ptr<ObjectGeometry> out;
};

#endif /* SRC_PROJECT_OPERATION_HEELNORMALIZE_H_ */
