///////////////////////////////////////////////////////////////////////////////
// Name               : Bender.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 14.07.2020
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

#ifndef L3D_BENDER_H
#define L3D_BENDER_H

/*!\class Bender
 * \brief Create a geometry bending transformation
 *
 * This class generates a bending function. It uses four coordinate systems:
 *
 * from0, from1, to0 and to1 to describe the desired bending operation.
 *
 * \htmlonly
 <svg height="32mm" width="86mm"><g transform="translate(0,-938.9764)">
 <path d="m 165.88585,997.57487 5.20553,14.38343 22.67187,-8.9724 c 0,0 20.8154,-12.85073 43.0156,-4.31252 26.40231,10.15432 31.57334,11.22962 31.57334,11.22962 l 21.02764,6.0276 3.97236,-13.9723 -21.02764,-6.02767 c 0,0 -3.7952,-1.19909 -31.64067,-10.34017 -28.50723,-9.35833 -52.94828,3.90147 -52.94828,3.90147 z"
 style="fill:#ff9100;fill-opacity:1;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 14.379838,980.93064 0,15 114.999992,0 0.20553,-14.86179 z"
 style="fill:#ff9100;fill-opacity:1;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <text y="1016.8805" x="54.783722"
 style="font-style:normal;font-weight:normal;font-size:20px;line-height:125%;font-family:sans-serif;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" xml:space="preserve">
 <tspan y="1016.8805" x="54.783722">x</tspan></text>
 <path d="m 39.379845,1035.9306 0,-64.99996"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 34.379845,975.93064 5,-5 5,5"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 6.5262968,1003.0771 64.9999902,0"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 66.526287,998.07708 5,5.00002 -5,5"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <text y="961.56281" x="5.072228"
 style="font-style:normal;font-weight:normal;font-size:20px;line-height:125%;font-family:sans-serif;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" xml:space="preserve">
 <tspan y="961.56281" x="5.072228">from0</tspan></text>
 <text y="962.17584" x="81.640656"
 style="font-style:normal;font-weight:normal;font-size:20px;line-height:125%;font-family:sans-serif;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" xml:space="preserve">
 <tspan y="962.17584" x="81.640656">from1</tspan></text>
 <text y="967.62299" x="176.30269"
 style="font-style:normal;font-weight:normal;font-size:20px;line-height:125%;font-family:sans-serif;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" xml:space="preserve">
 <tspan y="967.62299" x="176.30269">to0</tspan></text>
 <text y="967.87665" x="255.00443"
 style="font-style:normal;font-weight:normal;font-size:20px;line-height:125%;font-family:sans-serif;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" xml:space="preserve">
 <tspan y="967.87665" x="255.00443">to1</tspan></text>
 <text y="1017.0079" x="121.15866"
 style="font-style:normal;font-weight:normal;font-size:20px;line-height:125%;font-family:sans-serif;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" xml:space="preserve">
 <tspan y="1017.0079" x="121.15866">x</tspan></text>
 <path d="m 105.75478,1036.0581 0,-64.99994"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 100.75478,976.05816 5,-5 5,5"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 72.901235,1003.2046 64.999985,0"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 132.90122,998.20465 5,4.99995 -5,5"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <text transform="matrix(0.93337911,-0.35889196,0.35889196,0.93337911,0,0)" y="1025.832" x="-164.12384"
 style="font-style:normal;font-weight:normal;font-size:20px;line-height:125%;font-family:sans-serif;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" xml:space="preserve">
 <tspan y="1025.832" x="-164.12384">x</tspan></text>
 <path d="M 207.43238,1039.7022 184.10441,979.03258"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 181.23198,985.49394 2.87243,-6.46136 6.46136,2.87244"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 164.9767,1020.8283 60.66963,-23.328"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 219.18497,994.62786 6.46136,2.87244 -2.87244,6.4614"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <text transform="matrix(0.97097353,0.23918696,-0.23918696,0.97097353,0,0)" y="937.85669" x="517.75031"
 style="font-style:normal;font-weight:normal;font-size:20px;line-height:125%;font-family:sans-serif;letter-spacing:0px;word-spacing:0px;fill:#000000;fill-opacity:1;stroke:none;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" xml:space="preserve">
 <tspan y="937.85669" x="517.75031">x</tspan></text>
 <path d="m 258.88546,1049.2859 15.54715,-63.11319"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 268.3818,989.83164 6.05081,-3.65893 3.65893,6.0508"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 234.84366,1009.528 63.11327,15.5471"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 <path d="m 294.29799,1019.0243 3.65894,6.0508 -6.05081,3.6589"
 style="fill:none;fill-rule:evenodd;stroke:#000000;stroke-width:1px;stroke-linecap:butt;stroke-linejoin:miter;stroke-opacity:1" />
 </g>/svg>
 * \endhtmlonly
 *
 * from0 and from1 describe the starting pair of the transformation.
 *
 * to0 and to1 the end of it.
 *
 * Halfspaces are always with respect to the x coordninate.
 *
 * All points on the negative halfspace of from0 are transformed into the respective coordinates
 * of the negative halfspace of to0.
 *
 * All points in the positive halfspace of from0 and in the negative halfspace of from1 are bend
 * and reshaped in between both target halfspaces to0 and to1.
 *
 * All points on the positive halfspace of from1 are transformed into their respective coordinates
 * in the positive halfspace of to1.
 *
 */

#include <utility>
#include "AffineTransformMatrix.h"
#include "Vector3.h"

class Bender {
public:
	Bender() = default;

	void Calculate(); //!< Calculates all internal variables. Has to be called after setting up the coordinate systems.

	Vector3 operator()(const Vector3 & v) const; //!< Transforms a Vector3

	AffineTransformMatrix from0;
	AffineTransformMatrix from1;
	AffineTransformMatrix to0;
	AffineTransformMatrix to1;

private:

	struct Rotation {
		bool parallel;
		Vector3 p;
		Vector3 n;
		double R;
	};

	AffineTransformMatrix diff0;
	AffineTransformMatrix diff1;
	AffineTransformMatrix inv0;
	AffineTransformMatrix inv1;

	AffineTransformMatrix nullf0;
	AffineTransformMatrix nullf1;
	AffineTransformMatrix nullf0inv;
	AffineTransformMatrix nullf1inv;

	AffineTransformMatrix nullt0;
	AffineTransformMatrix nullt1;
	AffineTransformMatrix nullt0inv;
	AffineTransformMatrix nullt1inv;

	double Lfrom = 1.0;
	double Lto = 1.0;

	bool parallelf = false;
	bool parallelt = false;

	static Rotation RotationalAxis(const AffineTransformMatrix & a,
			const AffineTransformMatrix & b);

};

#endif /* L3D_BENDER_H */
