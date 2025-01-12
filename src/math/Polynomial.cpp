///////////////////////////////////////////////////////////////////////////////
// Name               : Polynomial.cpp
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

#include "Polynomial.h"

#include <cfloat>
#include <cmath>
#include <limits>
#include <sstream>
#include <stdexcept>

// Most formulas are calculated with the FRICAS computer algebra system.

Polynomial Polynomial::ByValue(double r0, double v0) {
	Polynomial temp;
	(void) (r0); // Suppresses "Unused parameter" warning.
	temp.c.assign(1, v0);
	return temp;
}

Polynomial Polynomial::ByValue(double r0, double v0, double r1, double v1) {
	Polynomial temp;
	const double T2 = r1 - r0;
	if (fabs(T2) < DBL_EPSILON)
		throw(std::domain_error(
		__FILE__ ": Polynomial(r0,v0,r1,v1) - Both positions have the same r."));
	temp.c.resize(2);
	temp.c[1] = ((-r0 * v1) + r1 * v0) / T2;
	temp.c[0] = (v1 - v0) / T2;
	return temp;
}

Polynomial Polynomial::ByValue(double r0, double v0, double r1, double v1,
		double r2, double v2) {
	Polynomial temp;
	const double T2 = r0 * r0;
	const double T3 = r2 * r2;
	const double T4 = r1 * r1;
	const double T5 = r0 * T4;
	const double T6 = -T2 * r1;
	const double T7 = -T4 + T2;
	const double T8 = (r1 - r0) * T3 + T7 * r2 + T5 + T6;
	if (fabs(T8) < DBL_EPSILON)
		throw(std::domain_error(
				__FILE__ ": Polynomial(r0,v0,r1,v1,r2,v2) - Some positions have the same r."));
	const double T9 = r1 - r0;
	temp.c.resize(3);
	temp.c[2] = ((T5 + T6) * v2 + ((-r0 * T3) + T2 * r2) * v1
			+ (r1 * T3 + (-T4 * r2)) * v0) / T8;
	temp.c[1] = (T7 * v2 + (T3 - T2) * v1 + (-T3 + T4) * v0) / T8;
	temp.c[0] = (T9 * v2 + (-r2 + r0) * v1 + (r2 - r1) * v0) / T8;
	return temp;
}

Polynomial Polynomial::ByValue(double r0, double v0, double r1, double v1,
		double r2, double v2, double r3, double v3) {
	Polynomial temp;
	const double T2 = r0 * r0;
	const double T4 = r0 * T2;
	const double T5 = r1 * r1;
	const double T3 = r1 * T5;
	const double T6 = r0 * T5;
	const double T7 = T2 * r1;
	const double T8 = r0 * T3;
	const double T9 = T4 * r1;
	const double T10 = T2 * T3;
	const double T11 = T4 * T5;
	const double T12 = r2 * r2;
	const double T14 = r2 * T12;
	const double T15 = r3 * r3;
	const double T13 = r3 * T15;
	const double T16 = (-T6 + T7) * T14;
	const double T17 = (T8 - T9) * T12;
	const double T18 = (-T10 + T11) * r2;
	const double T19 = (T5 - T2) * T14 + (-T3 + T4) * T12 + T10 - T11;
	const double T20 = -T5 + T2;
	const double T21 = T3 - T4;
	const double T22 = T2 * T14;
	const double T23 = T4 * T12;
	const double T24 = T5 * T14;
	const double T25 = T3 * T12;
	const double T26 = ((r1 - r0) * T12 + T20 * r2 + T6 - T7) * T13
			+ ((-r1 + r0) * T14 + T21 * r2 - T8 + T9) * T15 + T19 * r3 + T16
			+ T17 + T18;
	if (fabs(T26) < DBL_EPSILON)
		throw(std::domain_error(
				__FILE__ ": Polynomial(r0,v0,r1,v1,r2,v2,r3,v3) - Some positions have the same r."));
	const double T27 = (-r1 + r0) * T14 + T21 * r2 - T8 + T9;
	const double T28 = r1 - r0;
	const double T29 = -T3 + T4;
	const double T30 = r0 * T14;
	const double T31 = T4 * r2;
	const double T32 = r1 * T14;
	const double T33 = T3 * r2;
	const double T34 = T28 * T12 + T20 * r2 + T6 - T7;
	const double T35 = -r1 + r0;
	const double T36 = T5 - T2;
	const double T37 = r0 * T12;
	const double T38 = T2 * r2;
	const double T39 = r1 * T12;
	const double T40 = T5 * r2;
	temp.c.resize(4);
	temp.c[3] = ((T16 + T17 + T18) * v3
			+ ((T6 - T7) * T13 + (-T8 + T9) * T15 + (T10 - T11) * r3) * v2
			+ ((-T37 + T38) * T13 + (T30 - T31) * T15 + (-T22 + T23) * r3) * v1
			+ ((T39 - T40) * T13 + (-T32 + T33) * T15 + (T24 - T25) * r3) * v0)
			/ T26;
	temp.c[2] = (T19 * v3 + (T20 * T13 + T21 * T15 - T10 + T11) * v2
			+ ((T12 - T2) * T13 + (-T14 + T4) * T15 + T22 - T23) * v1
			+ ((-T12 + T5) * T13 + (T14 - T3) * T15 - T24 + T25) * v0) / T26;
	temp.c[1] = (T27 * v3 + (T28 * T13 + T29 * r3 + T8 - T9) * v2
			+ ((-r2 + r0) * T13 + (T14 - T4) * r3 - T30 + T31) * v1
			+ ((r2 - r1) * T13 + (-T14 + T3) * r3 + T32 - T33) * v0) / T26;
	temp.c[0] = (T34 * v3 + (T35 * T15 + T36 * r3 - T6 + T7) * v2
			+ ((r2 - r0) * T15 + (-T12 + T2) * r3 + T37 - T38) * v1
			+ ((-r2 + r1) * T15 + (T12 - T5) * r3 - T39 + T40) * v0) / T26;
	return temp;
}

Polynomial Polynomial::ByDerivative(double r0, double v0, double dv0) {
	Polynomial temp;
	temp.c.resize(2);
	temp.c[1] = v0 + (-dv0 * r0);
	temp.c[0] = dv0;
	return temp;
}

Polynomial Polynomial::ByDerivative(double r0, double v0, double dv0, double r1,
		double v1) {
	Polynomial temp;
	const double T2 = r1 * r1;
	const double T3 = r0 * r0;
	const double T4 = -2 * r0 * r1;
	const double T5 = T2 + T4 + T3;
	if (fabs(T5) < DBL_EPSILON)
		throw(std::domain_error(
				__FILE__ ": InitByDerivative(r0,v0,dv0,r1,v1) - Both positions have the same r."));
	temp.c.resize(3);
	temp.c[2] = (T3 * v1 + (T2 + T4) * v0 + (-dv0 * r0 * T2) + dv0 * T3 * r1)
			/ T5;
	temp.c[1] = ((-2 * r0 * v1) + 2 * r0 * v0 + dv0 * T2 + (-dv0 * T3)) / T5;
	temp.c[0] = (v1 - v0 + (-dv0 * r1) + dv0 * r0) / T5;
	return temp;
}

Polynomial Polynomial::ByDerivative(double r0, double v0, double dv0, double r1,
		double v1, double dv1) {
	const double T3 = r0 * r0;
	const double T4 = r1 * r1;
	const double T2 = r1 * T4;
	const double T5 = r0 * T3;
	const double T6 = -3 * r0 * T4;
	const double T7 = 3 * T3 * r1;
	const double T8 = T2 + T6 + T7 - T5;
	if (fabs(T8) < DBL_EPSILON)
		throw(std::domain_error(
				__FILE__ ": InitByDerivative(r0,v0,dv0,r1,v1,dv1) - Both positions have the same r."));
	const double T9 = 3 * r1;
	const double T10 = 3 * r0;
	const double T11 = -dv1 + (-2 * dv0);
	const double T12 = -dv1 + dv0;
	const double T13 = 2 * dv1 + dv0;
	Polynomial temp(4);
	temp.c[3] = ((T7 - T5) * v1 + (T2 + T6) * v0 + (-dv0 * r0 * T2)
			+ T12 * T3 * T4 + dv1 * T5 * r1) / T8;
	temp.c[2] = ((-6 * r0 * r1 * v1) + 6 * r0 * r1 * v0 + dv0 * T2
			+ T13 * r0 * T4 + T11 * T3 * r1 + (-dv1 * T5)) / T8;
	temp.c[1] = ((T9 + T10) * v1 + (-T9 - T10) * v0 + T11 * T4 + T12 * r0 * r1
			+ T13 * T3) / T8;
	temp.c[0] = ((-2 * v1) + 2 * v0 + (dv1 + dv0) * r1 + (-dv1 - dv0) * r0)
			/ T8;
	return temp;
}

Polynomial Polynomial::ByBezier(double v0) {
	Polynomial temp(1);
	temp.c[0] = v0;
	return temp;
}

Polynomial Polynomial::ByBezier(double v0, double v1) {
	Polynomial temp(2);
	temp.c[0] = v1 - v0;
	temp.c[1] = v0;
	return temp;
}

Polynomial Polynomial::ByBezier(double v0, double v1, double v2) {
	Polynomial temp(3);
	temp.c[0] = v0 - 2 * v1 + v2;
	temp.c[1] = 2 * v1 - 2 * v0;
	temp.c[2] = v0;
	return temp;
}

Polynomial Polynomial::ByBezier(double v0, double v1, double v2, double v3) {
	Polynomial temp(4);
	temp.c[0] = -v0 + 3 * v1 - 3 * v2 + v3;
	temp.c[1] = 3 * v0 - 6 * v1 + 3 * v2;
	temp.c[2] = 3 * v1 - 3 * v0;
	temp.c[3] = v0;
	return temp;
}

Polynomial Polynomial::ByBezier(const std::vector<double> &v) {
	// https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Polynomial_form
	switch (v.size()) {
	case 1:
		return Polynomial::ByBezier(v[0]);
	case 2:
		return Polynomial::ByBezier(v[0], v[1]);
	case 3:
		return Polynomial::ByBezier(v[0], v[1], v[2]);
	case 4:
		return Polynomial::ByBezier(v[0], v[1], v[2], v[3]);
	default: {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		err << "Only 1..4 double value are supported.";
		throw std::runtime_error(err.str());
	}
	}
}

Polynomial Polynomial::ByIntegrals(const std::vector<double> &integrals,
		double a, double b) {
	const size_t N = integrals.size();
	if (N == 0)
		return Polynomial();
	if (N == 1) {
		Polynomial temp(1);
		temp.c[0] = integrals[0] / (b - a);
		return temp;
	}
	std::vector<double> ab(N, 0);
	double a2 = a;
	double b2 = b;
	for (size_t n = N; n-- > 0;) {
		ab[n] = b2 - a2;
		a2 *= a;
		b2 *= b;
	}
	std::vector<size_t> f(N, 0);
	f[0] = 1;
	for (size_t n = 1; n < (N - 1); ++n)
		f[0] *= (N - n);
	Polynomial ret(N);
	for (size_t n = N; n-- > 0;) {
		double k = integrals[n];
		for (size_t m = 0; m < (N - n - 1); ++m) {
			k -= ret.c[m] * ab[m + n] * (double) f[m] / (double) (N - n - m);
		}
		ret.c[N - n - 1] = k / ab[N - 1] / (double) (f[N - n - 1]);
		if (n > 0) {
			f[N - n] = f[N - n - 1] / n;
			for (size_t m = 0; m < (N - n); ++m) {
				f[m] /= (N - n - m);
			}
		}
	}
	return ret;
}

Polynomial Polynomial::ByVectorDerivates(const std::vector<double> &r,
		const std::vector<double> &v, size_t order, size_t idxStart,
		size_t idxEnd) {
	if (r.size() != v.size())
		throw(std::logic_error(
				__FILE__ "Polynomial::ByVectorDerivates - r and v have different sizes."));
	if (r.empty())
		return Polynomial();
	if (idxEnd >= r.size())
		idxEnd = r.size() - 1;
	if (idxStart > idxEnd)
		return Polynomial();
	const size_t N = order + 1;
	const size_t M =
			((idxEnd - idxStart) >= order) ?
					(idxEnd - idxStart + 1) : (order + 1);

	// Calculate integrals
	std::vector<double> integral(N, 0.0);
	std::vector<double> p(M, 0.0);
	std::vector<double> d(M, 0.0);
	for (size_t m = 0; m < M; ++m) {
		p[m] = r[m + idxStart];
		d[m] = v[m + idxStart];
	}

	const double a = r[idxStart];
	const double b = r[idxEnd];

	double sum = 0.0;
	for (size_t m = 1; m < M; ++m)
		sum += (d[m] + d[m - 1]) / 2.0 * (p[m] - p[m - 1]);
	integral[0] = sum;

	for (size_t n = 1; n < N; ++n) {
		for (size_t m = 0; m < (M - n); ++m) {
			const double dp = (p[m + 1] - p[m]);
			const double dd = (d[m + 1] - d[m]);
			if (fabs(dp) < DBL_EPSILON) {
				d[m] = 0.0;
			} else {
				d[m] = dd / dp;
			}
			p[m] += dp / 2.0;
		}
		double sum1 = 0.0;
		for (size_t m = 1; m < (M - n); ++m)
			sum1 += (d[m] + d[m - 1]) / 2.0 * (p[m] - p[m - 1]);
		integral[n] = sum1;
	}
	return Polynomial::ByIntegrals(integral, a, b);
}

Polynomial Polynomial::ByVector(const std::vector<double> &r,
		const std::vector<double> &values, size_t order, size_t idxStart,
		size_t idxEnd) {
	if (r.size() != values.size())
		throw(std::logic_error(
		__FILE__ "Polynomial::ByVector - r and v have different sizes."));
	if (r.empty())
		return Polynomial();
	if (idxEnd >= r.size())
		idxEnd = r.size() - 1;
	if (idxStart > idxEnd)
		return Polynomial();
	const size_t N = order + 1;
	const size_t M =
			((idxEnd - idxStart) >= order) ?
					(idxEnd - idxStart + 1) : (order + 1);

	// Solving least-squares by using the pseudoinverse:
	// coefficients = inverse(transpose(vandermonde) * vandermonde) * transpose(vandermonde) * values

	//TODO: Speed-optimize this algorithm

	// Calculate Vandermonde matrix
	std::vector<double> Vandermonde(N * M, 1.0);
	for (size_t n = 1; n < N; ++n)
		for (size_t m = 0; m < M; ++m)
			Vandermonde[n * M + m] = Vandermonde[(n - 1) * M + m]
					* r[m + idxStart];

	// Prepare the pseudoinverse
	std::vector<double> H(N * N, 0.0); // Matrix that needs to be inverted
	for (size_t n1 = 0; n1 < N; ++n1)
		for (size_t n2 = 0; n2 < N; ++n2)
			for (size_t m = 0; m < M; ++m)
				H[n1 * N + n2] += Vandermonde[n1 * M + m]
						* Vandermonde[n2 * M + m];

	// Invert the symmetric matrix
	std::vector<double> Inv(N * N, 0.0); // Inverse Matrix of H

	if (N == 1) {
		Inv[0] = 1 / H[0];
	} else {
		// Simple basic Gauss-Jordan inversion

		// Set Inv to identity matrix
		for (size_t n = 0; n < N; ++n)
			Inv[n * N + n] = 1.0;

		for (size_t n0 = 0; n0 < N; ++n0) {
			// Find max element in column
			size_t nMax = n0;
			for (size_t n1 = n0; n1 < N; ++n1)
				if (fabs(H[n0 * N + n1]) > fabs(H[n0 * N + nMax]))
					nMax = n1;
			// Swap rows if the max element is not on the diagonal
			if (nMax != n0) {
				for (size_t n1 = 0; n1 < N; ++n1) {
					const double temp2 = H[n1 * N + n0];
					H[n1 * N + n0] = H[n1 * N + nMax];
					H[n1 * N + nMax] = temp2;
				}
				for (size_t n1 = 0; n1 < N; ++n1) {
					const double temp2 = Inv[n1 * N + n0];
					Inv[n1 * N + n0] = Inv[n1 * N + nMax];
					Inv[n1 * N + nMax] = temp2;
				}
			}
			// Eliminate bottom/left
			for (size_t n1 = n0 + 1; n1 < N; ++n1) {
				const double R = H[n0 * N + n1] / H[n0 * N + n0];
				for (size_t n2 = 0; n2 < N; ++n2) {
					H[n2 * N + n1] -= R * H[n2 * N + n0];
					Inv[n2 * N + n1] -= R * Inv[n2 * N + n0];
				}
			}
		}
		// Eliminate top/right
		for (size_t n0 = N; n0-- > 1;) {
			for (size_t n1 = n0; n1-- > 0;) {
				const double R = H[n0 * N + n1] / H[n0 * N + n0];
				for (size_t n2 = 0; n2 < N; ++n2) {
					H[n2 * N + n1] -= R * H[n2 * N + n0];
					Inv[n2 * N + n1] -= R * Inv[n2 * N + n0];
				}
			}
		}
		// Normalize the elements on the diagonal
		for (size_t n0 = 0; n0 < N; ++n0) {
			const double divider = H[n0 * N + n0];
			for (size_t n1 = 0; n1 < N; ++n1)
				H[n1 * N + n0] /= divider;
			for (size_t n1 = 0; n1 < N; ++n1)
				Inv[n1 * N + n0] /= divider;
		}
	}

	// Calculate coefficients
	Polynomial temp(N);
	for (size_t n1 = 0; n1 < N; ++n1) {
		double s = 0.0;
		for (size_t m = 0; m < M; ++m)
			for (size_t n2 = 0; n2 < N; ++n2)
				s += Inv[n1 * N + n2] * Vandermonde[n2 * M + m]
						* values[m + idxStart];
		temp.c[N - n1 - 1] = s;
	}
	return temp;
}

Polynomial Polynomial::Bernstein(const size_t N, const size_t index) {
	if (index >= N)
		throw std::runtime_error(
				"Polynomial::Bernstein - the index has to be in the range [0, N[");
	if (N == 0)
		return Polynomial(0);
	Polynomial ret(N);
	// Calculate Pascals triangle up to N.
	std::vector<size_t> P(N, 0);
	P[0] = 1;
	for (size_t n = 0; n < N; ++n) {
		for (size_t k = n; k > 0; --k)
			P[k] = P[k] + P[k - 1];
		if (n == (N - index - 1)) {
			double sign = 1.0;
			for (size_t k = 0; k < (N - index); ++k) {
				//ret[N - k - 1] = sign * (double) P[k];
				ret[index + k] = sign * (double) P[k];
				sign = -sign;
			}
		}
	}
	for (size_t k = index; k < N; ++k)
		ret[k] *= (double) P[index];
	return ret;
}

Polynomial Polynomial::Lagrange(const size_t N, const size_t index) {
	// TODO Already converted.
	if (index >= N)
		throw std::runtime_error(
				"Polynomial::Lagrange - the index has to be in the range [0, N[");
	if (N == 0)
		return Polynomial(0);
	Polynomial ret(N);
	double den = 1.0;
	ret[0] = 1.0;
	for (size_t n = 0; n < N; ++n) {
		if (n == index)
			continue;
		const double v = (double) n / (double) (N - 1);
		for (size_t m = N - 1; m > 0; --m)
			ret[m] = ret[m - 1] - ret[m] * v;
		ret[0] = -ret[0] * v;
		den *= ((double) index - (double) n) / (double) (N - 1);
	}
	for (size_t k = 0; k < N; ++k)
		ret[k] /= den;
	return ret;
}

Polynomial::Polynomial(size_t N) {
	c.assign(N, 0.0);
}

Polynomial::Polynomial(const std::initializer_list<double> coefficients) {
	const size_t N = coefficients.size();
	c.resize(N);
	size_t n = 0;
	for (const double coeff : coefficients) {
		c[N - n - 1] = coeff;
		++n;
	}
}

size_t Polynomial::Size() const {
	return c.size();
}

size_t Polynomial::Order() const {
	return (c.empty()) ? 0 : (c.size() - 1);
}

void Polynomial::Resize(size_t N) {
	const size_t N0 = c.size();
	if (N > N0) {
		c.resize(N, 0.0);
		for (size_t n = N; n-- > (N - N0);)
			c[n] = c[n + N0 - N];
		for (size_t n = 0; n < (N - N0); ++n)
			c[n] = 0.0;
	} else {
		for (size_t n = 0; n < N; ++n)
			c[n] = c[n + (N0 - N)];
		c.resize(N, 0.0);
	}
}

Polynomial Polynomial::Reduce(size_t N, double a, double b) const {

	Polynomial d;
	if (c.size() <= N) {
		d = *this;
		d.Resize(N);
		return d;
	}
	d.c.resize(N, 0.0);
	std::vector<double> integral(N, 0.0);
	Polynomial h = this->Integral();
	integral[0] = h(b) - h(a);
	if (N == 1) {
		d.c[0] = integral[0] / (b - a);
		return d;
	}
	h = *this;
	integral[1] = h(b) - h(a);
	for (size_t n = 2; n < (N); ++n) {
		h.Derive();
		integral[n] = h(b) - h(a);
	}
	std::vector<double> ab(N, 0);
	double a2 = a;
	double b2 = b;
	for (size_t n = N; n-- > 0;) {
		ab[n] = b2 - a2;
		a2 *= a;
		b2 *= b;
	}
	std::vector<size_t> f(N, 0);
	f[0] = 1;
	for (size_t n = 1; n < (N - 1); ++n)
		f[0] *= (N - n);
	for (size_t n = N; n-- > 0;) {
		double k = integral[n];
		for (size_t m = 0; m < (N - n - 1); ++m) {
			k -= d.c[m] * ab[m + n] * (double) f[m] / (double) (N - n - m);
		}
		d.c[N - n - 1] = k / ab[N - 1] / (double) f[N - n - 1];
		if (n > 0) {
			f[N - n] = f[N - n - 1] / n;
			for (size_t m = 0; m < (N - n); ++m) {
				f[m] /= (N - n - m);
			}
		}
	}
	return d;
}

double& Polynomial::operator [](size_t index) {
	return c[c.size() - index - 1];
}

double Polynomial::operator [](size_t index) const {
	return c[c.size() - index - 1];
}

double Polynomial::operator()(double r) const {
	const size_t N = c.size();
	if (N == 0)
		return 0.0;
	double temp = c[0];
	for (size_t n = 1; n < N; ++n)
		temp = temp * r + c[n];
	return temp;
}
std::vector<double> Polynomial::GetBezier() const {
	std::vector<double> temp(c.size(), 0);
	if (c.size() == 0)
		return temp;
	switch (c.size()) {
	case 1:
		temp[0] = c[0];
		break;
	case 2:
		temp[0] = c[1];
		temp[1] = c[0] + c[1];
		break;
	case 3:
		temp[0] = c[2];
		temp[1] = c[1] / 2 + c[2];
		temp[2] = c[0] + c[1] + c[2];
		break;
	case 4:
		temp[0] = c[3];
		temp[1] = c[2] / 3 + c[3];
		temp[2] = c[1] / 3 + c[2] * 2 / 3 + c[3];
		temp[3] = c[0] + c[1] + c[2] + c[3];
		break;
	default:
		throw(std::range_error(
				__FILE__" GetBezier - This function is only supports up to 4 Bezier-points."));
	}
	return temp;
}

Polynomial& Polynomial::operator +=(const double &b) {
	const size_t N = c.size();
	c[N - 1] += b;
	return *this;
}

Polynomial& Polynomial::operator +=(const Polynomial &b) {
	size_t B = b.Size();
	if (B > c.size()) {
		const size_t d = B - c.size();
		c.insert(c.begin(), d, 0.0);
		for (size_t n = 0; n < B; ++n)
			c[n] += b.c[n];
	} else {
		const size_t d = c.size() - B;
		for (size_t n = 0; n < B; ++n)
			c[n + d] += b.c[n];
	}
	return *this;
}

Polynomial& Polynomial::operator -=(const double &b) {
	const size_t N = c.size();
	c[N - 1] -= b;
	return *this;
}

Polynomial& Polynomial::operator -=(const Polynomial &b) {
	size_t B = b.Size();
	if (B > c.size()) {
		const size_t d = B - c.size();
		c.insert(c.begin(), d, 0.0);
		for (size_t n = 0; n < B; ++n)
			c[n] -= b.c[n];
	} else {
		const size_t d = c.size() - B;
		for (size_t n = 0; n < B; ++n)
			c[n + d] -= b.c[n];
	}
	return *this;
}

Polynomial& Polynomial::operator *=(const double &b) {
	const size_t N = c.size();
	for (size_t n = 0; n < N; ++n)
		c[n] *= b;
	return *this;
}

Polynomial& Polynomial::operator *=(const Polynomial &b) {
	const std::vector<double> cold = c;
	const size_t N = cold.size();
	const size_t M = b.c.size();
	c.assign(N + M - 1, 0.0);
	for (size_t n = 0; n < N; ++n)
		for (size_t m = 0; m < M; ++m)
			c[n + m] += cold[n] * b.c[m];
	return *this;
}

Polynomial& Polynomial::operator /=(const double &b) {
	const size_t N = c.size();
	for (size_t n = 0; n < N; ++n)
		c[n] /= b;
	return *this;
}

std::tuple<Polynomial, Polynomial> Polynomial::operator /(
		const Polynomial &b) const {
	const size_t N = Size();
	const size_t NV = b.Size();
	Polynomial Q(N);
	Polynomial R = *this;
	for (size_t k = N - NV + 1; k-- > 0;) {
		Q.c[N - (k + 1)] = R.c[N - (NV + k)] / b.c[NV - NV];
		for (size_t j = NV + k - 1 + 1; j-- > k + 1;) {
			R.c[N - j] = R.c[N - j] - Q.c[N - (k + 1)] * b.c[NV - (j - k)];
		}
	}
	R.c[N - NV] = 0.0;
	Q.Resize(N - NV + 1);
	R.Resize(NV - 1);
	return {Q, R};
}

void Polynomial::ScaleX(double val) {
	const size_t N = c.size();
	val = 1.0 / val;
	double temp = val;
	// This is a reverse for-loop for unsigned variables counting down to and including 0.
	for (size_t n = N - 1; n-- > 0;) {
		c[n] *= temp;
		temp *= val;
	}
}

void Polynomial::ScaleY(double val) {
	const size_t N = c.size();
	for (size_t n = 0; n < N; ++n)
		c[n] *= val;
}

void Polynomial::ShiftX(double val) {
	const size_t N = c.size();
	const std::vector<double> cold(c);
	std::vector<double> pascal(N, 1.0); // Stores a diagonal in Pascals triangle
	double valpower = val;
	double sign = -1.0;
	for (size_t nv = (N - 1); nv > 0; --nv) {
		for (size_t n = N - 1; n > (N - nv - 1); --n) {
			pascal[N - n] += pascal[N - n - 1];
			c[n] += sign * pascal[N - n - 1] * cold[n - (N - nv)] * valpower;
		}
		sign = -sign;
		valpower *= val;
	}
}

void Polynomial::ShiftY(double val) {
	const size_t N = c.size();
	c[N - 1] += val;
}

Polynomial Polynomial::Derivative(size_t order) const {
	//TODO Already fixed
	Polynomial temp = *this;
	const size_t N = temp.Size();
	if (order >= N)
		return Polynomial( { 0 });
	for (size_t m = 1; m <= order; ++m)
		for (size_t n = 0; n < (N - m); ++n)
			temp[n] = temp[n + 1] * (double) (n + 1);
	temp.Resize(N - order);
	return temp;
}

void Polynomial::Derive(size_t order) {
	const size_t N = c.size();
	if (order >= N) {
		c = { 0 };
		return;
	}
	for (size_t m = 1; m <= order; ++m)
		for (size_t n = 0; n < (N - m); ++n)
			c[n] *= (double) (N - m - n);
	c.resize(N - order);
}

double Polynomial::Integral(double a, double b) const {
	Polynomial temp = this->Integral();
	return temp(b) - temp(a);
}

Polynomial Polynomial::Integral(size_t order) const {
	// TODO Already fixed.
	Polynomial temp = *this;
	if (order == 0)
		return temp;
	const size_t N = temp.Size();
	temp.Resize(N + order);
	for (size_t m = 0; m < order; ++m) {
		for (size_t n = (N + m); n > m; --n)
			temp[n] = temp[n - 1] / (double) (n);
		temp[m] = 0.0;
	}
	return temp;
}

void Polynomial::Integrate(size_t order) {
	// TODO Invert c.
	if (order == 0)
		return;
	const size_t N = c.size();
	c.resize(N + order, 0.0);
	for (size_t m = 0; m < order; ++m) {
		for (size_t n = (N + m); n > m; --n)
			c[(N + order - 1) - n] = c[(N + order - 1) - (n)] / (double) (n);
		c[(N + order - 1) - m] = 0.0;
	}
}

Polynomial Polynomial::Inverse(double a, double b, size_t N) {
	if (c.size() < 2)
		throw(std::range_error(
		__FILE__" Invert - This function needs at least 2 coefficients."));
	Polynomial ret;
	if (c.size() == 2) {
		if (fabs(c[1]) < DBL_EPSILON)
			throw(std::domain_error(
					__FILE__" Invert - This function cannot invert constants (first coefficient is 0)."));
		ret.Resize(2);
		ret[1] = 1 / c[0];
		ret[0] = -ret[1] * c[1];
	} else {
		if (N == (size_t) -1)
			N = c.size();
		ret.Resize(N);
		std::vector<double> x(N);
		std::vector<double> y(N);
		for (size_t n = 0; n < N; ++n)
			x[n] = a + (b - a) * (double) n / (double) (N - 1);
		for (size_t n = 0; n < N; ++n)
			y[n] = operator ()(x[n]);
		if (y[0] < y[1]) {
			// Increasing values
			for (size_t n = 0; n < (N - 1); ++n)
				if (y[n] >= y[n + 1])
					throw(std::range_error(
							__FILE__" Invert - The function has to be uniform in the selected range. (It has sections where it is falling after rising.)"));
		} else {
			// Decreasing values
			for (size_t n = 0; n < (N - 1); ++n)
				if (y[n] <= y[n + 1])
					throw(std::range_error(
							__FILE__" Invert - The function has to be uniform in the selected range. (It has sections where it is rising after falling.)"));
		}
		ret = Polynomial::ByVector(y, x, N - 1);
	}
	return ret;
}

double Polynomial::ExtremumPos() const {
	//TODO Change order of c
	switch (c.size()) {
	case 0:
	case 1:
	case 2: {
		return std::numeric_limits<double>::quiet_NaN();
	}
	case 3: {
		if (c[0] > -DBL_EPSILON)
			return std::numeric_limits<double>::quiet_NaN();
		double x = -c[1] / (2.0 * c[0]);
		return x;
	}
	case 4: {
		if (fabs(c[3]) < DBL_EPSILON)
			return std::numeric_limits<double>::quiet_NaN();
		const double h = c[1] * c[1] - 3.0 * c[0] * c[2];
		if (h < 0)
			return std::numeric_limits<double>::quiet_NaN();
		double x1 = (-c[1] + sqrt(h)) / (3.0 * c[0]);
		double x2 = (-c[1] - sqrt(h)) / (3.0 * c[0]);
		double t1 = 6.0 * c[0] * x1 + 2.0 * c[1];
		double t2 = 6.0 * c[0] * x2 + 2.0 * c[1];
		if (t1 < 0.0)
			return x1;
		if (t2 < 0.0)
			return x2;
		return std::numeric_limits<double>::quiet_NaN();
	}
	default:
		throw(std::range_error(
				__FILE__" ExtremumPos - This function is only supported for an order up to three (= 4 coefficients)."));
	}

}

double Polynomial::ExtremumNeg() const {
	//TODO Change order of c
	switch (c.size()) {
	case 0:
	case 1:
	case 2: {
		return std::numeric_limits<double>::quiet_NaN();
	}
	case 3: {
		if (c[0] < DBL_EPSILON)
			return std::numeric_limits<double>::quiet_NaN();
		double x = -c[1] / (2.0 * c[0]);
		return x;
	}
	case 4: {
		if (fabs(c[3]) < DBL_EPSILON)
			return std::numeric_limits<double>::quiet_NaN();
		const double h = c[1] * c[1] - 3.0 * c[0] * c[2];
		if (h < 0)
			return std::numeric_limits<double>::quiet_NaN();
		double x1 = (-c[1] + sqrt(h)) / (3.0 * c[0]);
		double x2 = (-c[1] - sqrt(h)) / (3.0 * c[0]);
		double t1 = 6.0 * c[0] * x1 + 2.0 * c[1];
		double t2 = 6.0 * c[0] * x2 + 2.0 * c[1];
		if (t1 > 0.0)
			return x1;
		if (t2 > 0.0)
			return x2;
		return std::numeric_limits<double>::quiet_NaN();
	}
	default:
		throw(std::range_error(
				__FILE__" ExtremumNeg - This function is only supported for an order up to 3 (= 4 coefficients)."));
	}
}

double Polynomial::InflectionPoint() const {
	//TODO Change order of c
	if (c.size() > 4)
		throw(std::range_error(
				__FILE__" InflectionPoint - This function is only supported for an order up to 3 (= 4 coefficients)."));
	if (c.size() < 4)
		return std::numeric_limits<double>::quiet_NaN();
	if (fabs(c[0]) < DBL_EPSILON)
		return std::numeric_limits<double>::quiet_NaN();
	double x = -c[1] / (3.0 * c[0]);
	return x;
}

double Polynomial::FindZero(const double xStart, const size_t maxSteps) const {
	if (c.size() <= 1)
		return xStart;
	if (c.size() == 2) {
		return -c[1] / c[0];
	}
	const Polynomial pd1 = this->Derivative();
	const Polynomial pd2 = pd1.Derivative();
	double x = xStart;
	for (size_t n = 0; n < maxSteps; ++n) {
		const double d0 = this->operator ()(x);
		if (fabs(d0) < DBL_EPSILON)
			break;
		const double d1 = pd1.operator ()(x);
		const double d2 = pd2.operator ()(x);
		const double r2 = d1 * d1 - 2 * d0 * d2;
		const double h0 = -d1 / d2;
		x = x + h0;
		if (r2 < 0)
			continue;
		const double h1 = sqrt(r2) / d2;
		if ((h0 > 0 && h1 > 0) || (h0 < 0 && h1 < 0)) {
			x -= h1;
		} else {
			x += h1;
		}
	}
	return x;
}

