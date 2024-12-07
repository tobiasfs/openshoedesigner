///////////////////////////////////////////////////////////////////////////////
// Name               : NURBS.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 26.02.2018
// Copyright          : (C) 2018 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef PROJECT_LAST_NURBS_H
#define PROJECT_LAST_NURBS_H

/*!\class NURBS
 * \brief 3D Nurbs surface
 *
 * Data and weights of the surface are aligned to an even grid.
 *
 * <A href="https://en.wikipedia.org/wiki/Non-uniform_rational_B-spline">Wikipedia: NURBS</A>
 *
 * <A href="https://en.wikipedia.org/wiki/De_Boor%27s_algorithm">Wikipedia: De Boor Algorithm</A>
 *
 */

#include "Vector3.h"

#include <stddef.h>
#include <vector>

class NURBS {
public:
	NURBS();
	virtual ~NURBS();

	void Initialize(size_t nu, size_t nv, bool cyclicU = false, bool cyclicV =
			false, bool normalizeU = false, bool normalizeV = false);
	void InsertData(Vector3 v, double w);

	double GetU(int block, int subBlock = -1);
	double GetV(int block, int subBlock = -1);
	Vector3 Position(double u, double v) const;
	Vector3 Normal(double u, double v) const;

	void Paint(void) const;
	void Paint(int blockU, int blockV) const;

	int controlPointSize; // 0 or negative = do not draw

private:
	size_t nU;
	size_t nV;
	size_t insertpos;

	size_t resolutionU;
	size_t resolutionV;
	size_t subresolutionU;
	size_t subresolutionV;

	std::vector<Vector3> c;
	std::vector<double> w;
	std::vector<double> knotU;
	std::vector<double> knotV;

	bool cyclicU;
	bool cyclicV;
	std::vector<size_t> posU;
	std::vector<size_t> posV;

	// Variables to speed up calculation of subsequent (u,v) positions in Position(u, v).
	// Marked as mutable, because this function could be written without these variables
	// (slower) and are used only in this function.
	mutable size_t lastU; // Last interval calculated in U
	mutable size_t lastV; // Last interval calculated in V
};

#endif /* PROJECT_LAST_NURBS_H */
