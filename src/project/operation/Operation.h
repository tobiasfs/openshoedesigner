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
 * 	\ingroup ObjectOperations
 * 	\code #include "Operation.h"\endcode
 *  \brief Abstract class for operations that have to run to generate the shoe
 *
 *  The operations can be scheduled to run in parallel.
 */

class Operation {
public:
	Operation() = default;
	virtual ~Operation() = default;

	/**\brief Checking (mostly) if all inputs and all outputs are connected.
	 *
	 * Mostly a check, if the setup of this operations was correct and
	 * everything is connected correctly.
	 *
	 * For some operations also other checks (like "Does a file exist on
	 * drive?") are done.
	 *
	 * \return bool, if all was correct.
	 */
	virtual bool CanRun() = 0;

	/**\brief Propagate the "needed" and "valid" flags.
	 *
	 * The valid-flags are originated from some settings or measurements.
	 * The flags are, propagated forwards through the chain. Each object based
	 * on an modified setting is invalidated and needs to be rebuild if needed.
	 *
	 * The needed-flags are propagated backwards, the needed-flags are
	 * propagated forwards.
	 *
	 * \return bool, if something was propagated. This is used by the Builder
	 * 		   to know when enough calls to Propagate() have been done.
	 */
	virtual bool Propagate() = 0;

	/**\brief Does this operation have to run?
	 *
	 * Checks if the output is invalid and needed and the input is valid or a
	 * Parameter.
	 *
	 * Parameter have no valid flag, they have a modified flag. The resetting
	 * of the Parameter has to be done after a complete build run.
	 *
	 * \return bool, if the operations has to (and can) run right now.
	 */
	virtual bool HasToRun() = 0;

	/**\brief Run the operation
	 *
	 * Run the operation and set the output to valid afterwards. Thus the
	 * operations doen the chain can be run.
	 */
	virtual void Run() = 0;

#ifdef DEBUG
	virtual void Paint() const;
#endif
};

#ifdef DEBUG
#define DEBUGOUT std::cout
#else
#define DEBUGOUT if(false) std::cout
#endif

#endif /* PROJECT_OPERATION_H */
