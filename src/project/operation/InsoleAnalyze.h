///////////////////////////////////////////////////////////////////////////////
// Name               : InsoleAnalyze.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 12.01.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef SRC_PROJECT_OPERATION_INSOLEANALYZE_H_
#define SRC_PROJECT_OPERATION_INSOLEANALYZE_H_

/**\class InsoleAnalyze
 * \code #include "InsoleAnalyze.h"\endcode
 * \ingroup ObjectOperations
 * \brief Analyze the outline an determine the construction-points
 *
 * Determine the construction points A-Z from a given insole.
 *
 * All insole points, except M and H have the correct x, y, and z coordinates.
 * (For interpolation reasons these are zero for M and H.)
 */

#include "Operation.h"

#include "../object/Insole.h"
#include "../ParameterFormula.h"

#include <memory>
class InsoleAnalyze: public Operation {
public:
	InsoleAnalyze();
	virtual ~InsoleAnalyze() = default;

	virtual std::string GetName() const override;
	virtual bool CanRun() override;
	virtual bool Propagate() override;
	virtual bool HasToRun() override;
	virtual void Run() override;

private:
	/**\{
	 * \name Function for UV based operations.
	 */
	Geometry::Vertex Diff(Geometry::Vertex a, const Geometry::Vertex &b) const;
	double Dist(const Geometry::Vertex &a, const Geometry::Vertex &b) const;
	Geometry::Vertex Normal(Geometry::Vertex a) const;
	Polygon3::Intersections Intersect(const Geometry &geo,
			const Geometry::Vertex &a, const Geometry::Vertex &b,
			double relDistance) const;
	void UpdateXYZfromUV(const Geometry &geo, Geometry::Vertex &V);

	/**\}
	 */
public:
	std::shared_ptr<ParameterFormula> footLength;
	std::shared_ptr<ParameterFormula> ballMeasurementAngle;
	std::shared_ptr<ParameterFormula> heelDirectionAngle;
	std::shared_ptr<ParameterFormula> littleToeAngle;
	std::shared_ptr<ParameterFormula> bigToeAngle;
	std::shared_ptr<ParameterFormula> extraLength;

	std::shared_ptr<Insole> in;
	std::shared_ptr<Insole> out;
};

#endif /* SRC_PROJECT_OPERATION_INSOLEANALYZE_H_ */
