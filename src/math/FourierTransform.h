///////////////////////////////////////////////////////////////////////////////
// Name               : FourierTransform.h
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

#ifndef MATH_FOURIERTRANSFORM_H
#define MATH_FOURIERTRANSFORM_H

/*!\class FourierTransform
 * \brief Fourier Transform for non-uniformly sampled data
 *
 * Applies a Fourier Transform for arbitrarily sampled data. A time vector has
 * to be passed to the function additionally to the (complex) input samples.
 * Functions for rescaling the input time vector are available. The frequencies
 * generated can be chosen arbitrarily as well. Also a fft-like result can be
 * requested (FLikeFFT).
 *
 * Note: The result is calculated to return the correct result with a symmetric
 * spectrum.
 *
 * Example: A x(t)=cos(2*pi*10*t) Fourier-transformed results in two peaks in
 * the spectrum. One at -10 Hz and one at +10 Hz. Both with a height of 1/2.
 * Because the synthesis function would be
 * xx(t) = 0.5*exp(j*2*pi*-10*t) + 0.5*exp(j*2*pi*10*t).
 * Therefore a single sided has to be doubled in amplitude to return the right
 * results. The function SingleSidedResult takes care of this.
 */

#include <cstddef>
#include <vector>
#include <cmath>

class FourierTransform {
public:
	FourierTransform() = default;
	virtual ~FourierTransform() = default;

	void TReset();

	void TSetSize(size_t N);
	size_t TGetSize() const;

	/**\brief Fills the T vector with equidistant points like Matlabs/Octaves linspace command.
	 *
	 * \param t0 First point on the T axis
	 * \param t1 Last point on the T axis
	 * \param N Number of samples
	 */
	void TLinspace(double t0, double t1, size_t N = 0);

	void XSet(size_t n, double re, double im = 0.0);
	void XAdd(double t, double re, double im = 0.0);

	void TSetLoopLength(double loopLength);
	// TUnwraping is a special function written for symmetry detection.
	// (Should probably be moved over into the Symmetry class.
	void TUnwrap(double tol = M_PI);
	void TScale(const double scale);

	void FLinspace(double f0, double f1, size_t N = 0);

	//!\brief Set up an analysis with an output that is similar to a FFT
	void FLikeFFT(size_t N);

	//!\brief Do the Fourier Transform and store the results in OutRe and OutIm
	void Transform();

	//!\brief Double all results in OutRe and OutIm except for f == 0
	void SingleSidedResult();

	//!\brief Scale the full spectrum
	void YScale(const double scale);

	std::vector<double> t;
	std::vector<double> InRe;
	std::vector<double> InIm;

	std::vector<double> f;
	std::vector<double> OutRe;
	std::vector<double> OutIm;
};

#endif /* MATH_FOURIERTRANSFORM_H */
