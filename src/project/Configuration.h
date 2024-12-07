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
	Configuration() = default;
	virtual ~Configuration() = default;

	void Register(ParameterEvaluator &evaluator_);

	bool IsModified() const; //!< Returns \b true if the measurements have been modified since last update.
	void Modify(bool modified = true);

	static bool IsValidID(int id);
	static std::string GetName(int id);
	std::shared_ptr<Parameter> GetParameter(int id);
	const std::shared_ptr<const Parameter> GetParameter(int id) const;

	void FromJSON(const JSON &js);
	void ToJSON(JSON &js) const;

public:

	// Measurements

	//TODO Convert to ParameterEnum
	bool measurementsSymmetric = true;
	//TODO Convert to ParameterEnum
	enum class MeasurementSource {
		fromMeasurements, fromFootScan
	} measurementsource = MeasurementSource::fromMeasurements;

	std::shared_ptr<PointCloud> footScan;

	// Last generator

	std::shared_ptr<ParameterString>lastFilename;
	std::shared_ptr<ParameterEnum>modelType;


	// Insole construction

	std::shared_ptr<ParameterFormula> bigToeAngle;
	std::shared_ptr<ParameterFormula> littleToeAngle;
	std::shared_ptr<ParameterFormula> ballMeasurementAngle;
	std::shared_ptr<ParameterFormula> heelDirectionAngle;

	std::shared_ptr<ParameterFormula> upperLevel;
	std::shared_ptr<ParameterFormula> extraLength;
	std::shared_ptr<ParameterFormula> footCompression;

	// Shoe generator

	//TODO Convert to ParameterEnum
	enum class Generator {
		Experimental, //!< Default generator for development of algorithms
		Welted, //!< Welt-sewn shoes: Generates last, insole, sole, upper pattern and cutout
		Cemented, //!< for cemented soles (simple, glued-together shoes)
		Molded, //!< for industrial shoes, where the sole is injection-molded to the upper
		Dutch, //!< Generator for dutch wooden clogs: Generates last, insole and clog
		Geta   //!< Japanese Geta generator
	} generator = Generator::Welted;

	std::shared_ptr<ParameterFormula> heelHeight;
	std::shared_ptr<ParameterFormula> ballHeight;
	std::shared_ptr<ParameterFormula> heelPitch;
	std::shared_ptr<ParameterFormula> toeSpring;

	std::shared_ptr<ParameterFormula> tipSharpness;
};

#endif /* SRC_PROJECT_CONFIGURATION_H_ */
