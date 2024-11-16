///////////////////////////////////////////////////////////////////////////////
// Name               : FourierTransform.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 11.05.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "FourierTransform.h"

#include <stdexcept>

void FourierTransform::TReset() {
	t.clear();
	InRe.clear();
	InIm.clear();
}

void FourierTransform::TSetSize(size_t N) {
	t.resize(N);
	InRe.resize(N, 0);
	InIm.resize(N, 0);
	if (N < 1)
		throw(std::domain_error(
		__FILE__"FourierTransform::SetInputSize: Nin < 1"));
	for (size_t n = 0; n < N; ++n)
		t[n] = (double) n / (double) N;
}

size_t FourierTransform::TGetSize() const {
	return t.size();
}

void FourierTransform::TLinspace(double t0, double t1, size_t N) {
	t.resize(N);
	InRe.resize(N, 0);
	InIm.resize(N, 0);
	if (N < 2)
		throw(std::domain_error(
		__FILE__"FourierTransform::TLinspace: Nin < 2"));
	const double dt = (t1 - t0) / (N - 1);
	for (size_t n = 0; n < N; ++n) {
		t[n] = t0;
		t0 += dt;
	}
}

void FourierTransform::XSet(size_t n, double re, double im) {
	if (n >= InRe.size())
		throw(std::out_of_range(
		__FILE__"FourierTransform::SetInput: index n is out of range"));
	InRe[n] = re;
	InIm[n] = im;
}

void FourierTransform::XAdd(double t, double re, double im) {
	this->t.push_back(t);
	this->InRe.push_back(re);
	this->InIm.push_back(im);
}

void FourierTransform::TSetLoopLength(double loopLength) {
	if (t.size() >= InRe.size() + 1)
		throw(std::logic_error(
				__FILE__"FourierTransform::TSetLoopLength: Function has already been called."));
	if (t.size() < 1)
		throw(std::logic_error(
				__FILE__"FourierTransform::TSetLoopLength: Cannot close empty time-vector."));
	if (t[t.size() - 1] >= t[0])
		t.push_back(t[0] + loopLength);
	else
		t.push_back(t[0] - loopLength);
}

void FourierTransform::TUnwrap(double tol) {
	if (tol <= 0.0)
		throw(std::domain_error(
		__FILE__"FourierTransform::TUnwrap: 'tol' should be a positive number."));
	for (size_t n = 1; n < t.size(); ++n) {
		while (t[n] < t[n - 1] - tol)
			t[n] += 2 * tol;
		while (t[n] > t[n - 1] + tol)
			t[n] -= 2 * tol;
	}
}

void FourierTransform::TScale(const double scale) {
	for (size_t n = 0; n < t.size(); ++n)
		t[n] *= scale;
}

void FourierTransform::FLinspace(double f0, double f1, size_t N) {
	if (N < 2)
		throw(std::domain_error(
		__FILE__"FourierTransform::FLinspace: Nout < 2"));
	f.resize(N);
	OutRe.resize(N);
	OutIm.resize(N);
	const double df = (f1 - f0) / (N - 1);
	for (size_t n = 0; n < N; ++n) {
		f[n] = f0;
		f0 += df;
	}
}

void FourierTransform::FLikeFFT(size_t N) {
	if (N < 1)
		throw(std::domain_error(
		__FILE__"FourierTransform::FLikeFFT: N < 1"));
	f.resize(N);
	OutRe.resize(N);
	OutIm.resize(N);
	const size_t limit = (N + 1) / 2;
	for (size_t n = 0; n < N; ++n) {
		if (n >= limit)
			f[n] = -(double) (N - n);
		else
			f[n] = (double) n;
	}
}

void FourierTransform::Transform() {
//  from wxMaxima:
//		I(t,t0,t1,x0,x1):=(x1-x0)/(t1-t0)*(t-t0)+x0;
//		integrate(I(t,t0,t1,xRe0,xRe1)*cos(2*%pi*f*t)+I(t,t0,t1,xIm0,xIm1)*sin(2*%pi*f*t),t,t0,t1)
//		integrate(I(t,t0,t1,xIm0,xIm1)*cos(2*%pi*f*t)-I(t,t0,t1,xRe0,xRe1)*sin(2*%pi*f*t),t,t0,t1)

	for (size_t n = 0; n < f.size(); ++n) {
		const double fr = f[n];
		double re = 0.0;
		double im = 0.0;

		for (size_t m = 0; m < (t.size() - 1); ++m) {
			const double t0 = t[m];
			const double t1 = t[m + 1];
			const double xRe0 = InRe[m];
			const double xRe1 = (m < InRe.size() - 1) ? InRe[m + 1] : InRe[0];
			const double xIm0 = InIm[m];
			const double xIm1 = (m < InIm.size() - 1) ? InIm[m + 1] : InIm[0];

			// The distinction of fr == 0 is necessary, because a central element of the
			// Fourier calculation is sin(fr)/fr which is 1 for fr = 0 as per definition
			// of the sinc (and L'Hopital's rule).
			if (fabs(fr) < 1e-9) {
				const double d = fabs(t1 - t0) / 2;
				re += (xRe0 + xRe1) * d;
				im += (xIm0 + xIm1) * d;
			} else {
				const double den = 4 * M_PI * M_PI * fr * fr * fabs(t1 - t0);
				const double co0 = cos(2 * M_PI * fr * t0);
				const double si0 = sin(2 * M_PI * fr * t0);
				const double co1 = cos(2 * M_PI * fr * t1);
				const double si1 = sin(2 * M_PI * fr * t1);

				re += ((-co0 + co1
						+ si1 * (2 * M_PI * fr * t1 - 2 * M_PI * fr * t0))
						* xRe1
						+ (co0 - co1 + 2 * M_PI * fr * si0 * t0
								- 2 * M_PI * fr * si0 * t1) * xRe0
						+ (-si0 + si1
								+ co1
										* (2 * M_PI * fr * t0
												- 2 * M_PI * fr * t1)) * xIm1
						+ (si0 - si1 - 2 * M_PI * co0 * fr * t0
								+ 2 * M_PI * co0 * fr * t1) * xIm0) / den;

				im += (-(-si0 + si1
						+ co1 * (2 * M_PI * fr * t0 - 2 * M_PI * fr * t1))
						* xRe1
						- (si0 - si1 - 2 * M_PI * co0 * fr * t0
								+ 2 * M_PI * co0 * fr * t1) * xRe0
						- (co0 - co1
								+ si1
										* (2 * M_PI * fr * t0
												- 2 * M_PI * fr * t1)) * xIm1
						- (-co0 + co1 - 2 * M_PI * fr * si0 * t0
								+ 2 * M_PI * fr * si0 * t1) * xIm0) / den;
			}
		}
		OutRe[n] = re;
		OutIm[n] = im;
	}
}

void FourierTransform::SingleSidedResult() {
	for (size_t n = 0; n < OutRe.size(); ++n) {
		if (fabs(f[n]) > 1e-9) {
			OutRe[n] *= 2;
			OutIm[n] *= 2;
		}
	}
}

void FourierTransform::YScale(const double scale) {
	for (size_t n = 0; n < OutRe.size(); ++n) {
		OutRe[n] *= scale;
		OutIm[n] *= scale;
	}
}
