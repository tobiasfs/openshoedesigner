///////////////////////////////////////////////////////////////////////////////
// Name               : InsoleTransform.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 12.11.2024
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
#ifndef SRC_PROJECT_OPERATION_INSOLETRANSFORM_H_
#define SRC_PROJECT_OPERATION_INSOLETRANSFORM_H_

/** \class InsoleTransform
 * 	\code #include "InsoleTransform.h"\endcode
 * 	\ingroup GroupName
 *  \brief Description
 *
 * Text
 */

#include "Operation.h"

#include "../Configuration.h"
#include "../Insole.h"

#include <memory>
class InsoleTransform: public Operation {
public:
	InsoleTransform() = default;
	virtual ~InsoleTransform() = default;

	virtual bool CanRun() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

private:
	void Shape();

public:

	std::shared_ptr<ParameterFormula> heelPitch;
	std::shared_ptr<ParameterFormula> toeSpring;
	std::shared_ptr<ParameterFormula> heelHeight;
	std::shared_ptr<ParameterFormula> ballHeight;
	std::shared_ptr<ParameterFormula> legLengthDifference;

	std::shared_ptr<Configuration> config;
	std::shared_ptr<Insole> in;
	std::shared_ptr<Insole> out;
};

#endif /* SRC_PROJECT_OPERATION_INSOLETRANSFORM_H_ */
