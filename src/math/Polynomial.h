///////////////////////////////////////////////////////////////////////////////
// Name               : Polynomial.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 28.01.2017
// Copyright          : (C) 2017 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef MATH_POLYNOMIAL_H
#define MATH_POLYNOMIAL_H

/*!\class Polynomial
 * \brief Evaluateable polynomial with an arbitrary number of coefficients
 *
 * This class contains a polynomial of arbitrary order.
 *
 * The coefficients are stored Octave/Matlab style: The first coefficient is
 * the one for the highest power.
 *
 * e.g. \f[
 * 	f(r) = c_3\cdot r^3 + c_2\cdot r^2 + c_1 \cdot r + c_0
 * \f]
 * The vector returned by the stream output << can be used directly in the
 * Octave/Matlab polyval command. Note that the order of coefficient is inverted
 * here.
 *
 * \htmlonly
 * <svg width="300" height="200">
 * <path transform="translate(0,-852.35945)" d="m 280,1032.3594 -15,5 0,-10 15,5 -260,0 -2e-6,-160.00001 4.999999,15 -9.999999,0 5,-15 c 0,0 61.281556,160.00001 129.999982,160.00001 68.71843,0 129.99999,-160.00001 129.99999,-160.00001"  stroke="black"  stroke-width="2" fill="none" />
 * </svg>
 * \endhtmlonly
 */

#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <tuple>
#include <vector>

class Polynomial {
public:

	/**\name Static functions for constructions
	 *
	 * \note Bezier ranges for r are always from 0 to 1.
	 *
	 * \{
	 */

	static Polynomial Identity();
	static Polynomial ByValue(double r0, double v0); ///< Initialize by 1 point with position and value (actually only the value is used, because it is a constant)
	static Polynomial ByValue(double r0, double v0, double r1, double v1); ///< Initialize by 2 points with position and value
	static Polynomial ByValue(double r0, double v0, double r1, double v1,
			double r2, double v2); ///< Initialize by 3 points with position and value
	static Polynomial ByValue(double r0, double v0, double r1, double v1,
			double r2, double v2, double r3, double v3); ///< Initialize by 4 points with position and value

	static Polynomial ByDerivative(double r0, double v0, double dv0); ///< Initialize by 1 point with position, value and derivative
	static Polynomial ByDerivative(double r0, double v0, double dv0, double r1,
			double v1); ///< Initialize by a point with position, value and derivative and a second point only with position and value
	static Polynomial ByDerivative(double r0, double v0, double dv0, double r1,
			double v1, double dv1); ///< Initialize by two points with position, value and derivative

	static Polynomial ByBezier(double v0); ///< Bezier interpolation with constant value
	static Polynomial ByBezier(double v0, double v1); ///< Bezier interpolation with start- and endvalue
	static Polynomial ByBezier(double v0, double v1, double v2); ///< Bezier interpolation with start-, support- and end-value
	static Polynomial ByBezier(double v0, double v1, double v2, double v3); ///< Bezier interpolation with two support values
	static Polynomial ByBezier(const std::vector<double> &v); ///< Bezier interpolation with one to four support values

	static Polynomial ByIntegrals(const std::vector<double> &integrals,
			double a, double b);
	static Polynomial ByVectorDerivates(const std::vector<double> &r,
			const std::vector<double> &v, size_t order, size_t idxStart = 0,
			size_t idxEnd = (size_t) -1);

	/*! \brief Fit a polynomial to vector vector of positions values
	 *
	 * \param r Running variable
	 * \param v Dependent variable
	 * \param order Order of generated polynomial
	 * \param idxStart (optional) Starting index in the vectors
	 * \param idxEnd (optional) Ending index in the vectors
	 */
	static Polynomial ByVector(const std::vector<double> &r,
			const std::vector<double> &v, size_t order, size_t idxStart = 0,
			size_t idxEnd = (size_t) -1);

	/*! \brief Bernstein polynomial
	 *
	 * Bernstein polynomials are a set of N polynomials with the index 0 .. N-1, that
	 * add up to 1 at each point in the interval 0..1.
	 *
	 * For N = 1 there is only one polynomial with the index of 0. This polynomial is
	 * a constant 1.0.
	 *
	 * For N = 2 there are two polynomials, indexed 0 and 1 the first is a linear
	 * ramp from 1.0 to 0.0, the second one is a ramp from 0.0 to 1.0.
	 *
	 * For N = 3 there are three polynomials (0..2) The first is a downward
	 * parabola from 1.0 to 0.0, the last one is the reverse parabola from
	 * 0.0 to 1.0. The middle there is another parabola centered around 0.5
	 * scaled such as the sum of all three parabola is 1.0 for every x.
	 *
	 * and so on...
	 *
	 * The Bernstein polynomials are the generalization of a
	 * Bezier-interpolation.
	 *
	 * For exact interpolation see Lagrange().
	 *
	 * \param N Number of polynomials in total.
	 * \param index Index from 0 to N-1 for the polynomial from the set.
	 * \return Polynomial of order N-1 with N coefficients.
	 */
	static Polynomial Bernstein(const size_t N, const size_t index);

	/*!\brief Lagrange polynomial
	 *
	 * Lagrange polynomials are used for exact interpolation through N points.
	 * Contrary to the Bezier or Bernstein polynomials, where the points
	 * between the end points are control points resulting in super smooth
	 * interpolation without overswinging, Lagrange polynomials emphasize exact
	 * interpolation of the given points.
	 *
	 * As with the Bernstein polynomials there are N polynomials indexed from
	 * 0 to N-1. The polynomial interpolates in the range 0 to 1.
	 *
	 * Each polynomial is 1 for x = index/(N-1) for all other position of
	 * control points it is 0. In between the control points the polynomial
	 * swings.
	 *
	 * \param N Number of polynomials in total.
	 * \param index Index from 0 to N-1 for the polynomial from the set.
	 * \return Polynomial of order N-1 with N coefficients.
	 */
	static Polynomial Lagrange(const size_t N, const size_t index);

	/**\}\name Basic operations
	 * \{
	 */

	explicit Polynomial(size_t N = 0);
	Polynomial(const std::initializer_list<double> coefficients);

	size_t Size() const; ///< Number of coefficients
	size_t Order() const; ///< Order of the polynomial (= number of coefficients - 1)

	/*! \brief Change the number of coefficients
	 *
	 * If the number of coefficients is increased, zeros are appended to the
	 * vector of coefficients. Appending zeros does not change the form of the
	 * polygon. On a decrease of the coefficients, the extra coefficients are
	 * simply cut off. The form of the polygon may change a lot during this
	 * operation. If a reduction is necessary while keeping the polygon fit as
	 * close as possible to the original one, use the Reduce() function.
	 *
	 * \param N New coefficient count
	 */
	void Resize(size_t N);

	/*! \brief Generate a polynomial of reduced order
	 *
	 * Reduces the polynomial to N coefficients, but tries to fit the
	 * reduced polynomial as close as possible to the original in the range
	 * from a to b.
	 *
	 * If N is greater than the current coefficient-count, zeros are added for
	 * the additional higher powers.
	 *
	 * \param N New coefficient count
	 * \param a Lower-limit of the range to fit the polynomial
	 * \param b Upper-limit of the range to fit the polynomial
	 */
	Polynomial Reduce(size_t N, double a, double b) const;

	double& operator[](size_t index); ///< Access the coefficients
	double operator[](size_t index) const; ///< Const access the coefficients

	double operator()(double x) const; ///< Evaluation of polygon

	std::vector<double> GetBezier() const; ///< Returns the Bezier-values for a polynomial of up to order 3 (= 4 coefficients)

	/**\}\name Arithmetics
	 *
	 * Addition, subtraction, multiplication and division of polynomial by a
	 * double value or a polynomial.
	 * \{
	 */

	Polynomial& operator+=(const double &b);
	Polynomial& operator+=(const Polynomial &b);

	Polynomial& operator-=(const double &b);
	Polynomial& operator-=(const Polynomial &b);

	Polynomial& operator*=(const double &b);
	Polynomial& operator*=(const Polynomial &b);

	Polynomial& operator/=(const double &b);

	friend Polynomial operator+(Polynomial a, const double &b) {
		a += b;
		return a;
	}
	friend Polynomial operator+(Polynomial a, const Polynomial &b) {
		a += b;
		return a;
	}
	friend Polynomial operator-(Polynomial a, const double &b) {
		a -= b;
		return a;
	}
	friend Polynomial operator-(Polynomial a, const Polynomial &b) {
		a -= b;
		return a;
	}
	friend Polynomial operator*(Polynomial a, const double &b) {
		a *= b;
		return a;
	}
	friend Polynomial operator*(Polynomial a, const Polynomial &b) {
		a *= b;
		return a;
	}
	friend Polynomial operator/(Polynomial a, const double &b) {
		a /= b;
		return a;
	}

	/**\brief Polynomial division with quotient and remainder
	 *
	 * Usage:
	 *
	 * ~~~~~~~~~~~~~~~{.cpp}
	 * Polynomial a {...};
	 * Polynomial b {...};
	 * Polynomial q {...};
	 * Polynomial r {...};
	 *
	 * // From C++ 11 on:
	 * std::tie(q, r) = a / b;
	 *
	 * // From C++ 17 on:
	 * auto [q, r] = a / b;
	 * ~~~~~~~~~~~~~~~
	 */

	std::tuple<Polynomial, Polynomial> operator/(const Polynomial &b) const;

	/**\}\name Manipulations
	 * \{
	 */
	void ScaleX(double val); ///< Increase the width of the graph by a factor of val
	void ScaleY(double val); ///< Increase the height of the graph by a factor of val
	void ShiftX(double val); ///< Shift the graph right by val
	void ShiftY(double val); ///< Shift the graph up by val

	Polynomial Derivative(size_t order = 1) const; ///< Return the derivative without changing the polynomial
	void Derive(size_t order = 1); ///< Derive the polynomial itself

	double Integral(double a, double b) const; ///< Evaluate the integral of the polynomial between a and b
	Polynomial Integral(size_t order = 1) const; ///< Return the integral of the polynomial without changing it itself
	void Integrate(size_t order = 1); ///< Integrate the polynomial. (The integration-constant has to be added using the operator '+' afterwards.)

	/**\brief Invert a polynomial in a given range
	 *
	 * Inverts the polynomial. This can only be approximated. (Except for the
	 * linear case, which is exact.)
	 *
	 * An approximating polynomial f-1(x) is returned.
	 *
	 * \note Not satisfyingly working for N > 2 right now.
	 *
	 * \param a Lower limit of range
	 * \param b Upper Limit of range
	 * \param N (optional) Change the number of coefficient for the approximation
	 * \return Approximated inverted polynomial
	 */
	Polynomial Inverse(double a = 0.0, double b = 1.0,
			size_t N = (size_t) -1) const;

	void Invert(double a = 0.0, double b = 1.0, size_t N = (size_t) -1);

	/**\}\name Analysis
	 * \{
	 */

	double ExtremumPos() const; ///< Return the positive extremum, if there is any (up to an order of 3 (4 coefficients))
	double ExtremumNeg() const; ///< Return the negative extremum, if there is any (up to an order of 3 (4 coefficients))
	double InflectionPoint() const; ///< Return the inflection point (only working for an order of exactly 3 (4 coefficients))

	double FindZero(const double xStart = 0.0,
			const size_t maxSteps = 10) const; ///< From a given starting-point the function searches for the value where the Polygon evaluates to 0.

//	std::vector<double> FindRoots() const;

	/**\}
	 */

	/*! \brief Output coefficients in Octave/Matlab format
	 *
	 *~~~~~
	 * Polynomial p = Polynomial::ByValue(-1,1, 0,0, 1,1);
	 * std::cout << p << "\n";
	 *~~~~~
	 */
	friend std::ostream& operator<<(std::ostream &os, const Polynomial &p) {
		os << "[";
		for (size_t n = 0; n < p.Size(); ++n) {
			if (n > 0)
				os << ", ";
			os << p[p.Size() - n - 1];
		}
		os << "]";
		return os;
	}

private:
	std::vector<double> c;
};

#endif /* MATH_POLYNOM_H */

