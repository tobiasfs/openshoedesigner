///////////////////////////////////////////////////////////////////////////////
// Name               : Matrix.h
// Purpose            : Matrix class
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 04.11.2014
// Copyright          : (C) 2014 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H
/*!\class Matrix
 * \brief Matrix class
 *
 * Matrix class to store double values in a multi-dimensional matrix
 * organization.
 *
 * To the outside it has the same interface as a std::vector<double> with some
 * of the functions overwritten to keep the additional member variables for
 * the dimensions (counts, strides) in sync.
 *
 * ## Functions on the matrix
 *
 *  * Transpose
 *  * Squeeze
 *  * Reshape
 *  * Size
 *  * Min, Max, AllMin, AllMax, AbsMax
 *  * Elementwise operations +, -, *, / with double values.
 *  * Matrix operations +, -, /
 *  * Matrix inversion
 *  * Matrix pseudoinverse (numerically not very stable, use SVD for big equation systems)
 *
 * The functions provided behave like the Octave/Matlab equivalents, except
 * that **all** indices are 0 based.
 *
 * ### Principle of least surprise
 *
 * Octaves and Matlabs automatic square matrices are NOT implemented.
 * (E.g. if only one dimension is given as in eye(3) Octave/Matlab would return
 * a 3x3 matrix with 9 elements.) Here all dimensions are needed: Matrix::Eye(3, 3)
 *
 * ## Initializer Functions:
 *
 *  * Zeros
 *  * Ones
 *  * Value
 *  * Eye
 *
 * ## Data Organization
 *
 * By default values are stored in the mathematical order (as in Octave/Matlab)
 * which is a column based storage. This can be adjusted by modifying the
 * order variable.
 *
 * The first dimension (dimension 0) has the stride of 1.
 * Contrary to *Torch.Tensor*. A *Torch.Tensors* last dimension has a stride
 * of 1.
 *
 * \htmlonly
 <svg viewBox="0 0 166.5354 159.4488" height="45mm" width="47mm">
 <style>text{font-size:16px;text-align:center;text-anchor:middle;fill:black;}</style>
 <g transform="translate(0,-892.9134)">
 <text y="951" x="54">0</text>
 <text y="969" x="54">1</text>
 <text y="986.5" x="54">2</text>
 <text y="1004.5" x="54">3</text>
 <text y="1021.5" x="54">4</text>
 <text y="951" x="71.6">5</text>
 <text y="969" x="71.6">6</text>
 <text y="986.5" x="71.6">7</text>
 <text y="1003" x="71.6">⋮</text>
 <text transform="matrix(0,-1,1,0,0,0)" y="25" x="-980">Dimension 0</text>
 <text y="915" x="80">Dimension 1</text>
 <path d="m 44.89413,935.96111 0,88.58269 70.86614,0 0,-88.58269 z" style="fill:none;stroke:#000000;" />
 <path d="m 62.610666,935.96111 0,88.58269" style="fill:none;stroke:#000000;stroke-dasharray:1, 2;"/>
 <path d="m 80.327201,935.96111 0,88.58269" style="fill:none;stroke:#000000;stroke-dasharray:1, 2;"/>
 <path d="m 98.043737,935.96111 0,88.58269" style="fill:none;stroke:#000000;stroke-dasharray:1, 2;"/>
 <path d="m 44.89413,953.67765 70.86614,0" style="fill:none;stroke:#000000;stroke-dasharray:1, 2;"/>
 <path d="m 44.89413,971.39418 70.86614,0" style="fill:none;stroke:#000000;stroke-dasharray:1, 2;"/>
 <path d="m 44.89413,989.11072 70.86614,0" style="fill:none;stroke:#000000;stroke-dasharray:1, 2;"/>
 <path d="m 44.89413,1006.8273 70.86614,0" style="fill:none;stroke:#000000;stroke-dasharray:1, 2;"/>
 <path d="m 36.035863,935.96111 0,106.29919" style="fill:none;stroke:#000000;"/>
 <path d="m 44.89413,927.10284 88.58267,0" style="fill:none;stroke:#000000;"/>
 <path d="m 124.61854,918.24458 8.85826,8.85826 -8.85826,8.85827" style="fill:none;stroke:#000000;"/>
 <path d="m 27.177595,1033.4021 8.858268,8.8582 8.858267,-8.8582" style="fill:none;stroke:#000000;"/>
 </g></svg>
 * \endhtmlonly
 */

#include <ostream>
#include <stddef.h>
#include <string>
#include <vector>

class Matrix: public std::vector<double> {
protected:
	/**\brief Size of each dimension
	 *
	 * For each dimension of the matrix the number of elements in this
	 * direction is stored. The total number of elements in the std::vector is
	 * the product of all counts.
	 *
	 * If resized, the Resize() function uses the set order to update the
	 * strides from the counts.
	 *
	 * (This is called counts because the function .size() is inherited from
	 * std::vector and .size would clash and .sizes is too close to .size())
	 */
	std::vector<size_t> counts;

	/**\brief Strides for requesting data from the matrix
	 *
	 * The strides are the multipliers to find the position of the value in the
	 * array. This is a more flexible approach than using the Horner-schema on
	 * the dimension array. The strides are updated when resizing the matrix
	 * or updating the dimensions.
	 */
	std::vector<size_t> strides;

	/**\brief The ordering of the dimensions
	 *
	 * The dimensions of the matrix can be ordered in arbitrary ways. This
	 * order influences how the values are organized in memory. The memory
	 * representation is always a linear vector of double values. The order
	 * describes how the strides are calculated from the dimensions.
	 */
	enum class Order {
		NORMAL, ///< (default) Order used in Mathematics, Octave/Matlab
		TWO_REVERSED, ///< Normal order with first two dimensions reversed, useful for adding new rows in a table without sorting.
		REVERSED ///< Order used in e.g. Torch
	} order = Order::NORMAL;

	/**
	 * When inserting values linearly, this is the next position to add a
	 * value. Advances in steps of +1 upon insertion.
	 */
	size_t bufferpos = 0;

public:
	/**\name Static setters
	 *
	 * Create a matrix commonly used in calculation or as a starting point for
	 * other manipulations.
	 * \{
	 */
	static Matrix Zeros(size_t S0, size_t S1 = 1, size_t S2 = 1, size_t S3 = 1);
	static Matrix Zeros(const std::vector<size_t> &dims);
	static Matrix Ones(size_t S0, size_t S1 = 1, size_t S2 = 1, size_t S3 = 1);
	static Matrix Ones(const std::vector<size_t> &dims);
	static Matrix Eye(size_t S0, size_t S1, size_t S2 = 1, size_t S3 = 1);
	static Matrix Eye(const std::vector<size_t> &dims);
	static Matrix Value(double value, size_t S0, size_t S1 = 1, size_t S2 = 1,
			size_t S3 = 1);
	static Matrix Value(double value, const std::vector<size_t> &dims);
	static Matrix Diag(const Matrix &other);
	static Matrix Diag(const Matrix &other, size_t S0, size_t S1);
	static Matrix Diag(const Matrix &other, const std::vector<size_t> &dims);

	static Matrix Vandermonde(const std::vector<double> &x,
			size_t polynomialOrder);

	/**\}
	 * \name Construction of matrices and size manipulation
	 * \{
	 */
	Matrix() = default;
	virtual ~Matrix() = default;
	explicit Matrix(size_t S0, size_t S1 = 1, size_t S2 = 1, size_t S3 = 1);
	explicit Matrix(const std::string &name, size_t S0 = 0, size_t S1 = 1,
			size_t S2 = 1, size_t S3 = 1);
	explicit Matrix(const std::vector<size_t> &dims);
	explicit Matrix(const std::string &name, const std::vector<size_t> &dims);

	void Reset();
	bool IsEmpty() const;

	void SetSize(const size_t S0, size_t S1 = 1, size_t S2 = 1, size_t S3 = 1);
	void SetSize(const std::vector<size_t> &dims);
	void SetSize(const Matrix &other);

	/**
	 * \brief Size of the matrix along one dimension.
	 *
	 * The index of the dimension is 0-based. Every dimension outside of the
	 * range of defined dimensions is returned as 1.
	 *
	 * A quirk of Octave/Matlab is, that, if the dimensions are undefined, the
	 * first two dimensions are returned as 0 and the rest as 1.
	 *
	 * A matrix can be defined as of dimensions 0, 100. Then these values would
	 * be reported as defined.
	 *
	 * \param dim Number of the dimension to return the size of. (0-based)
	 * \return Size of the matrix in the selected direction.
	 */
	size_t Size(const size_t dim) const;

	/**\brief Return a copy of the internal dimension vector
	 *
	 * This vector might be empty.
	 *
	 * This is the vector to be used, when generating derived matrices for
	 * mathematical operations:
	 * ´´´´´
	 * Matrix B = A - Matrix::Eye(A.Size());
	 * ´´´´´
	 *
	 * \return std::vector of size_t.
	 */
	const std::vector<size_t>& Size() const;

	/**\brief Return a vector of dimensions
	 *
	 * The vector has at least one element. If the matrix is empty, the element
	 * has the number 0.
	 *
	 * Trailing dimensions of size 1 are cut off.
	 *
	 * \return std::vector of size_t.
	 */
	std::vector<size_t> GetDimensions() const;

	/**\brief Return a squeezed vector of dimensions
	 *
	 * Return a minimized vector of dimensions: Every dimension of size 1 is
	 * removed. If one dimension is 0. The returned vector only contains 0.
	 *
	 * \return std::vector of size_t.
	 */
	std::vector<size_t> GetMinDimensions() const;

	/** \}
	 * \name Element access
	 *
	 * Next to the functions for linear element access provided by std::vector
	 * functions for multi-dimensional access are provided.
	 *
	 * ### Read/write Pointer to underlying double data array
	 *
	 * The raw double array inside the std::vector that holds the values
	 * for the matrix. Check the Numel() (or the .size()) function to find the
	 * number of values in the vector.
	 *
	 * To access the underlying data the .data() function can be used to get a
	 * pointer to the data. This should be rarely necessary as the operator []
	 * is exposed.
	 * \{
	 */

	/**
	 * \brief Move the insert-position around
	 *
	 * If called without parameter, the insert position is reset to the
	 * beginning of the matrix.
	 */
	void SetInsertPosition(const size_t p0 = 0, const size_t p1 = 0,
			const size_t p2 = 0, const size_t p3 = 0);
	void Insert(const double value);
	void Insert(const double value, const size_t p0, const size_t p1);
	void Insert(const double value, const size_t p0, const size_t p1,
			const size_t p2, const size_t p3 = 0);
	void Insert(const std::vector<double> &values_);
	void Insert(const double *value, const size_t count);
	void Insert(const double *value, const size_t count, const size_t p2);
	void Insert(const float *value, const size_t count);
	void Insert(const bool *value, const size_t count);

	/**\brief Check if the insert-position is past the last element
	 *
	 */
	bool IsFilled() const;

	/**
	 * \brief Number of elements
	 *
	 * This function should be used to find the number of elements in the
	 * std::vector inside every matrix. This is necessary when accessing the
	 * vector by a linear index operation or when accessing the double array
	 * in this class.
	 *
	 *
	 * \return Total number of elements in the matrix, identical to .size()
	 */
	size_t Numel() const;

	/** \}
	 * \name Manipulation
	 * \{
	 */

	Matrix& operator+=(const Matrix &b);
	friend Matrix operator+(Matrix a, const Matrix &b) {
		a += b;
		return a;
	}
	Matrix& operator-=(const Matrix &b);
	friend Matrix operator-(Matrix a, const Matrix &b) {
		a -= b;
		return a;
	}
	Matrix& operator*=(const Matrix &b);
	friend Matrix operator*(Matrix a, const Matrix &b) {
		a *= b;
		return a;
	}
	Matrix& operator+=(const double b);
	friend Matrix operator+(Matrix a, const double b) {
		a += b;
		return a;
	}
	Matrix& operator-=(const double b);
	friend Matrix operator-(Matrix a, const double b) {
		a -= b;
		return a;
	}
	Matrix& operator*=(const double b);
	friend Matrix operator*(Matrix a, const double b) {
		a *= b;
		return a;
	}
	Matrix& operator/=(const double b);
	friend Matrix operator/(Matrix a, const double b) {
		a += b;
		return a;
	}

	double& operator()(const size_t p0, const size_t p1 = 0,
			const size_t p2 = 0, const size_t p3 = 0); ///< Coordinate read/write access
	double operator()(const size_t p0, const size_t p1 = 0, const size_t p2 = 0,
			const size_t p3 = 0) const; ///< Coordinate read access

	void Squeeze();
	void Reshape(const size_t S1 = (size_t) -1, const size_t S2 = 1,
			const size_t S3 = 1, const size_t S4 = 1);
	void Reshape(std::vector<size_t> dims);

	Matrix T() const;
	void Transpose();

	bool Invert();

	/**
	 * \brief Pseudoinverse for independent columns
	 *
	 * Calculates inv(transpose(A)*A)*transpose(A)
	 *
	 * A matrix with independent columns and over or under determined rows
	 * can be inverted by this form of the Moore-Penrose Pseudoinverse.
	 *
	 * This is the form where every row contains a sample that need
	 * interpolation. The entries in the matrix are e.g. x*x, x, 1 to
	 * interpolate a parabel through a set of samples.
	 *
	 * There is a second form for the Pseudoinverse:
	 * transpose(A)*inv(A*transpose(A))
	 * This form is used for independent rows. It is not directly implemented
	 * and can be accomplished by transposing the matrix, calculating the
	 * Pseudoinverse and transposing the matrix again.
	 */
	void PseudoInvert();

	enum struct Mode {
		Remove, ///< Remove the rows/cols with the given index.
		Assign, ///< Assign the rows/cols from the rows/cols at the given index. One index can be used multiple times.
		AssignInverse ///< Assign zeros rows/cols at the given index in the resulting matrix. This is the inverse operation to Remove.
	};

	void MapRows(Mode mode, const std::vector<size_t> &rows);
	void MapCols(Mode mode, const std::vector<size_t> &cols);

private:
	std::vector<size_t> FillIndex(Mode mode, const std::vector<size_t> &x,
			size_t N) const;
public:

	void AlignAtZero(); ///< Shift the offset of the values in the matrix to align the smallest element at zero
	void Normalize(double max = 1.0); ///< Rescale the matrix to the value of max as the maximum value
	void Normalize(double min, double max); ///< Normalize the values to be between min and max

	enum class Axis {
		X, Y, Z
	};
	/**
	 * \brief Mirror Volume
	 *
	 * Mirror the volume along an axis.
	 */
	void Mirror(Axis a);

	/**
	 * \brief Rotate Volume
	 *
	 * Rotate the Volume around an axis. Some functions use this feature to
	 * access the model from all sides.
	 *
	 * @param a Axis of rotation (enum X,Y or Z).
	 * @param quarters Number of 90 degree turns to do.
	 */
	void Rotate(Axis a, int quarters);

	enum class Method {
		MaxValue, MinValue, MeanValue, Sum
	};
	Matrix XRay(Method method) const;

	/**\}
	 * \name Extract information
	 * \{
	 */

	double Norm() const; ///< Square-norm of the matrix.
	double Cond() const; ///< Conditional number for the matrix.
	Matrix Min(size_t dim = 0) const;
	Matrix Max(size_t dim = 0) const;
	double AllMin() const; ///< Minimum value of all elements in the matrix
	double AllMax() const; ///< Maximum value of all elements in the matrix
	double AllMaxAbs() const; ///< Maximum absolute value of all elements in the matrix

	/**
	 * \brief Octave/Matlab style output for 1-D or 2-D matrices
	 *
	 * The output is display row-wise: Each number in a row is separated by
	 * a comma, each row is separated by a semi-colon.
	 *
	 * When copied into Octave/Matlab, the matrix is interpreted/displayed
	 * correctly.
	 */
	friend std::ostream& operator<<(std::ostream &os, const Matrix &matrix) {
		const size_t N = matrix.Size(0);
		const size_t M = matrix.Size(1);
		os << "[";
		for (size_t n = 0; n < N; ++n) {
			if (n > 0)
				os << "; ";
			for (size_t m = 0; m < M; ++m) {
				if (m > 0)
					os << ", ";
				os << matrix(n, m);
			}
		}
		os << "]";
		return os;
	}

	/**\}
	 * \name Naming variables
	 *
	 * For storing in Octave/Matlab .mat files, an additional variable name
	 * is introduced.
	 * \{
	 */
private:
	std::string variablename;
public:
	void SetVariableName(const std::string &name);
	std::string GetVariableName() const;

	/**\}
	 */

};

#endif /* MATH_MATRIX_H */

