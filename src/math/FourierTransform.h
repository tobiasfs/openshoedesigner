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
 * Applies a Fourier Transform for arbitrarily sampled data.
 *
 * A time vector has to be passed to the function additionally to
 * the (complex) input samples.
 *
 * A frequency vector has to be defined, that the amplitude/phase values are
 * calculated for. As the input is not uniformly sampled, this vector cannot
 * be derived from the input T axis. If the input happens to be uniformly
 * sampled the function FlikeFFT() can be used to setup a compatilbe F vector.
 *
 *
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
	struct PointTime {
		double t;
		double re;
		double im;
	};
	struct PointFrequency {
		double f;
		double re;
		double im;
	};

	FourierTransform() = default;
	virtual ~FourierTransform() = default;

	/**\{
	 * \name Input preparation
	 *
	 */

	/**\brief Empty the T and X vector.
	 */
	void TXClear();

	void TSetSize(size_t N);
	size_t TGetSize() const;

	/**\brief Fills the T vector with equidistant points like Octaves/Matlabs linspace command.
	 *
	 * \param t0 First point on the T axis
	 * \param t1 Last point on the T axis
	 * \param N Number of samples
	 */
	void TLinspace(double t0, double t1, size_t N = 0);

	/**\brief Set the value at an index.
	 */
	void XSet(size_t n, double re, double im = 0.0);
	/**\brief Add a sample to the back of the X-T input vectors.
	 *
	 * Extend the input vector by one and set the last value to the time t and
	 * the given point re and im.
	 *
	 * Values can be added in arbitrary order and later sorted usind TSort().
	 */
	void XAdd(double t, double re, double im = 0.0);

	/**\brief Sort the T vector
	 *
	 * The vector T is sorted for ascending time. The real and imaginary
	 * values are kept with the time vector. This function can be used to
	 * enter values in arbitrary order into the vector T.
	 */
	void TSort();

	/**\brief Close the loop if the input signal is periodic.
	 *
	 * This function adds another point to the end of the sample data, that
	 * is positioned so that the loop-length is set. The added point is the
	 * the first point of the sample data.
	 *
	 * Since the transformation algorithm works by bilinear interpolating the
	 * samples, this results in the correct periodic continuation of the
	 * input data in T/X.
	 */
	void TSetLoopLength(double loopLength);

	/**\brief Unwraping the t vector
	 *
	 * This is a special function written for symmetry detection.
	 * (Should probably be moved over into the Symmetry class.)
	 */
	void TUnwrap(double tol = M_PI);

	/**\brief Scale the length of the t vector by a scalar value.
	 */
	void TScale(const double scale);

	void FLinspace(double f0, double f1, size_t N = 0);

	/**\brief Set up an analysis with an output that is similar to a FFT
	 *
	 * Since the time vector is not uniformly sampled, there is no concept
	 * of a sample-rate. This function sets up the frequency vector so, that
	 * the output of the transformation is the same as if an uniformly sampled
	 * vector was passed into Octaves/Matlabs fft command.
	 *
	 * The frequency vector is filled with N numbers. E.g. for N = 10
	 *
	 * ```
	 *  0, 1, 2, 3, 4, -5, -4, -3, -2, -1
	 * ```
	 * For N = 11:
	 * ```
	 *  0, 1, 2, 3, 4, 5, -5, -4, -3, -2, -1
	 * ```
	 */
	void FLikeFFT(size_t N);

	/**\brief Scale the length of the F vector by a scalar value.
	 *
	 * Useful in conjunction with FLikeFFT().
	 */
	void FScale(const double scale);

	/**\}
	 * \{
	 * \name Transformation
	 */

	//!\brief Do the Fourier Transform and store the results in OutRe and OutIm
	void Transform();

	/**\}
	 * \{
	 * \name Output handling
	 */

	/**\brief Double all results in OutRe and OutIm except for f == 0
	 *
	 */
	void SingleSidedResult();

	//!\brief Scale the full spectrum
	void YScale(const double scale);

	/**\}
	 */

	std::vector<PointTime> x;
	std::vector<PointFrequency> y;

};

#endif /* MATH_FOURIERTRANSFORM_H */
