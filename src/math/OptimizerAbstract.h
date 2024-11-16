///////////////////////////////////////////////////////////////////////////////
// Name               : OptimizerAbstract.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 31.12.2023
// Copyright          : (C) 2023 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef OPTIMIZERABSTRACT_H
#define OPTIMIZERABSTRACT_H

/*!\class OptimizerAbstract
 * \brief Abstract interface class for the optimizers.
 *
 * The optimizer used are from the class of derivative free optimizers.
 *
 * https://en.wikipedia.org/wiki/Derivative-free_optimization
 *
 * No inside into the function to be optimized is needed here as
 *
 * This class describes the abstract interface, used by all optimizer classes.
 *
 * The implemented optimizers are:
 *  - Nelder-Mead optimizer
 *  - CMAES optimizer
 *  - Linear optimizer
 *  - MCS optimizer
 *  - https://en.wikipedia.org/wiki/Powell%27s_method
 *
 * Each class is used in the same manner:
 *
 *   - generate an object of the class
 *   - fill-in the param std::vector with the start parameter set.
 *   - tune the settings for the optimizer itself (differs from class to class)
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
 * ...classname... optim;
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
 * printf("Minimum: x = %g\n", optim.param[0]);
 * ~~~~~~~~~~~~~~~
 *
 * The main loop hidden in IsRunning and SetError is a complicated state
 * machine. This orchestrates all evaluations needed during the optimization
 * process.
 *
 * See also NelderMeadOptimizer, CMAESOptimizer, LinearOptimizer.
 */

#include <stddef.h>

class OptimizerAbstract {
public:
	OptimizerAbstract() = default;
	virtual ~OptimizerAbstract() = default;

	double errorLimit = 1e-3; //!< Stop optimization if error is less than errorLimit. Default: 1e-3
	size_t evalLimit = 100; //!< Stop optimization if the loop has run more than evalLimit times. Default: 100

	virtual void Start()=0; //!< Setup the internal variables and start an optimization
	virtual bool IsRunning()=0; //!< Loop control function for the main loop
	virtual void SetError(double error)=0; //!< Insert the error back into the solver
	virtual void Stop()=0; //!< Optional: Stops the optimization prematurely and copies the best result so far into 'param'.

	size_t EvaluationsDone() const; //!< Number of evaluations of the loop
	double ResidualError() const; //!< Error of the returned result

protected:
	size_t evaluationCount = 0;
	double lastError = 0.0;
};

#endif /* OPTIMIZERABSTRACT_H */
