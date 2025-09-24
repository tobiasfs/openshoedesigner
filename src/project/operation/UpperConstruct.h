///////////////////////////////////////////////////////////////////////////////
// Name               : UpperConstruct.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 23.06.2025
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

#ifndef OPERATION_UPPERCONSTRUCT_H_
#define OPERATION_UPPERCONSTRUCT_H_

/*!\class UpperConstruct
 * \brief Construct the shoe-upper from a design
 *
 * ...
 */

#include "Operation.h"
#include "../../3D/Surface.h"
#include "../CoordinateSystem.h"
#include "../object/Design.h"
#include "../object/ObjectGeometry.h"
#include "../object/Upper.h"

#include <memory>
class UpperConstruct: public Operation {
public:
	UpperConstruct();
	virtual ~UpperConstruct() = default;

	virtual std::string GetName() const override;
	virtual bool CanRun() override;
	virtual bool Propagate() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

public:
	std::shared_ptr<Design> design_in;
	std::shared_ptr<CoordinateSystem> cs_in;
	std::shared_ptr<Upper> out;

};

#endif /* OPERATION_UPPERCONSTRUCT_H_ */
