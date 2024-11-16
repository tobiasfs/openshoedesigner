///////////////////////////////////////////////////////////////////////////////
// Name               : Operation.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 09.11.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef PROJECT_OPERATION_H
#define PROJECT_OPERATION_H

/** \class Operation
 * 	\ingroup project
 * 	\code #include "Operation.h"\endcode
 *  \brief Abstract class for operations that have to run to generate the shoe
 *
 *  The operations can be scheduled to run in parallel.
 *
 */

class Operation {
public:
	Operation() = default;
	virtual ~Operation() = default;

	virtual bool HasToRun() = 0;
	virtual bool CanRun() = 0;
	virtual void Run() = 0;
};

#endif /* PROJECT_OPERATION_H */
