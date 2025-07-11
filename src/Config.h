///////////////////////////////////////////////////////////////////////////////
// Name               : Config.h
// Purpose            : Global configuration
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 21.02.2010
// Copyright          : (C) 2010 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

/*  Configuration file. Major switches that change the code
 *  and the structure can be found here (_DEBUG_ for example).
 */

#ifndef CONFIG_H
#define CONFIG_H

#define OPENSHOEDESIGNER_VERSION "0.2"
#define OPENSHOEDESIGNER_AUTHORS "Tobias Schaefer"

#ifdef DEBUG
#define DEBUGOUT std::cout
#else
#define DEBUGOUT if(false) std::cout
#endif

//#define USE_PORTMIDI
//#define USE_GLAD
#define USE_UNIT
#define USE_3DPICKING
//#define USE_6DOFCONTROLLER
//#define USE_MULTITHREADING

#endif /* CONFIG_H */
