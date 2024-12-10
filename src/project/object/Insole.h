///////////////////////////////////////////////////////////////////////////////
// Name               : Insole.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.09.2020
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

#ifndef PROJECT_INSOLE_H
#define PROJECT_INSOLE_H

/*!\class Insole
 * \brief Construct, deform and export the insole of a shoe
 * \ingroup ObjectOperations
 *
 *
 * Constructs an insole from the measurements using the methods
 * found in the books:
 *
 *   * Alexander Besching, "Handbuch der Schuhindustrie", 1990, 14th ed., Dr. Alfred Hüthing Verlag GmbH, Heidelberg
 *   * Michael Dees, "Was der Schuhmacher unbedingt von seinem Handwerk wissen muß", 1948, 3rd ed., Gutenberg-Druck, Würzburg-Veitshöchheim
 *
 * I am pretty sure, that Michael Dees read one of the earlier
 * editions of "Handbuch der Schuhindustrie" (back then by Oswald
 * Besching) before writing his own book. The construction of the insole
 * is quite similar but he uses different letters for the points.
 *
 *
 * \htmlonly
 <svg viewBox="0 0 223.22822 609.44859" height="172mm" width="63mm">
 <style>text{font-size:16px;text-align:left;text-anchor:left;fill:black;}</style>
 <style>path{fill:none;stroke:#000000;stroke-width:2px;}</style>
 <g transform="translate(-138.43299,-425.67688)">
 <path d="m 294.30155,932.3458 c -5.57559,53.48992 -27.45774,84.4444 -67.83547,80.8808 -45.5675,-4.0216 -57.9299,-44.78857 -50.0939,-94.96971 34.98584,-224.04661 34.33377,-147.83292 -16.69796,-264.55837 -11.97992,-27.4018 -1.82862,-118.09146 16.69796,-166.45783 7.98644,-20.84979 21.71889,-33.76301 59.4865,-30.78687 54.94055,4.32939 77.78173,78.32744 92.44481,124.96531 7.38453,23.48747 5.53858,82.89892 0.90891,102.49394 -29.87143,126.43028 -25.9534,162.49865 -34.91085,248.43273 z"/>
 <path d="m 176.91083,486.21391 -17.23661,167.48461 169.06689,30.26506 -0.35343,-198.42303 z"/>
 <path d="m 235.33687,455.93201 1.56543,558.33819"/>
 <path d="M 224.37883,1012.7047 266.12374,671.96192"/>
 <path d="m 175.85037,918.7787 118.45118,14.08891"/>
 <text x="235.40424" y="1029.3859">A</text>
 <text x="234.29324" y="452.80115">B</text>
 <text x="238.98955" y="663.09113">C</text>
 <text x="237.96275" y="483.01569">D</text>
 <text x="145.04666" y="665.19519">E</text>
 <text x="332.37695" y="683.81201">F</text>
 <text x="160.76837" y="483.45331">G</text>
 <text x="332.73056" y="482.8978">H</text>
 <text x="242.64223" y="919.30054">J</text>
 <text x="161.77831" y="917.61719">K</text>
 <text x="297.97104" y="932.64868">L</text>
 <text x="267.18417" y="669.35284">M</text>
 <text x="210.82857" y="1028.8809">N</text>
 <text x="332.46115" y="579.65161">Z</text>
 </g></svg>
 * \endhtmlonly
 *
 * The heel is normally oriented in the N-M axis.
 *
 * The coordinate system of the upper in the heel section can be oriented in
 * the N-M axis (e.g. Lloyd shoes, Apollo shoes) or in the A-C axis (e.g. Ecco
 * shoes).
 */

#include <functional>
#include <vector>

#include "../../3D/Geometry.h"
#include "../../3D/Polygon3.h"
#include "../../3D/Polynomial3.h"
#include "../../3D/Vector3.h"
#include "../../math/Polynomial.h"
#include "Object.h"

class Insole: public Geometry, public Object {
	friend class InsoleConstruct;
	friend class InsoleTransform;

protected:
	struct Point {
	public:
		Point() = default;
		Point(Vector3 p, double r = 0.0) :
				p(p), r(r) {
		}
		Vector3 p;
		Vector3 n;
		double r = 0.0;
		void SetNormal(const Point &p0, const Point &p1);
		void Transform(std::function<Vector3(Vector3)> func);
	};

	class Line: public Polynomial3 {
	public:
		// Bezier-Circle: sqrt(2) * f = 0.551915024494 --> f = 0.39
		void Setup(const Point &p0, const Point &p1, double f0 = 0.39,
				double f1 = 0.39);
		void Paint() const;
		bool IsInside(const double r) const;
	};

public:
	void Transform(std::function<Vector3(Vector3)> func);
	void Paint() const;

public:
	std::vector<Line> lines;
	std::vector<Point> outline;
	Polygon3 inside;
	Polygon3 outside;

protected:
	Insole::Point A; ///< Heel
	Insole::Point B; ///< Shoe tip
	Insole::Point C; ///< Insole center (middle of ball area)
	Insole::Point D; ///< Center of toes
	Insole::Point E; ///< Big toe ball
	Insole::Point F; ///< Little toe ball
	Insole::Point G; ///< Big toe tip
	Insole::Point H; ///< Little toe direction
	Insole::Point J; ///< Center point (below os tibia)
	Insole::Point K; ///< Heel half circle inside
	Insole::Point L; ///< Heel half circle outside
	Insole::Point N; ///< Middle of heel half-circle
	Insole::Point Z; ///< Little toe tip
};

#endif /* PROJECT_INSOLE_H */
