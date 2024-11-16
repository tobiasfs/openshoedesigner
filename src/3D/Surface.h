///////////////////////////////////////////////////////////////////////////////
// Name               : Surface.h
// Purpose            : Quadratic surface
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 05.10.2023
// Copyright          : (C) 2023 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_SURFACE_H
#define L3D_SURFACE_H

/*!\class Surface
 * \brief Surface description and coordinate system from quadratic patches
 *
 * # Points in a patch:
 *
 * \htmlonly
 * <svg width="92" height="85"><g transform ="translate(0,-967.32306)">
 * <path d="m 19.189407,1029.9748 47.973516,10.7729 1.851608,-60.09305 -50.83509,8.75305 z"  stroke="black"  stroke-width="2" fill="none" />
 * <text x="70" y="1049">1</text><text x="72" y="984.5">2</text><text x="6" y="990">3</text><text x="6" y="1042">0</text>
 * </g></svg>
 * \endhtmlonly
 *
 */

//fx(u,v):=cx0 + cx1*u + cx2*u^2 + cx3*u^3 + (cx4 + cx5*u + cx6*u^2 + cx7*u^3)*v + (cx8 + cx9*u + cx10*u^2 + cx11*u^3)*v^2 + (cx12 + cx13*u + cx14*u^2 + cx15*u^3)*v^3;
//fy(u,v):=cy0 + cy1*u + cy2*u^2 + cy3*u^3 + (cy4 + cy5*u + cy6*u^2 + cy7*u^3)*v + (cy8 + cy9*u + cy10*u^2 + cy11*u^3)*v^2 + (cy12 + cy13*u + cy14*u^2 + cy15*u^3)*v^3;
//fz(u,v):=cz0 + cz1*u + cz2*u^2 + cz3*u^3 + (cz4 + cz5*u + cz6*u^2 + cz7*u^3)*v + (cz8 + cz9*u + cz10*u^2 + cz11*u^3)*v^2 + (cz12 + cz13*u + cz14*u^2 + cz15*u^3)*v^3;

#include <array>
#include <cstdint>
#include <stddef.h>
#include <vector>

class Dependencies;

class Surface {
public:
	struct Point {
		double x = 0.0;
		double y = 0.0;
		double z = 0.0;
		double u = 0.0;
		double v = 0.0;
	};
	struct Vector {
		double x = 0.0;
		double y = 0.0;
		double z = 0.0;
		double nx = 0.0;
		double ny = 0.0;
		double nz = 0.0;
	};
	class Patch {
	public:
		Patch() = default;
		void Paint() const;
		Vector operator()(double u, double v) const;

		void CalcNormalMatrices();

		double MaxBend() const;

		std::array<double, 16> cx;
		std::array<double, 16> cy;
		std::array<double, 16> cz;
		std::array<double, 36> cnx;
		std::array<double, 36> cny;
		std::array<double, 36> cnz;
		double u0 = 0.0;
		double u1 = 1.0;
		double v0 = 0.0;
		double v1 = 1.0;
	};

public:
	Surface() = default;
	virtual ~Surface() = default;

	void Calculate(const std::vector<Surface::Point> &p);

	void Paint() const;

	void AddConnection(Dependencies &dep, uint_fast8_t N, size_t idx1,
			size_t idx2, double u, double v);

	std::vector<Patch> patches;

	double eps = 1e-9;

	std::vector<Point> debug;

};

#endif /* L3D_SURFACE_H */
