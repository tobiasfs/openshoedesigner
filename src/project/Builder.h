///////////////////////////////////////////////////////////////////////////////
// Name               : Builder.h
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
#ifndef SRC_PROJECT_BUILDER_H_
#define SRC_PROJECT_BUILDER_H_
/** \class Builder
 * 	\code #include "Builder.h"\endcode
 * 	\ingroup project
 *  \brief Setup and run the process-chain for the shoe generation.
 *
 * Reads the configuration and other relevant data from the project and
 * generates a chain of operations to produce all necessary output objects.
 *
 * The generated operations are sorted and executed in the correct sequence
 * and only if necessary (by tracking all the modified-flags).
 *
 *
 *
 */

#include "operation/Operation.h"

#include <memory>
#include <vector>

class Project;

class Builder {
public:
	Builder() = default;
	virtual ~Builder() = default;

	void Update(Project &project);

	void Paint() const;

public:
	std::string error;
private:
	void Setup(Project &project);

	std::vector<std::shared_ptr<Operation>> operations;
};

#endif /* SRC_PROJECT_BUILDER_H_ */
