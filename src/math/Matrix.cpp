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
#include <sstream>
#include <iterator>
#include <stddef.h>
#include <stdexcept>

#define ERROR(errortxt) { \
		std::ostringstream err; \
		err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ": "; \
		err << errortxt; \
		throw std::runtime_error(err.str()); \
	}

Matrix Matrix::Zeros(size_t S0, size_t S1, size_t S2, size_t S3) {
	return Zeros( { S0, S1, S2, S3 });
}

Matrix Matrix::Zeros(const std::vector<size_t> &dims) {
	Matrix temp(dims);
	temp.assign(temp.size(), 0.0);
	return temp;
}

Matrix Matrix::Ones(size_t S0, size_t S1, size_t S2, size_t S3) {
	return Ones( { S0, S1, S2, S3 });
}

Matrix Matrix::Ones(const std::vector<size_t> &dims) {
	Matrix temp(dims);
	temp.assign(temp.size(), 1.0);
	return temp;
}

Matrix Matrix::Eye(size_t S0, size_t S1, size_t S2, size_t S3) {
	return Eye( { S0, S1, S2, S3 });
}

Matrix Matrix::Eye(const std::vector<size_t> &dims) {

// TODO Add this check
//	const size_t N = other.counts[0] * other.counts[1];
//		if (N != other.Numel())
//			ERROR("The other matrix does not have only two dimensions.");
//		return Eye( { other.counts[0], other.counts[1] });

	Matrix temp(dims);
	temp.assign(temp.size(), 0.0);
	size_t S0 = temp.Size(0);
	size_t S1 = temp.Size(1);
	const size_t N = (S0 < S1) ? S0 : S1;
	const size_t dn = S0 + 1;
	for (size_t n = 0; n < (N * dn); n += dn)
		temp[n] = 1.0;
	return temp;
}

Matrix Matrix::Value(double value, size_t S0, size_t S1, size_t S2, size_t S3) {
	return Value(value, { S0, S1, S2, S3 });
}

Matrix Matrix::Value(double value, const std::vector<size_t> &dims) {
	Matrix temp(dims);
	temp.assign(temp.size(), value);
	return temp;
}

Matrix Matrix::Diag(const Matrix &other, const std::vector<size_t> &dims) {
	const size_t S0 = (dims.size() > 0) ? dims[0] : 0;
	const size_t S1 = (dims.size() > 1) ? dims[1] : 1;
	return Diag(other, S0, S1);
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
		temp[i] = other(i, i);
	return temp;
}

Matrix Matrix::Diag(const Matrix &other, const size_t S0, const size_t S1) {
	Matrix temp = Matrix::Zeros(S0, S1);
	const size_t P = (S0 < S1) ? S0 : S1;
	const size_t Q = (other.Numel() < P) ? other.Numel() : P;
	for (size_t i = 0; i < Q; ++i)
		temp(i, i) = other[i];
	return temp;
}

Matrix Matrix::Vandermonde(const std::vector<double> &x,
		size_t polynominalOrder) {
	const size_t N = x.size();
	Matrix temp(N, polynominalOrder + 1);
	for (size_t n = 0; n < N; ++n)
		temp[n] = 1.0;
	for (size_t m = 1; m <= polynominalOrder; ++m) {
		for (size_t n = 0; n < N; ++n)
			temp[n + m * N] = temp[n + (m - 1) * N] * x[n];
	}
	return temp;
}

Matrix::Matrix(size_t S0, size_t S1, size_t S2, size_t S3) {
	SetSize(S0, S1, S2, S3);
}

Matrix::Matrix(const std::string &name, size_t S0, size_t S1, size_t S2,
		size_t S3) {
	SetVariableName(name);
	SetSize(S0, S1, S2, S3);
}

Matrix::Matrix(const std::vector<size_t> &dims) {
	SetSize(dims);
}

Matrix::Matrix(const std::string &name, const std::vector<size_t> &dims) {
	SetVariableName(name);
	SetSize(dims);
}

void Matrix::Reset() {
	counts.clear();
	strides.clear();
	order = Order::NORMAL;
	clear();
	bufferpos = 0;
}

bool Matrix::IsEmpty() const {
	return (empty());
}

void Matrix::SetSize(const size_t S0, const size_t S1, const size_t S2,
		const size_t S3) {
	SetSize(std::vector<size_t> { S0, S1, S2, S3 });
}

void Matrix::SetSize(const Matrix &other) {
	SetSize(other.Size());
}

void Matrix::SetSize(const std::vector<size_t> &dims) {
	size_t N = (dims.empty()) ? 0 : 1;
	for (const size_t d : dims)
		N *= d;
	counts = dims;
	strides = CalculateStrides(order);
	resize(N, 0.0);
	bufferpos = 0;
}

size_t Matrix::Numel() const {
	return size();
}

size_t Matrix::Size(const size_t dim) const {
	// Copy of the Octave/Matlab behavior, but 0-based
	// If the dimensions are empty return 0 for dim=0 and dim=1 and 1 otherwise.
	if (counts.empty())
		return (dim > 1) ? 1 : 0;
	if (dim >= counts.size())
		return 1;
	return counts[dim];
}

const std::vector<size_t>& Matrix::Size() const {
	return counts;
}

std::vector<size_t> Matrix::GetDimensions() const {
	size_t L = counts.size();
	for (; L-- > 0;)
		if (counts[L] != 1)
			break;
	if (L == (size_t) -1)
		++L;
	std::vector<size_t> temp(L + 1, 0);
	std::copy_n(counts.begin(), L + 1, temp.begin());
	return temp;
}

std::vector<size_t> Matrix::GetMinDimensions() const {
	std::vector<size_t> temp;
	for (size_t m = 0; m < counts.size(); m++)
		if (counts[m] != 1)
			temp.push_back(counts[m]);
	if (temp.empty())
		temp.push_back((counts.empty()) ? 0 : 1);
	return temp;
}

void Matrix::ReorderDimensions(Order newOrder) {
	if (order == newOrder)
		return;
	Matrix temp = *this;
	order = newOrder;
	strides = CalculateStrides(newOrder);
	const size_t N = size();
	for (size_t n = 0; n < N; n++) {
		size_t m = Sub2Ind(temp.Ind2Sub(n));
		operator[](m) = temp[n];
	}
}

size_t Matrix::Sub2Ind(size_t p0, size_t p1, size_t p2, size_t p3) const {
	return Sub2Ind( { p0, p1, p2, p3 });
}

size_t Matrix::Sub2Ind(const std::vector<size_t> &pos) const {
	size_t v = 0;
	if (pos.size() > strides.size()) {
		auto it = pos.begin();
		for (const size_t sv : strides)
			v += sv * *(it++);
		bool check = true;
		while (it != pos.end()) {
			check &= *(it++) <= 1;
		}
		if (!check)
			throw std::runtime_error(
					"Matrix::Sub2Ind - More position-indices passed than available dimensions.");
	} else {
		auto sv = strides.begin();
		for (const size_t p : pos)
			v += p * *(sv++);
	}
	return v;
}

std::vector<size_t> Matrix::Ind2Sub(const size_t idx) const {
	const size_t N = counts.size();
	std::vector<size_t> ret(N);
	for (size_t n = 0; n < N; n++)
		ret[n] = (idx / strides[n]) % counts[n];
	return ret;
}

void Matrix::SetInsertPosition(const size_t p0, const size_t p1,
		const size_t p2, const size_t p3) {
	bufferpos = Sub2Ind( { p0, p1, p2, p3 });
}

void Matrix::Insert(const double value) {
	if (bufferpos >= size())
		ERROR(
				"Matrix full, " << bufferpos << " have already been written. Additional " << size() << " values do not fit.");
	operator[](bufferpos++) = value;
}

void Matrix::Insert(const std::vector<double> &values_) {
	if (bufferpos + values_.size() > size())
		ERROR(
				"Matrix is full. The matrix can hold " << size() << " values. It already contains " << bufferpos << " values. Additional " << values_.size() << " values cannot be written.");
	for (auto val = values_.begin(); val != values_.end(); ++val)
		operator[](bufferpos++) = *val;
}

void Matrix::Insert(const double value, const size_t p0, const size_t p1) {
	if (p0 >= this->Size(0))
		ERROR(
				"Index p0 = " << p0 << " is larger than the dimension 0 of " << this->Size(0)<<".");
	if (p1 >= this->Size(1))
		ERROR(
				"Index p1 = " << p1 << " is larger than the dimension 1 of " << this->Size(1)<<".");
	bufferpos = Sub2Ind( { p0, p1 });
	operator[](bufferpos++) = value;
}

void Matrix::Insert(const double value, const size_t p0, const size_t p1,
		const size_t p2, const size_t p3) {
	if (p0 >= this->Size(0))
		ERROR(
				"Index p0 = " << p0 << " is larger than the dimension 0 of " << this->Size(0)<<".");
	if (p1 >= this->Size(1))
		ERROR(
				"Index p1 = " << p1 << " is larger than the dimension 1 of " << this->Size(1)<<".");
	if (p2 >= this->Size(2))
		ERROR(
				"Index p2 = " << p2 << " is larger than the dimension 2 of " << this->Size(2)<<".");
	if (p3 >= this->Size(3))
		ERROR(
				"Index p3 = " << p3 << " is larger than the dimension 3 of " << this->Size(3)<<".");
	bufferpos = Sub2Ind( { p0, p1, p2, p3 });
	operator[](bufferpos++) = value;
}

void Matrix::Insert(const double *value, const size_t count) {
	if (bufferpos + count > size())
		ERROR(
				"Matrix is full. The matrix can hold " << size() << " values. It already contains " << bufferpos << " values. Additional " << count << " values cannot be written.");
	if (value == nullptr)
		ERROR("The *value is a nullptr.");
	for (size_t i = 0; i < count; ++i)
		operator[](bufferpos++) = value[i];
}

void Matrix::Insert(const double *value, const size_t count, const size_t p1) {
	bufferpos = Sub2Ind( { 0, p1 });
	if (bufferpos + count > size())
		ERROR(
				"Matrix is full. The matrix can hold " << size() << " values. It already contains " << bufferpos << " values. Additional " << count << " values cannot be written.");
	if (value == nullptr)
		ERROR("The *value is a nullptr.");
	for (size_t i = 0; i < count; ++i)
		operator[](bufferpos++) = value[i];
}

void Matrix::Insert(const float *value, const size_t count) {
	if (bufferpos + count > size())
		ERROR(
				"Matrix is full. The matrix can hold " << size() << " values. It already contains " << bufferpos << " values. Additional " << count << " values cannot be written.");
	if (value == nullptr)
		ERROR("The *value is a nullptr.");
	for (size_t i = 0; i < count; i++)
		operator[](bufferpos++) = (double) value[i];
}

void Matrix::Insert(const bool *value, const size_t count) {
	if (bufferpos + count > size())
		ERROR(
				"Matrix is full. The matrix can hold " << size() << " values. It already contains " << bufferpos << " values. Additional " << count << " values cannot be written.");
	if (value == nullptr)
		ERROR("The *value is a nullptr.");
	for (size_t i = 0; i < count; i++)
		operator[](bufferpos++) = value[i] ? 1.0 : 0.0;
}

bool Matrix::IsFilled() const {
	return bufferpos >= size();
}

double& Matrix::operator ()(const size_t p0, const size_t p1, const size_t p2,
		const size_t p3) {
	if (p0 >= this->Size(0))
		ERROR(
				"Index p0 = " << p0 << " is larger than the dimension 0 of " << this->Size(0)<<".");
	if (p1 >= this->Size(1))
		ERROR(
				"Index p1 = " << p1 << " is larger than the dimension 1 of " << this->Size(1)<<".");
	if (p2 >= this->Size(2))
		ERROR(
				"Index p2 = " << p2 << " is larger than the dimension 2 of " << this->Size(2)<<".");
	if (p3 >= this->Size(3))
		ERROR(
				"Index p3 = " << p3 << " is larger than the dimension 3 of " << this->Size(3)<<".");
	const size_t pos = Sub2Ind( { p0, p1, p2, p3 });
	return operator[](pos);
}

double Matrix::operator ()(const size_t p0, const size_t p1, const size_t p2,
		const size_t p3) const {
	if (p0 >= this->Size(0))
		ERROR(
				"Index p0 = " << p0 << " is larger than the dimension 0 of " << this->Size(0)<<".");
	if (p1 >= this->Size(1))
		ERROR(
				"Index p1 = " << p1 << " is larger than the dimension 1 of " << this->Size(1)<<".");
	if (p2 >= this->Size(2))
		ERROR(
				"Index p2 = " << p2 << " is larger than the dimension 2 of " << this->Size(2)<<".");
	if (p3 >= this->Size(3))
		ERROR(
				"Index p3 = " << p3 << " is larger than the dimension 3 of " << this->Size(3)<<".");
	const size_t pos = Sub2Ind( { p0, p1, p2, p3 });
	return operator[](pos);
}

double& Matrix::At(const size_t p0, const size_t p1, const size_t p2,
		const size_t p3) {
	const size_t pos = Sub2Ind( { p0, p1, p2, p3 });
	return operator[](pos);
}

double Matrix::At(const size_t p0, const size_t p1, const size_t p2,
		const size_t p3) const {
	const size_t pos = Sub2Ind( { p0, p1, p2, p3 });
	return operator[](pos);
}

void Matrix::Squeeze() {
	SetSize(GetMinDimensions());
}

void Matrix::Reshape(const size_t S0, const size_t S1, const size_t S2,
		const size_t S3) {
	std::vector<size_t> newdim = { S0, S1, S2, S3 };
	Reshape(newdim);
}

void Matrix::Reshape(std::vector<size_t> dims) {
	size_t N = 1;
	size_t idx = (size_t) -1;
	for (size_t n = 0; n < dims.size(); n++) {
		if (dims[n] == (size_t) -1) {
			if (idx != (size_t) -1)
				ERROR(
						"Only one dimension can be set to auto-calculate. The dimension " << idx << " is already selected, thus dimension " << n << " has to have a fixed value.");
			idx = n;
		} else {
			N *= dims[n];
		}
	}
	if (N > size())
		ERROR(
				"The fixed elements already have more element than the original matrix (Original: "<<size()<<", fixed: "<<N <<").");
	if (idx == (size_t) -1 && N != size())
		ERROR(
				"New shape has wrong number of elements. "<< size() << " elements cannot be shaped into " << N<< " elements.");
	if (idx != (size_t) -1) {
		if (size() % N != 0)
			ERROR(
					"The new dimensions do not fit without a remainder. " << size() << " are not evenly divisible by " << N<<".");
		dims[idx] = size() / N;
	}
	SetSize(dims);
}

void Matrix::Transpose() {
	*this = T();
}

Matrix Matrix::T() const {
	if (counts.empty())
		return *this;
	Matrix temp = *this;
	std::vector<size_t> dims = Size();
	if (dims.size() == 1)
		dims.push_back(1);
	switch (order) {
	default:
		std::swap(dims[0], dims[1]);
		temp.order = Order::TWO_REVERSED;
		break;
	case Order::REVERSED:
		temp.ReorderDimensions(Order::NORMAL);
		std::swap(dims[0], dims[1]);
		temp.order = Order::TWO_REVERSED;
		break;
	case Order::TWO_REVERSED:
		std::swap(dims[0], dims[1]);
		temp.order = Order::NORMAL;
		break;
	}
	temp.SetSize(dims);
	temp.ReorderDimensions();
	return temp;
}

bool Matrix::Invert() {
	const size_t N = Size(0);
	const size_t M = Size(1);
	if (N * M != size())
		ERROR("Only 2D-matrices can be inverted.");
	if (N != M)
		ERROR("Only square matrices can be inverted (maybe use PseudoInvert().");
	std::vector<size_t> p(N, 0);
	for (size_t k = 0; k < N; ++k) {
		double max = 0.0;
		for (size_t i = k; i < N; ++i) {
			double s = 0.0;
			for (size_t j = k; j < N; ++j)
				s += fabs(operator[](i + j * N));
			const double q = fabs(operator[](i + k * N)) / s;
			if (q > max) {
				max = q;
				p[k] = i;
			}
		}
		if (fabs(max) <= DBL_EPSILON)
			return false;
		if (p[k] != k) {
			for (size_t j = 0; j < N; ++j)
				std::swap(operator[](k + j * N), operator[](p[k] + j * N));
		}
		const double pivot = operator[](k + k * N);
		for (size_t j = 0; j < N; ++j) {
			if (j != k) {
				operator[](k + j * N) /= -pivot;
				for (size_t i = 0; i < N; ++i)
					if (i != k)
						operator[](i + j * N) += operator[](i + k * N)
								* operator[](k + j * N);
			}
		}
		for (size_t i = 0; i < N; ++i)
			operator[](i + k * N) /= pivot;
		operator[](k + k * N) = 1.0 / pivot;
	}
	for (size_t k = (N - 1); k-- > 0;) {
		if (p[k] != k) {
			for (size_t i = 0; i < N; ++i)
				std::swap(operator[](i + k * N), operator[](i + p[k] * N));
		}
	}
	return true;
}

void Matrix::PseudoInvert() {
	const size_t N = Size(0);
	const size_t M = Size(1);
	if (N * M != size())
		ERROR("Only 2D-matrices can be (pseudo-)inverted.");
	Matrix A = *this;
	Matrix H(M, M);
	H.assign(M * M, 0.0);
	for (size_t m1 = 0; m1 < M; ++m1)
		for (size_t m0 = 0; m0 < M; ++m0)
			for (size_t n = 0; n < N; ++n)
				H[m0 + m1 * M] += operator[](n + m0 * N)
						* operator[](n + m1 * N);
	H.Invert();
	SetSize(M, N);
	assign(N * M, 0.0);
	for (size_t m1 = 0; m1 < M; ++m1)
		for (size_t m0 = 0; m0 < M; ++m0)
			for (size_t n = 0; n < N; ++n)
				operator[](m0 + n * M) += H[m0 + m1 * M] * A[n + m1 * N];
}

Matrix Matrix::Min(size_t dim) const {
	Matrix ret;
	std::vector<size_t> d = Size();
	if (d.empty() || dim >= d.size())
		return *this;
	d[dim] = 1;
	ret = Matrix::Value(DBL_MAX, d);

	size_t idx = 0;
	for (size_t n3 = 0; n3 < Size(3); ++n3)
		for (size_t n2 = 0; n2 < Size(2); ++n2)
			for (size_t n1 = 0; n1 < Size(1); ++n1)
				for (size_t n0 = 0; n0 < Size(0); ++n0) {
					const size_t m0 = (dim == 0) ? 0 : n0;
					const size_t m1 = (dim == 1) ? 0 : n1;
					const size_t m2 = (dim == 2) ? 0 : n2;
					const size_t m3 = (dim == 3) ? 0 : n3;
					const double v = std::fmin(ret(m0, m1, m2, m3),
							operator[](idx));
					ret.Insert(v, m0, m1, m2, m3);
					idx++;
				}
	return ret;
}

Matrix Matrix::Max(size_t dim) const {
	Matrix ret;
	std::vector<size_t> d = Size();
	if (d.empty() || dim >= d.size())
		return *this;
	d[dim] = 1;
	ret = Matrix::Value(-DBL_MAX, d);

	size_t idx = 0;
	for (size_t n3 = 0; n3 < Size(3); ++n3)
		for (size_t n2 = 0; n2 < Size(2); ++n2)
			for (size_t n1 = 0; n1 < Size(1); ++n1)
				for (size_t n0 = 0; n0 < Size(0); ++n0) {
					const size_t m0 = (dim == 0) ? 0 : n0;
					const size_t m1 = (dim == 1) ? 0 : n1;
					const size_t m2 = (dim == 2) ? 0 : n2;
					const size_t m3 = (dim == 3) ? 0 : n3;
					const double v = std::fmax(ret(m0, m1, m2, m3),
							operator[](idx));
					ret.Insert(v, m0, m1, m2, m3);
					idx++;
				}
	return ret;
}

double Matrix::AllMin() const {
	double m = DBL_MAX;
	for (size_t n = 0; n < size(); n++) {
		const double v = operator[](n);
		if (v == DBL_MAX || v == -DBL_MAX)
			continue;
		m = fmin(m, v);
	}
	return m;
}

double Matrix::AllMax() const {
	double m = -DBL_MAX;
	for (size_t n = 0; n < size(); n++) {
		const double v = operator[](n);
		if (v == DBL_MAX || v == -DBL_MAX)
			continue;
		m = fmax(m, v);
	}
	return m;
}

double Matrix::AllMaxAbs() const {
	double v = -DBL_MAX;
	for (size_t n = 0; n < size(); n++)
		v = fmax(v, fabs(operator[](n)));
	return v;
}

void Matrix::AlignAtZero() {
	const double m = this->AllMin();
	for (size_t n = 0; n < size(); n++)
		operator[](n) -= m;
}

void Matrix::Normalize(double max) {
	double m = this->AllMaxAbs();
	if (fabs(m) <= DBL_EPSILON)
		return;
	m = max / m;
	for (size_t n = 0; n < size(); n++)
		operator[](n) *= m;
}

void Matrix::Normalize(double min, double max) {
	if (max <= min) {
		assign(size(), (min + max) / 2.0);
		return;
	}
	this->AlignAtZero();
	double m = this->AllMax();
	if (fabs(m) <= DBL_EPSILON)
		return;
	m = (max - min) / m;
	for (size_t n = 0; n < size(); ++n)
		operator[](n) = operator[](n) * m + min;
}

void Matrix::Mirror(Axis a) {
	const size_t Nx = Size(0);
	const size_t Ny = Size(1);
	const size_t Nz = Size(2);

	switch (a) {
	default:
		for (size_t i = 0; i < (Nx / 2); i++) {
			for (size_t j = 0; j < Ny; j++) {
				for (size_t k = 0; k < Nz; k++) {
					const size_t n = i + Nx * (j + Ny * k);
					const size_t m = (Nx - i - 1) + Nx * (j + Ny * k);
					std::swap(operator[](n), operator[](m));
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
					std::swap(operator[](n), operator[](m));
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
					std::swap(operator[](n), operator[](m));
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
		default:
			for (size_t i = 0; i < Nx; i++) {
				for (size_t n = i; n < (size() / 2); n += Nx) {
					std::swap(operator[](n),
							operator[]((size() - Nx) - n + 2 * i));
				}
			}
			break;
		case Axis::Y:
			for (size_t j = 0; j < Ny; j++) {
				size_t n = j * Nx;
				size_t m = size() - 1 - (Ny - 1) * Nx + j * Nx;
				for (size_t k = 0; k < (Nx * Nz / 2); k++) {
					std::swap(operator[](n), operator[](m));
					n = (n + Nx * Ny) % (size() - 1);
					m = (m + (size() - Nx * Ny - 1)) % (size() - 1);
				}
			}
			break;
		case Axis::Z:
			const size_t m = Nx * Ny;
			for (size_t k = 0; k < Nz; k++) {
				for (size_t n = 0; n < (m / 2); n++) {
					std::swap(operator[](k * Nx * Ny + n),
							operator[](k * Nx * Ny + (m - n - 1)));
				}
			}
			break;
		}
		return;
	}
	if (quarters != 1 && quarters != 3)
		ERROR("Rotation in the wrong quadrant.");
	std::vector<double> temp;
	temp.resize(size());

	switch (a) {
	default: {
		if (quarters == 1) {
			const size_t mo = size() + Nx;
			const size_t ad = size() - (Nx * (Ny - 1));
			for (size_t i = 0; i < Nx; i++) {
				size_t m = size() - Ny * Nx + i;
				for (size_t n = 0; n < (Ny * Nz); n++) {
					temp[n * Nx + i] = operator[](m);
					m = (m + ad) % mo;
				}
			}
		}
		if (quarters == 3) {
			const size_t mo = size() + Nx;
			const size_t ad = Nx * Ny;
			for (size_t i = 0; i < Nx; i++) {
				size_t m = (Ny - 1) * Nx + i;
				for (size_t n = 0; n < (Ny * Nz); n++) {
					temp[n * Nx + i] = operator[](m);
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
//			size() = Nx * Ny * Nz;
			size_t n = 0;
			size_t m = Nx - 1;
			for (size_t i = 0; i < Nx; i++) {
				for (size_t j = 0; j < Ny; j++) {
					for (size_t k = 0; k < Nz; k++) {
						temp[n] = operator[](m);
						n++;
						m += Nx * Ny;
					}
					m = m + Nx - size();
				}
				m = m - Nx * Ny - 1;
			}
		}
		if (quarters == 3) {
			size_t n = 0;
			size_t m = size();
			for (size_t i = 0; i < Nx; i++) {
				for (size_t j = 0; j < Ny; j++) {
					for (size_t k = 0; k < Nz; k++) {
						m -= Nx * Ny;
						temp[n] = operator[](m);
						n++;
					}
					m = m + size() + Nx;
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
				temp[n + k * Nx * Ny] = operator[](m + k * Nx * Ny);
				n++;
			}
		}
//		std::swap(Nx, Ny);
		Matrix::Reshape(Ny, Nx, Nz);
		break;
	}

	}
	this->swap(temp);
}

Matrix Matrix::operator -() const {
	Matrix ret = *this;
	for (double &v : ret)
		v = -v;
	return ret;
}

Matrix& Matrix::operator +=(const Matrix &b) {
	auto D0 = Size();
	auto D1 = b.Size();
	if (D0 != D1)
		ERROR("The dimensions of both matrices are not equal.");
	for (size_t i = 0; i < Numel(); ++i)
		operator[](i) += b[i];
	return *this;
}

Matrix& Matrix::operator -=(const Matrix &b) {
	auto D0 = Size();
	auto D1 = b.Size();
	if (D0 != D1)
		ERROR("The dimensions of both matrices are not equal.");
	for (size_t i = 0; i < Numel(); ++i)
		operator[](i) -= b[i];
	return *this;
}

Matrix& Matrix::operator *=(const Matrix &b) {
	const size_t N = Size(0);
	const size_t M = Size(1);
	if (N * M != Numel())
		ERROR("Only 2D-matrices can be multiplied.");
	if (M != b.Size(0))
		ERROR(
				"The right matrix [" << N << "x" << M << "] does not have as many columns as the left matrix [" << b.Size(0) << "x" << b.Size(1) << "] has rows.");
	const size_t P = b.Size(1);
	if (P * M != b.Numel())
		ERROR("Only 2D-matrices can be multiplied.");
	const Matrix A = *this;
	this->SetSize(N, P);
	this->assign(N * P, 0.0);
	for (size_t n = 0; n < N; ++n)
		for (size_t m = 0; m < M; ++m)
			for (size_t p = 0; p < P; ++p) {
				operator[](n + p * N) += A[n + m * N] * b[m + p * M];
			}
	return *this;
}

Matrix& Matrix::operator +=(const double b) {
	for (double &v : *this)
		v += b;
	return *this;
}

Matrix& Matrix::operator -=(const double b) {
	for (double &v : *this)
		v -= b;
	return *this;
}

Matrix& Matrix::operator *=(const double b) {
	for (double &v : *this)
		v *= b;
	return *this;
}

Matrix& Matrix::operator /=(const double b) {
	for (double &v : *this)
		v /= b;
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
	for (const auto &v : *this)
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

std::vector<size_t> Matrix::CalculateStrides(Order order_) const {
	std::vector<size_t> temp;
	const size_t N = counts.size();
	temp.assign(N, 1);
	if (N <= 1)
		return temp;
	switch (order_) {
	default:
		for (size_t n = 1; n < N; n++)
			temp[n] = temp[n - 1] * counts[n - 1];
		break;
	case Order::TWO_REVERSED:
		temp[0] = counts[1];
		if (N >= 3)
			temp[2] = temp[0] * counts[0];
		for (size_t n = 3; n < N; n++)
			temp[n] = temp[n - 1] * counts[n - 1];
		break;
	case Order::REVERSED:
		for (size_t n = N - 1; n > 0; n--)
			temp[n - 1] = temp[n] * counts[n];
		break;
	}
	return temp;
}

void Matrix::MapRows(Mode mode, const std::vector<size_t> &rows) {
	const size_t N = Size(0);
	std::vector<size_t> idx = FillIndex(mode, rows, N);
	std::vector<double> temp(*this);
	SetSize(idx.size(), Size(1));
	size_t offs = 0;
	for (size_t j = 0; j < counts[1]; ++j) {
		for (size_t i = 0; i < counts[0]; ++i) {
			if (idx[i] == (size_t) -1) {
				operator[](offs) = 0;
			} else {
				operator[](offs) = temp[j * N + idx[i]];
			}
			++offs;
		}
	}
}

void Matrix::MapCols(Mode mode, const std::vector<size_t> &cols) {
	const size_t M = Size(0);
	const size_t N = Size(1);
	std::vector<size_t> idx = FillIndex(mode, cols, N);
	std::vector<double> temp(*this);
	SetSize(Size(0), idx.size());
	size_t offsDst = 0;
	for (size_t i = 0; i < counts[1]; ++i) {

		if (idx[i] == (size_t) -1) {
			std::memset(data() + offsDst, 0, M * sizeof(double));
		} else {
			const size_t offsSrc = idx[i] * M;
			std::memcpy(data() + offsDst, temp.data() + offsSrc,
					M * sizeof(double));
		}
		offsDst += M;
	}
}

void Matrix::SetVariableName(const std::string &name) {
	this->variablename = name;
}

std::string Matrix::GetVariableName() const {
	return variablename;
}

