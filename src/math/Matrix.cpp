///////////////////////////////////////////////////////////////////////////////
// Name               : Matrix.cpp
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

#include "Matrix.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <iterator>
#include <stddef.h>
#include <stdexcept>

Matrix::Matrix(size_t S0, size_t S1, size_t S2, size_t S3) {
	SetSize(S0, S1, S2, S3);
}

Matrix::Matrix(const std::string &name, size_t S0, size_t S1, size_t S2,
		size_t S3) {
	SetVariableName(name);
	SetSize(S0, S1, S2, S3);
}

Matrix Matrix::Zeros(size_t S0, size_t S1, size_t S2, size_t S3) {
	Matrix temp(S0, S1, S2, S3);
	temp.values.assign(temp.values.size(), 0.0);
	return temp;
}

Matrix Matrix::Zeros(const Matrix &other) {
	Matrix temp(other);
	temp.values.assign(temp.values.size(), 0.0);
	return temp;
}

Matrix Matrix::Ones(size_t S0, size_t S1, size_t S2, size_t S3) {
	Matrix temp(S0, S1, S2, S3);
	temp.values.assign(temp.values.size(), 1.0);
	return temp;
}

Matrix Matrix::Ones(const Matrix &other) {
	Matrix temp(other);
	temp.values.assign(temp.values.size(), 1.0);
	return temp;
}

Matrix Matrix::Eye(size_t S0, size_t S1) {
	Matrix temp(S0, S1);
	temp.values.assign(temp.values.size(), 0.0);
	const size_t N = (S0 < S1) ? S0 : S1;
	const size_t dn = S0 + 1;
	for (size_t n = 0; n < (N * dn); n += dn)
		temp.values[n] = 1.0;
	return temp;
}

Matrix Matrix::Eye(const Matrix &other) {
	const size_t N = other.dimensions[0] * other.dimensions[1];
	if (N != other.values.size())
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Eye(const Matrix&): The other matrix does not have only two dimensions."));
	return Eye(other.dimensions[0], other.dimensions[1]);
}

Matrix Matrix::Value(double value, size_t S0, size_t S1, size_t S2, size_t S3) {
	Matrix temp(S0, S1, S2, S3);
	temp.values.assign(temp.values.size(), value);
	return temp;
}

Matrix Matrix::Value(double value, const Matrix &other) {
	Matrix temp(other);
	temp.values.assign(temp.values.size(), value);
	return temp;
}

Matrix Matrix::Diag(const Matrix &other) {
	const size_t M = other.Size(0);
	const size_t N = other.Size(1);
	if (N == 1 && M == 1)
		return other;
	if (M == 1 || N == 1)
		return Diag(other, N, M);
	const size_t P = (M < N) ? M : N;
	Matrix temp(P, 1);
	for (size_t i = 0; i < P; ++i)
		temp.values[i] = other(i, i);
	return temp;
}

Matrix Matrix::Diag(const Matrix &other, const size_t S0, const size_t S1) {
	Matrix temp = Matrix::Zeros(S0, S1);
	const size_t P = (S0 < S1) ? S0 : S1;
	const size_t Q = (other.Numel() < P) ? other.Numel() : P;
	for (size_t i = 0; i < Q; ++i)
		temp(i, i) = other.values[i];
	return temp;
}

Matrix Matrix::Vandermonde(const std::vector<double> &x, size_t order) {
	const size_t N = x.size();
	Matrix temp(N, order + 1);
	for (size_t n = 0; n < N; ++n)
		temp.values[n] = 1.0;
	for (size_t m = 1; m <= order; ++m) {
		for (size_t n = 0; n < N; ++n)
			temp[n + m * N] = temp[n + (m - 1) * N] * x[n];
	}
	return temp;
}

void Matrix::SetVariableName(const std::string &name) {
	this->variablename = name;
}

std::string Matrix::GetVariableName() const {
	return variablename;
}

void Matrix::Reset() {
	dimensions.resize(4, 1);
	dimensions[0] = 0;
	values.clear();
	bufferpos = 0;
}

bool Matrix::IsEmpty() const {
	return (values.empty());
}

void Matrix::SetSize(const size_t S0, const size_t S1, const size_t S2,
		const size_t S3) {
	dimensions.resize(4, 1);
	dimensions[0] = S0;
	dimensions[1] = S1;
	dimensions[2] = S2;
	dimensions[3] = S3;
	const size_t N = S0 * S1 * S2 * S3;
	values.resize(N, 0.0);
	bufferpos = 0;
}

void Matrix::SetSize(const Matrix &other) {
	this->SetSize(other.Size(0), other.Size(1), other.Size(2), other.Size(3));
}

void Matrix::SetSize(const std::vector<size_t> &dims) {
	size_t N = 1;
	for (size_t n = 0; n < dims.size(); ++n)
		N *= dims[n];
	dimensions = dims;
	values.resize(N, 0.0);
	bufferpos = 0;
}

size_t Matrix::Numel() const {
	return values.size();
}

size_t Matrix::Size(const size_t dim) const {
	// Copy of the Octave/Matlab behavior, but 0-based
	if (values.empty())
		return (dim > 0) ? 1 : 0;
	if (dim >= dimensions.size())
		return 1;
	return dimensions[dim];
}

std::vector<size_t> Matrix::Size() const {
	size_t L = dimensions.size();
	for (; L-- > 0;)
		if (dimensions[L] != 1)
			break;
	if (L == (size_t) -1)
		++L;
	std::vector<size_t> temp(L + 1);
	std::copy_n(dimensions.begin(), L + 1, temp.begin());
	return temp;
}

std::vector<size_t> Matrix::GetDimensions() const {
	return dimensions;
}

std::vector<size_t> Matrix::GetMinDimensions() const {
	std::vector<size_t> temp;
	for (size_t m = 0; m < dimensions.size(); m++)
		if (dimensions[m] != 1)
			temp.push_back(dimensions[m]);
	if (temp.empty() && !dimensions.empty())
		temp.push_back(1);
	return temp;
}

void Matrix::SetInsertPosition(const size_t p0, const size_t p1,
		const size_t p2, const size_t p3) {
	bufferpos = p0 + (p1 + (p2 + p3 * Size(2)) * Size(1)) * Size(0);
}

void Matrix::Insert(const double value) {
	if (bufferpos >= values.size())
		throw(std::logic_error(
				std::string(__FILE__) + ": Insert(double): Matrix full."));
	values[bufferpos++] = value;
}

void Matrix::Insert(const std::initializer_list<double> values_) {
	if (bufferpos + values_.size() > values.size())
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Insert(initalizer_list<double>): Matrix is full."));
	for (auto val = values_.begin(); val != values_.end(); ++val)
		values[bufferpos++] = *val;
}

void Matrix::Insert(const double value, const size_t p0, const size_t p1) {
	if (p0 >= this->Size(0))
		throw(std::out_of_range(
				std::string(__FILE__) + ": Insert(double,p0,p2): p0 >= S0."));
	if (p1 >= this->Size(1))
		throw(std::out_of_range(
				std::string(__FILE__) + ": Insert(double,p0,p1): p0 >= S1."));
	bufferpos = p0 + p1 * Size(0);
	values[bufferpos++] = value;
}

void Matrix::Insert(const double value, const size_t p0, const size_t p1,
		const size_t p2, const size_t p3) {
	if (p0 >= this->Size(0))
		throw(std::out_of_range(
				std::string(__FILE__)
						+ ": Insert(double,p0,p1,p2,p3): p0 >= S0."));
	if (p1 >= this->Size(1))
		throw(std::out_of_range(
				std::string(__FILE__)
						+ ": Insert(double,p0,p1,p2,p3): p1 >= S1."));
	if (p2 >= this->Size(2))
		throw(std::out_of_range(
				std::string(__FILE__)
						+ ": Insert(double,p0,p1,p2,p3): p2 >= S2."));
	if (p3 >= this->Size(3))
		throw(std::out_of_range(
				std::string(__FILE__)
						+ ": Insert(double,p0,p1,p2,p3): p3 >= S3."));
	bufferpos = p0 + (p1 + (p2 + p3 * Size(2)) * Size(1)) * Size(0);
	values[bufferpos++] = value;
}

void Matrix::Insert(const double *value, const size_t count) {
	if (bufferpos + count > values.size())
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Insert(double*,count): Matrix is full."));
	if (value == NULL)
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Insert(double*,count): value == NULL."));
	for (size_t i = 0; i < count; ++i)
		values[bufferpos++] = value[i];
}

void Matrix::Insert(const double *value, const size_t count, const size_t p1) {
	bufferpos = p1 * Size(0);
	if (bufferpos + count > values.size())
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Insert(col,double*,count): Matix full."));
	if (value == NULL)
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Insert(col,double*,count): value == NULL."));
	for (size_t i = 0; i < count; ++i)
		values[bufferpos++] = value[i];
}

void Matrix::Insert(const float *value, const size_t count) {
	if (bufferpos + count > values.size())
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Insert(float*,count): Matrix is full."));
	if (value == NULL)
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Insert(float*,count): value == NULL."));
	for (size_t i = 0; i < count; i++)
		values[bufferpos++] = (double) value[i];
}

void Matrix::Insert(const bool *value, const size_t count) {
	if (bufferpos + count > values.size())
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Insert(bool*,count): Matrix is full."));
	if (value == NULL)
		throw(std::logic_error(
				std::string(__FILE__) + ": Insert(bool*,count): value == NULL."));
	for (size_t i = 0; i < count; i++)
		values[bufferpos++] = value[i] ? 1.0 : 0.0;
}

bool Matrix::IsFilled() const {
	return bufferpos >= values.size();
}

double& Matrix::operator[](const size_t &index) {
	if (index >= values.size())
		throw(std::out_of_range(
				std::string(__FILE__) + ": operator[]: Access out of bounds!"));
	return values[index];
}

double Matrix::operator[](const size_t &index) const {
	if (index >= values.size())
		throw(std::out_of_range(
				std::string(__FILE__) + ": operator[]: Access out of bounds!"));
	return values[index];
}

double& Matrix::operator ()(const size_t p0, const size_t p1, const size_t p2,
		const size_t p3) {
	if (p0 >= this->Size(0))
		throw(std::out_of_range(
				std::string(__FILE__) + ": GetValue(p0,p1,p2,p3): p0 >= S0."));
	if (p1 >= this->Size(1))
		throw(std::out_of_range(
				std::string(__FILE__) + ": GetValue(p0,p1,p2,p3): p1 >= S1."));
	if (p2 >= this->Size(2))
		throw(std::out_of_range(
				std::string(__FILE__) + ": GetValue(p0,p1,p2,p3): p2 >= S2."));
	if (p3 >= this->Size(3))
		throw(std::out_of_range(
				std::string(__FILE__) + ": GetValue(p0,p1,p2,p3): p3 >= S3."));
	const size_t pos = p0 + (p1 + (p2 + p3 * Size(2)) * Size(1)) * Size(0);
	return values[pos];
}

double Matrix::operator ()(const size_t p0, const size_t p1, const size_t p2,
		const size_t p3) const {
	if (p0 >= this->Size(0))
		throw(std::out_of_range(
				std::string(__FILE__) + ": GetValue(p0,p1,p2,p3): p0 >= S0."));
	if (p1 >= this->Size(1))
		throw(std::out_of_range(
				std::string(__FILE__) + ": GetValue(p0,p1,p2,p3): p1 >= S1."));
	if (p2 >= this->Size(2))
		throw(std::out_of_range(
				std::string(__FILE__) + ": GetValue(p0,p1,p2,p3): p2 >= S2."));
	if (p3 >= this->Size(3))
		throw(std::out_of_range(
				std::string(__FILE__) + ": GetValue(p0,p1,p2,p3): p3 >= S3."));
	const size_t pos = p0 + (p1 + (p2 + p3 * Size(2)) * Size(1)) * Size(0);
	return values[pos];
}

double* Matrix::Pointer() {
	return values.data();
}

const double* Matrix::Pointer() const {
	return values.data();
}

void Matrix::Squeeze() {
	dimensions = GetMinDimensions();
}

void Matrix::Reshape(const size_t S0, const size_t S1, const size_t S2,
		const size_t S3) {
	unsigned int nrCalculatedDimensions = 0;
	size_t newNumel = 1;
	if (S0 == (size_t) -1)
		nrCalculatedDimensions++;
	else
		newNumel *= S0;
	if (S1 == (size_t) -1)
		nrCalculatedDimensions++;
	else
		newNumel *= S1;
	if (S2 == (size_t) -1)
		nrCalculatedDimensions++;
	else
		newNumel *= S2;
	if (S3 == (size_t) -1)
		nrCalculatedDimensions++;
	else
		newNumel *= S3;

	if (nrCalculatedDimensions > 1)
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Reshape: More than one dimension is set to -1."));
	if (newNumel > values.size())
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Reshape: The new dimensions describe a matrix with more values than before."));
	if (nrCalculatedDimensions == 0 && newNumel != values.size())
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Reshape: The new dimensions describe a different number of values than contained in the matrix."));
	if (nrCalculatedDimensions > 0) {
		if (values.size() % newNumel != 0)
			throw(std::logic_error(
					std::string(__FILE__)
							+ ": Reshape: The new dimensions do not fit without remainder."));
	}
	size_t calculatedDimension = values.size() / newNumel;
	dimensions.resize(4);
	if (S0 == 0)
		dimensions[0] = calculatedDimension;
	else
		dimensions[0] = S0;
	if (S1 == 0)
		dimensions[1] = calculatedDimension;
	else
		dimensions[1] = S1;
	if (S2 == 0)
		dimensions[2] = calculatedDimension;
	else
		dimensions[2] = S2;
	if (S3 == 0)
		dimensions[3] = calculatedDimension;
	else
		dimensions[3] = S3;
}

void Matrix::Reshape(const std::vector<size_t> &dims) {
	size_t N = 1;
	for (size_t n = 0; n < dims.size(); n++)
		N *= dims[n];
	if (N != values.size()) {
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Reshape(vector<size_t>): New shape has wrong number of elements."));
	}
	dimensions = dims;
}

void Matrix::Transpose() {
	*this = T();
}

Matrix Matrix::T() const {
	if (dimensions.size() == 0)
		return *this;
	Matrix temp;
	temp.SetSize(*this);
	if (temp.dimensions.size() == 1)
		temp.dimensions.push_back(1);

	if (values.size() > 0) {
		const size_t D1 = Size(0);
		const size_t D2 = Size(1);
		const size_t D12 = D1 * D2;
		size_t Dr = 1;
		for (size_t n = 2; n < dimensions.size(); n++)
			Dr *= dimensions[n];

		size_t pFrom = 0;
		size_t pTo2 = 0;
		for (size_t p3 = 0; p3 < Dr; p3++) {
			size_t pTo = pTo2;
			const size_t limit = pTo + D12;
			for (size_t p2 = 0; p2 < D12; p2++) {
				temp.values[pTo] = values[pFrom++];
				pTo += D2;
				if (pTo >= limit)
					pTo = pTo - D12 + 1;
			}
			pTo2 += D12;
		}
	}

	size_t h1 = temp.Size(0);
	size_t h2 = temp.Size(1);
	if (temp.dimensions.size() < 2)
		temp.dimensions.resize(2);
	temp.dimensions[0] = h2;
	temp.dimensions[1] = h1;
	return temp;
}

bool Matrix::Invert() {
	const size_t N = Size(0);
	const size_t M = Size(1);
	if (N * M != values.size())
		throw std::logic_error(
		__FILE__ "Matrix::Invert - Only 2D-matrices can be inverted.");
	if (N != M)
		throw std::logic_error(
				__FILE__ "Matrix::Invert - Only square matrices can be inverted (maybe use PseudoInvert().");
	std::vector<size_t> p(N, 0);
	for (size_t k = 0; k < N; ++k) {
		double max = 0.0;
		for (size_t i = k; i < N; ++i) {
			double s = 0.0;
			for (size_t j = k; j < N; ++j)
				s += fabs(values[i + j * N]);
			const double q = fabs(values[i + k * N]) / s;
			if (q > max) {
				max = q;
				p[k] = i;
			}
		}
		if (max == 0.0)
			return false;
		if (p[k] != k) {
			for (size_t j = 0; j < N; ++j)
				std::swap(values[k + j * N], values[p[k] + j * N]);
		}
		const double pivot = values[k + k * N];
		for (size_t j = 0; j < N; ++j) {
			if (j != k) {
				values[k + j * N] /= -pivot;
				for (size_t i = 0; i < N; ++i)
					if (i != k)
						values[i + j * N] += values[i + k * N]
								* values[k + j * N];
			}
		}
		for (size_t i = 0; i < N; ++i)
			values[i + k * N] /= pivot;
		values[k + k * N] = 1.0 / pivot;
	}
	for (size_t k = (N - 1); k-- > 0;) {
		if (p[k] != k) {
			for (size_t i = 0; i < N; ++i)
				std::swap(values[i + k * N], values[i + p[k] * N]);
		}
	}
	return true;
}

void Matrix::PseudoInvert() {
	const size_t N = Size(0);
	const size_t M = Size(1);
	if (N * M != values.size())
		throw std::logic_error(
				__FILE__ "Matrix::PseudoInvert - Only 2D-matrices can be (pseudo-)inverted.");
	Matrix A = *this;
	Matrix H(M, M);
	H.values.assign(M * M, 0.0);
	for (size_t m1 = 0; m1 < M; ++m1)
		for (size_t m0 = 0; m0 < M; ++m0)
			for (size_t n = 0; n < N; ++n)
				H.values[m0 + m1 * M] += values[n + m0 * N]
						* values[n + m1 * N];
	H.Invert();
	dimensions[0] = M;
	dimensions[1] = N;
	values.assign(N * M, 0.0);
	for (size_t m1 = 0; m1 < M; ++m1)
		for (size_t m0 = 0; m0 < M; ++m0)
			for (size_t n = 0; n < N; ++n)
				values[m0 + n * M] += H.values[m0 + m1 * M]
						* A.values[n + m1 * N];
}

double Matrix::Min() const {
	double m = DBL_MAX;
	for (size_t n = 0; n < values.size(); n++) {
		const double v = values[n];
		if (v == DBL_MAX || v == -DBL_MAX)
			continue;
		m = fmin(m, v);
	}
	return m;
}

double Matrix::Max() const {
	double m = -DBL_MAX;
	for (size_t n = 0; n < values.size(); n++) {
		const double v = values[n];
		if (v == DBL_MAX || v == -DBL_MAX)
			continue;
		m = fmax(m, v);
	}
	return m;
}

double Matrix::MaxAbs() const {
	double v = -DBL_MAX;
	for (size_t n = 0; n < values.size(); n++)
		v = fmax(v, fabs(values[n]));
	return v;
}

void Matrix::AlignAtZero() {
	const double m = this->Min();
	for (size_t n = 0; n < values.size(); n++)
		values[n] -= m;
}

void Matrix::Normalize(double max) {
	double m = this->MaxAbs();
	if (m == 0.0)
		return;
	m = max / m;
	for (size_t n = 0; n < values.size(); n++)
		values[n] *= m;
}

void Matrix::Normalize(double min, double max) {
	if (max <= min) {
		values.assign(values.size(), (min + max) / 2.0);
		return;
	}
	this->AlignAtZero();
	double m = this->Max();
	if (m == 0.0)
		return;
	m = (max - min) / m;
	for (size_t n = 0; n < values.size(); ++n)
		values[n] = values[n] * m + min;
}

void Matrix::Mirror(Axis a) {
	const size_t Nx = Size(0);
	const size_t Ny = Size(1);
	const size_t Nz = Size(2);

	switch (a) {
	case Axis::X:
		for (size_t i = 0; i < (Nx / 2); i++) {
			for (size_t j = 0; j < Ny; j++) {
				for (size_t k = 0; k < Nz; k++) {
					const size_t n = i + Nx * (j + Ny * k);
					const size_t m = (Nx - i - 1) + Nx * (j + Ny * k);
					std::swap(values[n], values[m]);
				}
			}
		}
		break;
	case Axis::Y:
		for (size_t i = 0; i < Nx; i++) {
			for (size_t j = 0; j < (Ny / 2); j++) {
				for (size_t k = 0; k < Nz; k++) {
					const size_t n = i + Nx * (j + Ny * k);
					const size_t m = i + Nx * ((Ny - j - 1) + Ny * k);
					std::swap(values[n], values[m]);
				}
			}
		}
		break;
	case Axis::Z:
		for (size_t i = 0; i < Nx; i++) {
			for (size_t j = 0; j < Ny; j++) {
				for (size_t k = 0; k < (Nz / 2); k++) {
					const size_t n = i + Nx * (j + Ny * k);
					const size_t m = i + Nx * (j + Ny * (Nz - k - 1));
					std::swap(values[n], values[m]);
				}
			}
		}
		break;
	}
}

void Matrix::Rotate(Axis a, int quarters) {
	const size_t Nx = Size(0);
	const size_t Ny = Size(1);
	const size_t Nz = Size(2);

	quarters %= 4;
	if (quarters < 0)
		quarters += 4;
	if (quarters == 0)
		return;
	if (quarters == 2) {
		switch (a) {
		case Axis::X:
			for (size_t i = 0; i < Nx; i++) {
				for (size_t n = i; n < (values.size() / 2); n += Nx) {
					std::swap(values[n],
							values[(values.size() - Nx) - n + 2 * i]);
				}
			}
			break;
		case Axis::Y:
			for (size_t j = 0; j < Ny; j++) {
				size_t n = j * Nx;
				size_t m = values.size() - 1 - (Ny - 1) * Nx + j * Nx;
				for (size_t k = 0; k < (Nx * Nz / 2); k++) {
					std::swap(values[n], values[m]);
					n = (n + Nx * Ny) % (values.size() - 1);
					m = (m + (values.size() - Nx * Ny - 1))
							% (values.size() - 1);
				}
			}
			break;
		case Axis::Z:
			const size_t m = Nx * Ny;
			for (size_t k = 0; k < Nz; k++) {
				for (size_t n = 0; n < (m / 2); n++) {
					std::swap(values[k * Nx * Ny + n],
							values[k * Nx * Ny + (m - n - 1)]);
				}
			}
			break;
		}
		return;
	}
	if (quarters != 1 && quarters != 3)
		throw(std::logic_error(
				std::string(__FILE__)
						+ ": Rotate(Axis,int): Rotation in the wrong quadrant."));
	std::vector<double> temp;
	temp.resize(values.size());

	switch (a) {
	case Axis::X: {
		if (quarters == 1) {
			const size_t mo = values.size() + Nx;
			const size_t ad = values.size() - (Nx * (Ny - 1));
			for (size_t i = 0; i < Nx; i++) {
				size_t m = values.size() - Ny * Nx + i;
				for (size_t n = 0; n < (Ny * Nz); n++) {
					temp[n * Nx + i] = values[m];
					m = (m + ad) % mo;
				}
			}
		}
		if (quarters == 3) {
			const size_t mo = values.size() + Nx;
			const size_t ad = Nx * Ny;
			for (size_t i = 0; i < Nx; i++) {
				size_t m = (Ny - 1) * Nx + i;
				for (size_t n = 0; n < (Ny * Nz); n++) {
					temp[n * Nx + i] = values[m];
					m = (m + ad) % mo;
				}
			}
		}

//		std::swap(Ny, Nz);
		Matrix::Reshape(Nx, Nz, Ny);
		break;
	}
	case Axis::Y: {
		if (quarters == 1) {
//			Nx = 4;
//			Ny = 5;
//			Nz = 5;
//			values.size() = Nx * Ny * Nz;
			size_t n = 0;
			size_t m = Nx - 1;
			for (size_t i = 0; i < Nx; i++) {
				for (size_t j = 0; j < Ny; j++) {
					for (size_t k = 0; k < Nz; k++) {
						temp[n] = values[m];
						n++;
						m += Nx * Ny;
					}
					m = m + Nx - values.size();
				}
				m = m - Nx * Ny - 1;
			}
		}
		if (quarters == 3) {
			size_t n = 0;
			size_t m = values.size();
			for (size_t i = 0; i < Nx; i++) {
				for (size_t j = 0; j < Ny; j++) {
					for (size_t k = 0; k < Nz; k++) {
						m -= Nx * Ny;
						temp[n] = values[m];
						n++;
					}
					m = m + values.size() + Nx;
				}
				m = m - Nx * Ny + 1;
			}
		}
		Matrix::Reshape(Nz, Ny, Nx);
//		std::swap(Nz, Nx);
		break;
	}
	case Axis::Z: {
		const size_t mo = Nx * Ny + 1;
		size_t ad;
		if (quarters == 1)
			ad = Nx * (Ny - 1) + 1;
		if (quarters == 3)
			ad = Nx;
		for (size_t k = 0; k < Nz; k++) {
			size_t n = 0;
			for (size_t m = ad - 1; m != mo - 1; m = (m + ad) % mo) {
				temp[n + k * Nx * Ny] = values[m + k * Nx * Ny];
				n++;
			}
		}
//		std::swap(Nx, Ny);
		Matrix::Reshape(Ny, Nx, Nz);
		break;
	}
	}
	values.swap(temp);
}

Matrix& Matrix::operator +=(const Matrix &b) {
	auto D0 = Size();
	auto D1 = b.Size();
	if (D0 != D1)
		throw std::runtime_error(
				"Matrix::operator += : The dimensions of both matrices are not equal.");
	for (size_t i = 0; i < Numel(); ++i)
		values[i] += b.values[i];
	return *this;
}

Matrix& Matrix::operator -=(const Matrix &b) {
	auto D0 = Size();
	auto D1 = b.Size();
	if (D0 != D1)
		throw std::runtime_error(
				"Matrix::operator -= : The dimensions of both matrices are not equal.");
	for (size_t i = 0; i < Numel(); ++i)
		values[i] -= b.values[i];
	return *this;
}

Matrix& Matrix::operator *=(const Matrix &b) {
	const size_t N = Size(0);
	const size_t M = Size(1);
	if (N * M != Numel())
		throw std::logic_error(
		__FILE__ "Matrix::operator*= - Only 2D-matrices can be multiplied.");
	if (M != b.Size(0))
		throw std::logic_error(
				__FILE__ "Matrix::operator*= - The right matrix does not have as many rows as the left matrix has columns.");
	const size_t P = b.Size(1);
	if (P * M != b.Numel())
		throw std::logic_error(
		__FILE__ "Matrix::operator*= - Only 2D-matrices can be multiplied.");
	const Matrix A = *this;
	this->SetSize(N, P);
	this->values.assign(N * P, 0.0);
	for (size_t n = 0; n < N; ++n)
		for (size_t m = 0; m < M; ++m)
			for (size_t p = 0; p < P; ++p) {
				values[n + p * N] += A.values[n + m * N] * b.values[m + p * M];
			}
	return *this;
}

Matrix Matrix::XRay(Method method) const {
	const size_t Nx = Size(0);
	const size_t Ny = Size(1);
	const size_t Nz = Size(2);

	Matrix temp;
	temp.SetSize(Nx, 1, Nz);
	temp.SetInsertPosition(0);

	for (size_t z = 0; z < Nz; z++) {
		for (size_t x = 0; x < Nx; x++) {
			switch (method) {
			case Method::MaxValue: {
				double vm = -DBL_MAX;
				for (size_t y = 0; y < Ny; y++) {
					const double v = Matrix::operator()(x, y, z);
					if (v > vm)
						vm = v;
				}
				temp.Insert(vm);
				break;
			}
			case Method::MinValue: {
				double vm = DBL_MAX;
				for (size_t y = 0; y < Ny; y++) {
					const double v = Matrix::operator()(x, y, z);
					if (v < vm)
						vm = v;
				}
				temp.Insert(vm);
				break;
			}
			case Method::MeanValue: {
				double vm = 0;
				for (size_t y = 0; y < Ny; y++) {
					const double v = Matrix::operator()(x, y, z);
					vm += v;
				}
				temp.Insert(vm / (double) Ny);
				break;
			}
			case Method::Sum: {
				double vm = 0;
				for (size_t y = 0; y < Ny; y++) {
					const double v = Matrix::operator()(x, y, z);
					vm += v;
				}
				temp.Insert(vm);
				break;
			}
			}
		}
	}
	return temp;
}

double Matrix::Norm() const {
	double res = 0.0;
	for (const auto &v : values)
		res += v * v;
	return res;
}

double Matrix::Cond() const {
	Matrix temp = *this;
	temp.PseudoInvert();
	return Norm() * temp.Norm();
}

std::vector<size_t> Matrix::FillIndex(Mode mode, const std::vector<size_t> &x,
		size_t N) const {
	std::vector<size_t> idx;
	std::vector<size_t> xs(x);
	if (mode == Mode::Remove || mode == Mode::AssignInverse) {
		std::sort(xs.begin(), xs.end());
		auto last = std::unique(xs.begin(), xs.end());
		xs.erase(last, xs.end());
	}
	const size_t M = xs.size();
	size_t j = 0;
	switch (mode) {
	case Mode::Remove:
		idx.reserve(N - M);
		for (size_t i = 0; i < N; ++i)
			if (j < M && i == xs[j]) {
				++j;
			} else {
				idx.push_back(i);
			}
		break;
	case Mode::Assign:
		idx = x;
		break;
	case Mode::AssignInverse:
		idx.reserve(N + M);
		for (size_t i = 0; i < (N + M); ++i) {
			if (j < M && i == xs[j]) {
				idx.push_back((size_t) -1);
				++j;
			} else {
				idx.push_back(i - j);
			}
		}
		break;
	}
	return idx;
}

void Matrix::MapRows(Mode mode, const std::vector<size_t> &rows) {
	const size_t N = Size(0);
	std::vector<size_t> idx = FillIndex(mode, rows, N);
	std::vector<double> temp(values);
	SetSize(idx.size(), Size(1));
	size_t offs = 0;
	for (size_t j = 0; j < dimensions[1]; ++j) {
		for (size_t i = 0; i < dimensions[0]; ++i) {
			if (idx[i] == (size_t) -1) {
				values[offs] = 0;
			} else {
				values[offs] = temp[j * N + idx[i]];
			}
			++offs;
		}
	}
}

void Matrix::MapCols(Mode mode, const std::vector<size_t> &cols) {
	const size_t M = Size(0);
	const size_t N = Size(1);
	std::vector<size_t> idx = FillIndex(mode, cols, N);
	std::vector<double> temp(values);
	SetSize(Size(0), idx.size());
	size_t offsDst = 0;
	for (size_t i = 0; i < dimensions[1]; ++i) {

		if (idx[i] == (size_t) -1) {
			std::memset(values.data() + offsDst, 0, M * sizeof(double));
		} else {
			const size_t offsSrc = idx[i] * M;
			std::memcpy(values.data() + offsDst, temp.data() + offsSrc,
					M * sizeof(double));
		}
		offsDst += M;
	}
}

