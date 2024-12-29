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

/** \class InsoleConstruct
 * 	\code #include "InsoleConstruct.h"\endcode
 * 	\ingroup ObjectOperations
 *  \brief Construct an insole from the settings given in the foot measurements.
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
 *
 * \htmlonly
 <svg viewBox="0 0 223.22822 609.44859" height="172mm" width="63mm">
 <style>text{font-size:16px;text-align:left;text-anchor:left;fill:black;}</style>
 <style>path{fill:none;stroke:#000000;stroke-width:2px;}</style>
 <g transform="translate(-138.43299,-425.67688)">
 <path d="m 294.30155,932.3458 c -5.57559,53.48992 -27.45774,84.4444 -67.83547,80.8808 -45.5675,-4.0216 -57.9299,-44.78857 -50.0939,-94.96971 34.98584,-224.04661 34.33377,-147.83292 -16.69796,-264.55837 -11.97992,-27.4018 -1.82862,-118.09146 16.69796,-166.45783 7.98644,-20.84979 21.71889,-33.76301 59.4865,-30.78687 54.94055,4.32939 77.78173,78.32744 92.44481,124.96531 7.38453,23.48747 5.53858,82.89892 0.90891,102.49394 -29.87143,126.43028 -25.9534,162.49865 -34.91085,248.43273 z"/>
 <path d="m 176.91083,486.21391 -17.23661,167.48461 169.06689,30.26506 -0.35343,-198.42303 z"/>
 <path d="m 235.33687,455.93201 1.56543,558.33819"/>
 <path d="M 224.37883,1012.7047 266.12374,671.96192"/>
 <path d="m 175.85037,918.7787 118.45118,14.08891"/>
 <text x="235.40424" y="1029.3859">A</text>
 <text x="234.29324" y="452.80115">B</text>
 <text x="238.98955" y="663.09113">C</text>
 <text x="237.96275" y="483.01569">D</text>
 <text x="145.04666" y="665.19519">E</text>
 <text x="332.37695" y="683.81201">F</text>
 <text x="160.76837" y="483.45331">G</text>
 <text x="332.73056" y="482.8978">H</text>
 <text x="242.64223" y="919.30054">J</text>
 <text x="161.77831" y="917.61719">K</text>
 <text x="297.97104" y="932.64868">L</text>
 <text x="267.18417" y="669.35284">M</text>
 <text x="210.82857" y="1028.8809">N</text>
 <text x="332.46115" y="579.65161">Z</text>
 </g></svg>
 * \endhtmlonly
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
	std::shared_ptr<ParameterFormula> extraLength;

	std::shared_ptr<Insole> out;

};

#endif /* SRC_PROJECT_OPERATION_INSOLECONSTRUCT_H_ */
