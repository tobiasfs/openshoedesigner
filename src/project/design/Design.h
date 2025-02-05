///////////////////////////////////////////////////////////////////////////////
// Name               : Design.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 04.02.2025
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
#ifndef SRC_PROJECT_DESIGN_DESIGN_H_
#define SRC_PROJECT_DESIGN_DESIGN_H_

/** \class Design
 * 	\code #include "Design.h"\endcode
 * 	\ingroup shoe
 *  \brief Design for the upper of a shoe
 *
 * Contains several Patches that make up the design of the upper of the shoe.
 *
 * The coordinate System is cylindric.
 *  * U from -pi to pi, with 0 in front, positive angles to the outside,
 *  	negative angles to the inside of the shoe.
 *  * V height from the sole
 *  	-1 is the floor,
 *  	 0 is the sole,
 *  	 1 the ankle,
 *  	 2 the knee,
 *  	 3 the hip.
 */

#include "../object/Object.h"

#include <vector>
class Design: public Object {

public:
	class Patch {
	public:
		Patch() = default;
		virtual ~Patch() = default;



	};

public:
	Design() = default;
	virtual ~Design() = default;

	std::vector<Patch> patches;

	void Paint();

};

#endif /* SRC_PROJECT_DESIGN_DESIGN_H_ */
