///////////////////////////////////////////////////////////////////////////////
// Name               : DesignSolve.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 18.03.2026
// Copyright          : (C) 2026 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef SRC_PROJECT_OPERATION_DESIGNSOLVE_H_
#define SRC_PROJECT_OPERATION_DESIGNSOLVE_H_

/*!\class DesignSolve
 * \brief ...
 *
 * ...
 */

#include "../../3D/Surface.h"
#include "../CoordinateSystem.h"
#include "../Design.h"
#include "../object/DesignSolution.h"
#include "../object/ObjectGeometry.h"
#include "../object/Upper.h"
#include "Operation.h"

#include <memory>

class DesignSolve: public Operation {
public:
	DesignSolve();
	virtual ~DesignSolve() = default;

	virtual std::string GetName() const override;
	virtual bool CanRun() override;
	virtual bool Propagate() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

	/**\brief Use the edges to fill the patches with a grid of points.
	 */
	void UpdatePatches(double res, const Polynomial &scaleU =
			Polynomial::ByValue(-M_PI, -0.28, M_PI, 0.28),
			const Polynomial &scaleV = Polynomial::ByValue(0, 0, 1, 0.12));

public:
	std::shared_ptr<Design> design;
	std::shared_ptr<DesignSolution> out;
};

#endif /* SRC_PROJECT_OPERATION_DESIGNSOLVE_H_ */
