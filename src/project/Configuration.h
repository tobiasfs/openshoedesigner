///////////////////////////////////////////////////////////////////////////////
// Name               : Configuration.h
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
#ifndef SRC_PROJECT_CONFIGURATION_H_
#define SRC_PROJECT_CONFIGURATION_H_

/** \class Configuration
 * 	\code #include "Configuration.h"\endcode
 * 	\ingroup project
 *  \brief Definition of a shoe
 *
 * Objects of this class keep a list of all the parameters describing
 * the bones. Furthermore the skin surrounding the bones is describes.
 * The third group of parameters describes the angles of the bones, thus
 * the position of the foot.
 *
 * The foots position is not part of the dataset, but has to be kept here,
 * because the functions generating the last need these informations as
 * well.
 */

#include "../3D/PointCloud.h"
#include "../system/JSON.h"
#include "FootMeasurements.h"
#include "ParameterEnum.h"
#include "ParameterEvaluator.h"
#include "ParameterFormula.h"
#include "ParameterString.h"

#include <memory>

class Configuration {
public:
	Configuration();
	virtual ~Configuration() = default;

	void Register(ParameterEvaluator &evaluator);

	bool IsModified() const; //!< Returns \b true if the measurements have been modified since last update.
	void Modify(bool modified = true);

	static bool IsValidID(int id);
	static std::string GetName(int id);
	std::shared_ptr<ParameterFormula> GetParameter(int id);
	const std::shared_ptr<const ParameterFormula> GetParameter(int id) const;

	void FromJSON(const JSON &js);
	void ToJSON(JSON &js) const;

public:

	// Measurements

	std::shared_ptr<ParameterEnum> measurementSource;
	std::shared_ptr<PointCloud> footScan;

	// Last generator

	std::shared_ptr<ParameterString> lastFilename;
	std::shared_ptr<ParameterEnum> modelType;

	// Insole construction

	std::shared_ptr<ParameterFormula> bigToeAngle;
	std::shared_ptr<ParameterFormula> littleToeAngle;
	std::shared_ptr<ParameterFormula> ballMeasurementAngle;
	std::shared_ptr<ParameterFormula> heelDirectionAngle;

	std::shared_ptr<ParameterFormula> upperLevel;
	std::shared_ptr<ParameterFormula> extraLength;
	std::shared_ptr<ParameterFormula> footCompression;

	// Shoe generator
	std::shared_ptr<ParameterEnum> generator;

	std::shared_ptr<ParameterFormula> heelHeight;
	std::shared_ptr<ParameterFormula> ballHeight;
	std::shared_ptr<ParameterFormula> heelPitch;
	std::shared_ptr<ParameterFormula> toeSpring;

	std::shared_ptr<ParameterFormula> tipSharpness;
};

#endif /* SRC_PROJECT_CONFIGURATION_H_ */
