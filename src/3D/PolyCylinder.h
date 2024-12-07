///////////////////////////////////////////////////////////////////////////////
// Name               : PolyCylinder.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 12.02.2019
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

#ifndef POLYCYLINDER_H
#define POLYCYLINDER_H

/*!\class PolyCylinder
 * \brief DEPRECATED: Stores sections of polygonwise defined shapes
 * \deprecated Used to import from a single data-file. Largely unused.
 *
 * Useless file format, that should not be used. Use stl instead.
 * Was a good idea back then, but it is easier to model a last based
 * on some imaged from the internet in Blender from scratch.
 *
 * Maybe there is some last-scanning system the produces a format that can be
 * interpreted by this class...
 */

#include <string>
#include <vector>

#include "Geometry.h"
#include "Vector3.h"
#include "../math/Polynomial.h"

class PolyCylinder {
public:
	class CircleSection {
	public:
		class Segment {
		public:
			Polynomial y0;
			Polynomial z0;
			Polynomial y;
			Polynomial z;
			Polynomial dy;
			Polynomial dz;
			Segment() = default;
			void Scale(double sy = 1.0, double sz = 1.0);
			void Paint(void) const;
			double GetLength(void) const;
		};
		std::vector<Segment> segments;
		CircleSection() = default;
		void Scale(double sy = 1.0, double sz = 1.0);
		Vector3 Evaluate(double r) const;
		double GetLength(void) const;
		void Paint(void) const;
	};
	std::vector<CircleSection> sections;
	double dx = 1.0;
	PolyCylinder() = default;

	void Scale(double sx, double sy = 1.0, double sz = 1.0);

	void Load(std::string filename);

	void Paint(void) const;
	Geometry GenerateGeometry(bool mirrored = false) const;
	// void Test(void);

	// TODO: Write the following function, if this class is really used:
	// void GenerateHull(Geometry &hull, bool mirrored = false);
};

#endif /* POLYCYLINDER_H */
