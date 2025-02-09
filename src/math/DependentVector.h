///////////////////////////////////////////////////////////////////////////////
// Name               : DependentVector.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 17.06.2019
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

#ifndef MATH_DEPENDENTVECTOR_H
#define MATH_DEPENDENTVECTOR_H

/*!\class DependentVector
 * \brief Vectors of numbers dependent on other vectors stored in a Matrix
 *
 * That one axis is an abscissa and the other axes are ordinates is only by
 * convention. Internally all axis are identical. (i.e. only columns in a
 * matrix.
 */

#include "Matrix.h"
#include "Unit.h"

#include <cfloat>
#include <cmath>
#include <cstddef>
#include <functional>
#include <vector>

class DependentVector: public Matrix {
public:
	std::vector<Unit> units;

protected:
	bool cyclic = false;
	double cycleLength = 0;

private:
	mutable size_t idxSearchSpeedup = 0;

public:
	DependentVector() = default;
	DependentVector(size_t N, size_t NAxes = 1);
	virtual ~DependentVector() = default;

private:
	/**\brief Synchronize the arrays and data orientation.
	 *
	 * Synchronizes units vector with axes and ensures, that the Matrix is
	 * column first orientation. (For insertion operations it is beneficial to
	 * use row a row first matrix.
	 */
	void Sync();

	/**\brief Ensure, that the requested axis exists.
	 */
	void Access(size_t idxAxis);

public:
	/**\brief A vector with linear points
	 *
	 * \param v0 Value of first point
	 * \param v1 Value of last point
	 * \param N Number of points in total.
	 */
	static DependentVector Linspace(double v0, double v1, size_t N);

	/**\brief A vector with Chebyshev node points
	 *
	 * [Chebyshev nodes](https://en.wikipedia.org/wiki/Chebyshev_nodes) are
	 * points spaced in a way, that interpolations based on these minimize
	 * [Runge's phenomenon](https://en.wikipedia.org/wiki/Runge%27s_phenomenon).
	 *
	 * \param v0 Value of first point
	 * \param v1 Value of last point
	 * \param N Number of points in total.
	 */
	static DependentVector Chebyshev(double v0, double v1, size_t N);

public:
	/**\brief Set the sizes of the vectors to 0
	 *
	 * Remove all elements from all vectors.
	 */
	void Clear();

	/** \brief Resize all vectors to N points.
	 */
	void SetSize(size_t N, size_t NAxes = 1);
	void SetSize(const std::vector<size_t> &dims);
	void SetSize(const DependentVector &other);

	size_t Length() const; ///< Number of samples
	size_t AxesCount() const; ///< Number of axes

	/**\brief Connect the last point of the vector to the first (virtually)
	 *
	 * The data in the vectors is not changed. The length new length of the
	 * abscissa is stored and the output of some functions is modified to
	 * reflect a dataset, where the x-axis loops around to the first point.
	 * The last x-point in the dataset should be smaller than the first x-point
	 * plus the cycleLength.
	 */
	void SetCyclic(double cycleLength);

	/**\brief Flag the vector as a normal linear vector.
	 *
	 * The cycleLength is set to 0 and the cyclic-flag is set to false.
	 */
	void SetLinear();
	bool IsCyclic() const; ///< Return flag is cyclic
	double CycleLength() const; ///< Return cycle length if cyclic or 0.

	/**\brief Cut a subrange from a vector
	 *
	 * A new DependentVector is returned only containing the indices from
	 * rowStart to rowEnd. The y-axes are also cut accordingly.
	 */
	DependentVector Range(size_t rowStart = 0,
			size_t rowEnd = (size_t) -1) const;

public:
	DependentVector& operator+=(const DependentVector &a);
	friend DependentVector operator+(DependentVector lhs,
			const DependentVector &rhs) {
		lhs += rhs;
		return lhs;
	}
	DependentVector& operator-=(const DependentVector &a);
	friend DependentVector operator-(DependentVector lhs,
			const DependentVector &rhs) {
		lhs -= rhs;
		return lhs;
	}
	DependentVector& operator*=(const DependentVector &a);
	friend DependentVector operator*(DependentVector lhs,
			const DependentVector &rhs) {
		lhs *= rhs;
		return lhs;
	}
	DependentVector& operator/=(const DependentVector &a);
	friend DependentVector operator/(DependentVector lhs,
			const DependentVector &rhs) {
		lhs /= rhs;
		return lhs;
	}

public:
	/**\brief Push a single point into the vectors.
	 *
	 * If no y-axis existed, a single y-axis is created.
	 */
	void PushBack(double x, double y);

public:
	struct Point {
		Point() = default;
		explicit Point(size_t yAxesCount) {
			y.resize(yAxesCount, 0.0);
		}
		Point(size_t idx, double rel, double x) :
				idx(idx), rel(rel), x(x) {
		}
		size_t idx = (size_t) -1; ///< Index of the point searched for
		double rel = 0.0; ///< relative distance from idx to idx+1 (or idx-1 to idx for the last point in the not cyclic case); interpolation value for all axes.
		double x = 0.0; ///< Exact position of the search condition (interpolated)
		std::vector<double> y; ///< Exact values on all y-axes
	};

	/**\class AxisView
	 * \brief Subclass for accessing vectors in the matrix.
	 *
	 * The AxisView object contains a pointer to the parent DependentVector.
	 * Therefore the lifespan of an object of this class should not exceed the
	 * lifespan of the parent DependentVector.
	 *
	 *
	 *
	 *
	 */
	class AxisView {
	public:
		AxisView(DependentVector *const parent, size_t axBegin, size_t axEnd,
				size_t rowBegin = 0, size_t rowEnd = (size_t) -1);
	private:
		DependentVector *const parent = nullptr;
		const size_t axBegin = 0;
		const size_t axEnd = (size_t) -1;
		const size_t rowBegin = 0;
		const size_t rowEnd = (size_t) -1;

		size_t GetRowStart() const;
		size_t GetRowEnd() const;

	public:

		double& operator[](size_t row);
		double operator[](size_t row) const;

		/**\brief Initialize the vector with a value
		 *
		 * All elements in the vector are set.
		 * \param value Value to initialize the vector with
		 */
		void Init(double value = 0.0);

		/**\brief Fill the vector with linearly spaced points.
		 *
		 * \param v0 Start value
		 * \param v1 End value
		 */
		void Linspace(double v0, double v1);

		/**\brief Fill the vector with Chebyshev node points.
		 *
		 * \param v0 Start value
		 * \param v1 End value
		 */
		void Chebyshev(double v0, double v1);

		/**\brief Limit the values of an axis between vMin and vMax
		 *
		 * \param vMin lowest value
		 * \param vMax highest value
		 */
		void Limit(double vMin, double vMax);

		/**\brief Wrap the values of the selected axis in the range of v0 to v1
		 *
		 * Limit the range of values by wrapping the values around at the range
		 * from v0 to v1. Common settings are 0 and 2*pi or -pi and pi for the
		 * limits to force the values to an angle on a circle.
		 *
		 * \brief v0 Minimal value of the range
		 * \brief v1 Maximal value of the range
		 */
		void Wrap(double v0, double v1);

		/**\brief Unwrap a wrapped range
		 *
		 * Assuming that the values on the vector follow a sequence that is wrapped
		 * this function can unwrap these values in a linear sequence by checking
		 * against a tolerance. If a point is offset by more than the tolerance
		 * the position of the point is adjusted by two times that very tolerance.
		 *
		 * e.g. if one point is 0.1 and the next point is 6.183 unwrapping would
		 * detect that the point is > 0.1+M_PI and subtract 2*M_PI from the point
		 * and all subsequent points. The unwrapped value would be -0.1.
		 *
		 * The function removes steps in data.
		 *
		 * \param tolerance for detection of steps. 2 * tolerance for the correction of the step.
		 */
		void Unwrap(double tolerance = M_PI);

		void Normalize();

		void CumSum();
		void CumProd();

		/**\brief Fill an axis with calculated points
		 *
		 * This function can only be applied to any axis. The values of the
		 * x-axis are transformed and written to the selected axis. The x-axis
		 * can also be overwritten by this function.
		 *
		 * If the x-axis and other axes are selected, the x-axis is overwritten
		 * last. I.e. all axes are calculated from the old x-axis values.
		 *
		 * \param func Any function that returns a double for a double
		 */
		void Fill(std::function<double(double)> func);

		void Integrate();
		void Derive();

		double Mean() const;
		double Area() const;

		double Max() const;
		double Min() const;
		DependentVector::Point MaxP(bool useParabolas = false) const;
		DependentVector::Point MinP(bool useParabolas = false) const;

		std::vector<DependentVector::Point> FindPeaks(const double vMin =
				-DBL_MAX);
		std::vector<DependentVector::Point> FindValleys(const double vMax =
		DBL_MAX);

	private:
		DependentVector::Point ExtremumP(bool useParabolas, bool invert,
				bool immediatlyReturn) const;

		/**\brief Parabola implementation with emphasis on quick recalculation
		 */
		class Parabola {
		public:
			Parabola() = delete;
			Parabola(double x0, double x1, double x2);
			bool IsOK() const;
			void Calc(double v0, double v1, double v2);
			bool IsMax() const;
			bool IsMin() const;
			double XExtremum() const;
			double Y(double x) const;
		private:
			double ec0v0 = 0.0;
			double ec0v1 = 0.0;
			double ec0v2 = 0.0;
			double ec1v0 = 0.0;
			double ec1v1 = 0.0;
			double ec1v2 = 0.0;
			double ec2v0 = 0.0;
			double ec2v1 = 0.0;
			double ec2v2 = 0.0;
			double c0 = 0.0;
			double c1 = 0.0;
			double c2 = 0.0;
			double div = 1.0;
			bool OK = true;
		};

	};

	/**\name Access to the pointer an axis
	 *
	 * In the none constant case, the axis accessed is created, it it did
	 * not exist before.
	 *
	 * \param axis Index of the axis to access.
	 * \{
	 */
	AxisView All(); ///< Accessor to all axes
	AxisView AllY(); ///< Accessor to all axes
	AxisView Ax(size_t axis); ///< Accessor to an axis
	AxisView X(); ///< Accessor to x-axis
	AxisView Y(); ///< Accessor to first y-axes
//	AxisView Y(size_t axis); ///< Accessor to one y-axes

	///>\}

public:

	/**\brief Sort the values on the x-axis or on one of the y-axes
	 *
	 * All axes are sorted by one of the axes. This make the values (weakly)
	 * monotonic increasing for that axis.
	 *
	 * The sorting function modifies all axes to keep the consistency in the
	 * stored data.
	 *
	 * \param axisIdx Sort by this axis.
	 */
	void Sort(size_t axisIdx);

	void Reverse(); ///< Reverse the ordering of all axes.
	void Resample(size_t N);
	DependentVector Sample(const std::vector<double> &x) const;
	DependentVector Sample(const std::vector<Point> &pos) const;
	void Map(const std::vector<size_t> &indices);

public:
	enum class Direction {
		first_risingabove, ///< The first point, that has a value >= searched value
		last_risingabove, ///< The last point, that passes value >= searched value
		first_fallingbelow, ///< The first point that has a value <= searched value
		last_fallingbelow, ///< The last point that has a value <= searched value
		first_passing, ///< The first passing of the values searched for
		last_passing ///< The passing passing of the values searched for
	};

	/**\brief Index of a value
	 *
	 * Returns the first point of the interval the value is in. If
	 * the value is outside the range of values, the first point or the last
	 * point respectively is returned.
	 *
	 * \note (The axis parameter is mostly for symmetry with the other functions
	 * 		 PatV() and VatV() that need knowledge of the other axes to return
	 * 		 a result.
	 *
	 * \param value Value to compare the values in the vector against
	 * \param axis Axis to search for the value in
	 * \param direction Enum from the Direction class on the search condition
	 * \param rowStart First index to look at (default: begin of vector)
	 * \param rowEnd Last index to look at (default: last value in vector)
	 */
	size_t IatV(double value, size_t axis, Direction direction =
			Direction::first_risingabove, size_t rowStart = 0, size_t rowEnd =
			(size_t) -1) const;

	/**\brief Search for a value, return a Point struct
	 *
	 * Search for a value in the vector and return a point struct on the
	 * findings.
	 *
	 * \param value Value to compare the values in the vector against
	 * \param axis Axis to search for the value in
	 * \param direction Enum from the Direction class on the search condition
	 * \param rowStart First index to look at (default: begin of vector)
	 * \param rowEnd Last index to look at (default: last value in vector)
	 */
	Point PatV(double value, size_t axis, Direction direction =
			Direction::first_risingabove, size_t rowStart = 0, size_t rowEnd =
			(size_t) -1) const;

	/*!\brief Interpolation of the array
	 *
	 * The output of this function is the same as Octaves/Matlabs interp1 with
	 * the 'extrap' option enabled. The extrapolation is linear.
	 *
	 * \param value Value to compare the values in the vector against
	 * \param axisFrom Axis to search for the value in
	 * \param axisTo Axis to read the returned value from
	 * \param direction Enum from the Direction class on the search condition
	 * \param rowStart First index to look at (default: begin of vector)
	 * \param rowEnd Last index to look at (default: last value in vector)
	 */
	double VatV(const double value, size_t axisFrom, size_t axisTo = 0,
			Direction direction = Direction::first_risingabove,
			size_t rowStart = 0, size_t rowEnd = (size_t) -1) const;

public:
	void Paint() const;
	static void PaintCircle(double radius); //TODO This function is in the wrong place/class (again). It needs a better "home".
};

#endif /* MATH_DEPENDENTVECTOR_H */

