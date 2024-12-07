///////////////////////////////////////////////////////////////////////////////
// Name               : FormFinder.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 31.05.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef FORMFINDER_H
#define FORMFINDER_H
/*!\class FormFinder
 * \brief Finds forms in a Polygon3
 *
 * This is used to identify shapes.
 */

#include "PolyFilter.h"
#include "../3D/Vector3.h"

#include <stddef.h>
#include <vector>

class Polygon3;

class FormFinder {
public:
	FormFinder() = default;
	virtual ~FormFinder() = default;

	void AddPolygon(const Polygon3 &poly, size_t Nsections);

	void Paint(void) const;

	std::vector<Vector3> a;
	std::vector<Vector3> b;
	std::vector<Vector3> c;
	std::vector<Vector3> d;

	std::vector<double> bend;

	PolyFilter pf;
};

#endif /* FORMFINDER_H */
