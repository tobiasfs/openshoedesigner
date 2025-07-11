///////////////////////////////////////////////////////////////////////////////
// Name               : Object.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 17.11.2024
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
#ifndef OBJECT_OBJECT_H
#define OBJECT_OBJECT_H

/** \class Object
 * 	\code #include "Object.h"\endcode
 * 	\ingroup ObjectOperations
 *  \brief All objects that are created during the shoe generation
 *
 * All objects generated during the shoe generation are derived from this class.
 *
 * This class is the other half of the Object and Operation system that limits
 * the number of operations done with each modification of the project.
 *
 * Object is derived from last in the class definitions, so that it does not
 * impede the ordering of member variables in memory.
 */

class Object {
public:
	Object() = default;
	virtual ~Object() = default;

	void MarkNeeded(bool needed_);
	bool IsNeeded() const;

	void MarkValid(bool valid_);
	bool IsValid() const;

private:
	bool needed = false;
	bool valid = false;
};

#endif /* OBJECT_OBJECT_H */
