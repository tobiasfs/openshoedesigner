///////////////////////////////////////////////////////////////////////////////
// Name               : CoordinateSystem.cpp
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

#include "CoordinateSystem.h"

//#include <iostream>

//#include "../math/Ellipse.h"
//#include "foot/FootMeasurements.h"
//#include "Insole.h"
//#include "last/LastModel.h"

void CoordinateSystem::Update() {

//	inside08 = insole.inside;
//	outside08 = insole.outside;
//
//	for(size_t n = 0; n < inside08.Size(); ++n){
//		const double h = last.height08((double) n / (double) inside08.Size());
//		inside08[n] += inside08.Normal(n) * h;
//	}
//	for(size_t n = 0; n < outside08.Size(); ++n){
//		const double h = last.height08((double) n / (double) outside08.Size());
//		outside08[n] += outside08.Normal(n) * h;
//	}

//	SetSize(3, 3);
//	P(0, 0).p.Set(0, 0, 0.1);
//	P(1, 0).p.Set(1, 0, 0.4);
//	P(2, 0).p.Set(2, 0, -0.3);
//	P(0, 1).p.Set(0, 1, 0.3);
//	P(1, 1).p.Set(1, 1, 0.7);
//	P(2, 1).p.Set(2, 1, 0.4);
//	P(0, 2).p.Set(0, 2, 0.1);
//	P(1, 2).p.Set(1, 2, 0.5);
//	P(2, 2).p.Set(2, 2, 0.1);
//	CalculateDirections();
//	Calculate();

//	p0 = Polynom3::ByPolygon3(0.01, 3, 0, 25);

}

void CoordinateSystem::Paint() const {
//	inside08.Paint(0);
//	outside08.Paint(0);

	Paint();
//	p0.Paint();
//	Ellipse e;
//	e.w = 0.1;
//	e.h = 0.05;
//	e.Paint();
}
