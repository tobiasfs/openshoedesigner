///////////////////////////////////////////////////////////////////////////////
// Name               : FootScanLoad.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 24.12.2024
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
#ifndef SRC_PROJECT_OPERATION_FOOTSCANLOAD_H_
#define SRC_PROJECT_OPERATION_FOOTSCANLOAD_H_

/** \class FootScanLoad
 * 	\code #include "FootScanLoad.h"\endcode
 * 	\ingroup GroupName
 *  \brief Description
 *
 * Text
 */

#include "Operation.h"

#include "../foot/FootModel.h"
#include "../ParameterString.h"

#include <memory>
#include <filesystem>

class FootScanLoad: public Operation {
public:
	FootScanLoad();
	bool CanRun() override;
	bool HasToRun() override;
	std::string GetName() const override;
	void Run() override;
	bool Propagate() override;

public:
	std::shared_ptr<ParameterString> filename;
	std::shared_ptr<FootModel> out;

private:
	std::filesystem::file_time_type lastModified;
};

#endif /* SRC_PROJECT_OPERATION_FOOTSCANLOAD_H_ */
