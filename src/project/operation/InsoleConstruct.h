///////////////////////////////////////////////////////////////////////////////
// Name               : InsoleConstruct.h
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
#ifndef SRC_PROJECT_OPERATION_INSOLECONSTRUCT_H_
#define SRC_PROJECT_OPERATION_INSOLECONSTRUCT_H_

/**\class InsoleConstruct
 * \brief Construct an insole from the settings given in the foot measurements.
 * \ingroup ObjectOperations
 * \code #include "InsoleConstruct.h"\endcode
 *
 * Constructs the Insole object using the measurements of the foot and the
 * general configuration for the shoe.
 *
 *
 * Constructs an insole from the measurements using the methods
 * found in the books:
 *
 *   * Alexander Besching, "Handbuch der Schuhindustrie", 1990, 14th ed., Dr. Alfred Hüthing Verlag GmbH, Heidelberg
 *   * Michael Dees, "Was der Schuhmacher unbedingt von seinem Handwerk wissen muß", 1948, 3rd ed., Gutenberg-Druck, Würzburg-Veitshöchheim
 *
 * I am pretty sure, that Michael Dees read one of the earlier
 * editions of "Handbuch der Schuhindustrie" (back then by Oswald
 * Besching) before writing his own book. The construction of the insole
 * is quite similar but he uses different letters for the points.
 *
 * \image html Insole.svg
 *
 */

#include "../FootMeasurements.h"
#include "../object/Insole.h"
#include "../ParameterFormula.h"
#include "Operation.h"

#include <memory>
class InsoleConstruct: public Operation {
public:
	InsoleConstruct();
	virtual ~InsoleConstruct() = default;

	virtual std::string GetName() const override;
	virtual bool CanRun() override;
	virtual bool Propagate() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

private:
	/**\brief Calculate the construction points A-Z
	 *
	 * Calculate the points using the construction method mentioned above.
	 * A set of splines is constructed in between the points.
	 *
	 * Finally the FinishConstruction() command is called to interpolate the
	 * splines into edges in the geometry.
	 */
	void Construct();

	/**\brief Interpolate the Bezier-polynomials
	 *
	 * \param N Number of steps for each line to interpolate in between.
	 */
	void FinishConstruction(const size_t N);

	double RatX(const double x, const bool yPositive) const;
	double RatY(const double y, const bool xPositive) const;

public:

	std::shared_ptr<ParameterFormula> footLength;
	std::shared_ptr<ParameterFormula> ballWidth;
	std::shared_ptr<ParameterFormula> heelWidth;

	std::shared_ptr<ParameterFormula> ballMeasurementAngle;
	std::shared_ptr<ParameterFormula> heelDirectionAngle;
	std::shared_ptr<ParameterFormula> littleToeAngle;
	std::shared_ptr<ParameterFormula> bigToeAngle;
	std::shared_ptr<ParameterFormula> tipSharpness;
	std::shared_ptr<ParameterFormula> extraLength;

	std::shared_ptr<Insole> out;

};

#endif /* SRC_PROJECT_OPERATION_INSOLECONSTRUCT_H_ */
