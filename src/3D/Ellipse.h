///////////////////////////////////////////////////////////////////////////////
// Name               : Ellipse.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 03.10.2020
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

#ifndef ELLIPSE_H
#define ELLIPSE_H

/*!\class Ellipse
 * \brief An ellipse
 *
 * This class collects routines for working with ellipses as it turns out, the
 * mathematics of ellipses are very tricky.
 * [Wikipedia: Ellipse](https://en.wikipedia.org/wiki/Ellipse)
 *
 */

#include "AffineTransformMatrix.h"

#include "Polygon3.h"

#include "Surface.h"
#include <cstddef>
#include <vector>

struct Ellipse: public AffineTransformMatrix {
public:
	Ellipse() = default;
	virtual ~Ellipse() = default;

	double Excentricity() const;
	double Circumfence() const;

	/** \brief Steiner construction
	 *
	 * The Steiner construction of an ellipsis uses an approach of two intersecting
	 * lines.
	 * [Wikipedia: Steiner generation](https://en.wikipedia.org/wiki/Ellipse#Steiner_generation)
	 * The analytic solution of this intersection is used to generate (almost)
	 * equidistant points on the ellipsis. The result is returned as an vector
	 * of Ellipse::Point.
	 *
	 * \param nrOfPoints Number of points on the circumfence to generate
	 * \return Geometry with points an edges.
	 */
	Polygon3 SteinerConstruction(size_t nrOfPoints) const;

	Polynomial3 GetSpline(size_t N, size_t n) const;

	void Paint() const;

private:
	static double E(const double e); ///< Complete elliptic integral of the 2nd kind as series evaluation
};

#endif /* ELLIPSE_H */
