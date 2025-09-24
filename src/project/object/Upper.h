///////////////////////////////////////////////////////////////////////////////
// Name               : Upper.h
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

#ifndef OBJECT_UPPER_H
#define OBJECT_UPPER_H

/*!\class Upper
 * \brief Upper material of a shoe
 *
 * \todo Can this be replaced by a Design object directly?
 */

#include "Object.h"
#include "../../3D/Geometry.h"

#include <vector>

class Upper: public Object {
public:
	Upper();
	virtual ~Upper() = default;


	std::vector<Geometry> patches;
};

#endif /* OBJECT_UPPER_H */
