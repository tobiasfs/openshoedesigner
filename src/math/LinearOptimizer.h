///////////////////////////////////////////////////////////////////////////////
// Name               : LinearOptimizer.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 13.09.2020
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

#ifndef MATH_LINEAROPTIMIZER_H
#define MATH_LINEAROPTIMIZER_H

/*!\class LinearOptimizer
 * \brief Linear / line-search optimizer
 */

#include "OptimizerAbstract.h"

#include <vector>
#include <stddef.h>

class LinearOptimizer: public OptimizerAbstract {
public:
	LinearOptimizer() = default;
	virtual ~LinearOptimizer() = default;

	std::vector<double> param; //!< Parameter of the system
	double spread = 0.1; //!< Spread for testing. Default: 0.1

	void SetFirstTest(double xFirst);

	void Start() override;; //!< Setup the internal variables and start an optimization
	bool IsRunning() override;; //!< Loop control function for the main loop
	void SetError(double error) override;; //!< Insert the error back into the solver
	void Stop() override;; //!< Optional: Stops the optimization prematurely and copies the best result so far into 'param'.

private:
	size_t state = 0;

	double x0 = 0.0;
	double x1 = 0.0;
	double y0 = 0.0;
	double y1 = 0.0;
};

#endif /* MATH_LINEAROPTIMIZER_H */

