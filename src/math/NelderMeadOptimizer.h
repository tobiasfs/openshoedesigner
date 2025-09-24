///////////////////////////////////////////////////////////////////////////////
// Name               : NelderMeadOptimizer.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 11.02.2018
// Copyright          : (C) 2018 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef MATH_NELDERMEADOPTIMIZER_H
#define MATH_NELDERMEADOPTIMIZER_H

/*!\class NelderMeadOptimizer
 * \brief Nelder-Mead Optimizer
 *
 * This class encapsulates the Nelder Mead Optimization method for
 * multi-dimensional optimization.
 *
 * https://en.wikipedia.org/wiki/Nelder%E2%80%93Mead_method
 *
 * The class is used by
 *   - generate an object of the class
 *   - fill-in the param std::vector with the start parameter set.
 *   - tune the settings for the optimizer itself (alpha, gamma, rho, sigma, errorLimit, evalLimit, reevalBest, simplexSpread)
 *   - call StartRun to make the optimizer setup its internal variables
 *   - run the main loop \code{.cpp} while(optim.IsRunning()){ ... } \endcode
 *   - in the main loop use the param array values to calculate some error
 *   - return the error to the optimizer with \code{.cpp} optim.SetError(...)\endcode
 *
 * After the main loop has run the param vector contains the optimal result.
 *
 * The main loop can be stopped by calling .Stop() inside the loop.
 *
 * ~~~~~~~~~~~~~~~{.cpp}
 * NelderMeadOptimizer optim;
 *
 * // Insert the startup parameter-set
 * optim.param.push_back(0.123);
 *
 * optim.Start();
 * while(optim.IsRunning()){
 *
 *    double x = optim.param[0];
 *    double f = x * x;
 *
 *    optim.SetError(f);
 * }
 *
 * printf("Optimum: x = %g\n", optim.param[0]);
 * ~~~~~~~~~~~~~~~
 *
 * The main loop hidden in IsRunning and SetError is a really complicated state
 * machine. This orchestrates all evaluations needed to form the simplex and
 * move it around the search-space.
 *
 * See also CMAESOptimizer.
 */

#include "OptimizerAbstract.h"

#include <cstddef>
#include <vector>

class NelderMeadOptimizer: public OptimizerAbstract {
public:
	NelderMeadOptimizer() = default;
	virtual ~NelderMeadOptimizer() = default;

	std::vector<double> param; //!< Parameter of the system

	double alpha = 1; //!< Reflection coefficient (alpha > 0 ), default: 1
	double gamma = 2; //!< Expansion coefficient (gamma > 1), default: 2
	double rho = 0.5; //!< Contraction coefficient (rho > 0 and rho <= 0.5), default: 0.5
	double sigma = 0.5; //!< Shrink coefficient (sigma > 0 and sigma < 1), default: 0.5

	double simplexSpread = 0.1; //!< Spread for setting up the simplex. Default: 0.1

	bool keepSimplex = false; //!< Speed up continued optimizations, if the system changes only a little. Default: false
	bool reevalBest = false; //!< After the optimization has finished, do an evaluation on the best result. Default: false

	void Start() override; //!< Setup the internal variables and start an optimization
	bool IsRunning() override; //!< Loop control function for the main loop
	void SetError(double error) override; //!< Insert the error back into the solver
	void Stop() override; //!< Optional: Stops the optimization prematurely and copies the best result so far into 'param'.

private:
	bool simplexIsSetup = false; //!< Internal variable to indicate that the simplex has been set up.
	size_t N = 0; //!< Number of parameter
	size_t M = 0; //!< Number of corners in simplex
	std::vector<double> simplex;
	std::vector<double> f;
	std::vector<double> xo;
	double fxo = 0.0;
	std::vector<double> xr;
	double fxr = 0.0;
	std::vector<double> xe;
	double fxe = 0.0;
	std::vector<double> xc;
	double fxc = 0.0;
	unsigned int state = 0;
	size_t index = 0;
};

#endif /* MATH_NELDERMEADOPTIMIZER_H */

