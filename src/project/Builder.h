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

#include "operation/FootModelLoad.h"
#include "operation/FootScanLoad.h"
#include "operation/InsoleConstruct.h"
#include "operation/InsoleTransform.h"
#include "operation/LastAnalyse.h"
#include "operation/LastNormalize.h"
#include "operation/ObjectLoad.h"
#include "operation/Operation.h"

#include "operation/FootModelUpdate.h"
#include <memory>
#include <vector>

class Project;

class Builder {
public:
	Builder() = default;
	virtual ~Builder() = default;

	bool IsSetup() const;
	void Setup(Project &project);
	void Update(Project &project);

	void Paint() const;

private:
	void Connect(Project &project);

public:
	std::string error;

private:
	std::vector<std::shared_ptr<Operation>> operations;

	std::shared_ptr<FootModelLoad> opFootModelLoad;
	std::shared_ptr<FootModelUpdate> opFootModelUpdate;
	std::shared_ptr<FootScanLoad> opFootScanLoad;
	std::shared_ptr<InsoleConstruct> opInsoleConstruct;
	std::shared_ptr<InsoleTransform> opInsoleTransform;
	std::shared_ptr<LastAnalyse> opAnalyse;
	std::shared_ptr<LastNormalize> opNormalize;
	std::shared_ptr<ObjectLoad> opHeelLoad;
	std::shared_ptr<ObjectLoad> opLastLoad;

};

#endif /* SRC_PROJECT_BUILDER_H_ */
