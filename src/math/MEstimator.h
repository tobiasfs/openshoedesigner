///////////////////////////////////////////////////////////////////////////////
// Name               : MEstimator.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 23.07.2020
// Copyright          : (C) 2020 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef MESTIMATOR_H
#define MESTIMATOR_H

/*!\class MEstimator
 * \brief Maximum-likelihood type estimators
 *
 * Estimates the average value of a X/Y-dataset using different estimating
 * functions. The estimators can be applied to ranges in the X/Y-dataset
 *
 * The estimating function are:
 *   * Least squares
 *   * Huber-K
 *   * Hamel
 *   * Andrews wave
 *   * Tukey's biweight
 *
 *   [Wikipedia: M-estimator](https://en.wikipedia.org/wiki/M-estimator)
 *   [Wikipedia: M-Schätzer](https://de.wikipedia.org/wiki/M-Sch%C3%A4tzer)
 */

#include "DependentVector.h"
#include "Kernel.h"

class MEstimator: public DependentVector {
public:
	class Estimator {
	public:
		virtual ~Estimator() = default;
		virtual double Rho(double z) const = 0;
		virtual double Psi(double z) const = 0;
		virtual double W(double z) const = 0;
	};

	struct LeastSquares: public Estimator {
	public:
		double Rho(double z) const;
		double Psi(double z) const;
		double W(double z) const;
	};

	struct HuberK: public Estimator {
	public:
		HuberK(double k = 1.28) :
				k(k) {
		}
		double k;
		double Rho(double z) const;
		double Psi(double z) const;
		double W(double z) const;
	};

	struct Hampel: public Estimator {
	public:
		Hampel(double a = 1.7, double b = 3.4, double c = 8.5) :
				a(a), b(b), c(c) {
		}
		double a;
		double b;
		double c;
		double Rho(double z) const;
		double Psi(double z) const;
		double W(double z) const;
	};

	struct AndrewWave: public Estimator {
	public:
		AndrewWave(double a = 4.2097) :
				a(a) {
		}
		double a;
		double Rho(double z) const;
		double Psi(double z) const;
		double W(double z) const;
	};

	struct TukeysBiweight: public Estimator {
	public:
		TukeysBiweight(double a = 4.685) :
				a(a) {
		}
		double a;
		double Rho(double z) const;
		double Psi(double z) const;
		double W(double z) const;
	};

	void EstimateY(const DependentVector &data, const Estimator &estimator,
			const double sigma, size_t xstart = 0, size_t xend = (size_t) -1,
			std::function<double(double)> weighting = Kernel::Uniform);

	void EstimateX(const DependentVector &data, const Estimator &estimator,
			const double sigma, const double yValue, size_t xstart = 0,
			size_t xend = (size_t) -1);

	std::vector<double> weight;

};

#endif /* MESTIMATOR_H */
