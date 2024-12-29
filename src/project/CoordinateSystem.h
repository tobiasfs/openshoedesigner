///////////////////////////////////////////////////////////////////////////////
// Name               : CoordinateSystem.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 22.09.2020
// Copyright          : (C) 2020 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_COORDINATESYSTEM_H
#define L3D_COORDINATESYSTEM_H
/*!\class CoordinateSystem
 * \brief UVW coordinate system
 *
 * Surface based coordinate system to project the pattern onto.
 */

#include "../3D/Polygon3.h"
#include "../3D/Polynomial3.h"
#include "../3D/Surface.h"
#include "object/Object.h"

class FootMeasurements;
class Insole;
class LastModel;

class CoordinateSystem: public Surface, public Object {
public:
//	CoordinateSystem() = default;

	void Update();

	void Paint() const;

private:
	Polygon3 inside08;
	Polygon3 outside08;

	Polynomial3 p0;

};

#endif /* L3D_COORDINATESYSTEM_H */

