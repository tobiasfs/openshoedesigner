///////////////////////////////////////////////////////////////////////////////
// Name               : LastConstruct.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 12.01.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef SRC_PROJECT_OPERATION_LASTCONSTRUCT_H_
#define SRC_PROJECT_OPERATION_LASTCONSTRUCT_H_

/**\class LastConstruct
 * \brief Construct last
 * \ingroup ObjectOperations
 * \code #include "LastConstruct.h"\endcode
 *
 * In the current implementation, the last is constructed from the coordinate
 * system and the insole as the lower cap of the coordinate system. The last
 * is cut off at the ankle.
 */

#include "../CoordinateSystem.h"
#include "../object/Insole.h"
#include "../object/ObjectGeometry.h"
#include "Operation.h"

#include <memory>

class LastConstruct: public Operation {
public:
	LastConstruct();
	virtual ~LastConstruct() = default;

	virtual std::string GetName() const override;
	virtual bool CanRun() override;
	virtual bool Propagate() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

public:
	std::shared_ptr<Insole> insole;
	std::shared_ptr<CoordinateSystem> cs;
	std::shared_ptr<ObjectGeometry> out;
};

#endif /* SRC_PROJECT_OPERATION_LASTCONSTRUCT_H_ */
