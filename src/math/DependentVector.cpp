///////////////////////////////////////////////////////////////////////////////
// Name               : DependentVector.cpp
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

#include "DependentVector.h"

#include <algorithm>
#include <cfloat>
#include <iterator>
#include <numeric>
#include <stdexcept>

#include "../3D/OpenGL.h"

DependentVector::DependentVector(size_t N, size_t NAxes) {
	SetSize(N, NAxes);
}

void DependentVector::Sync() {
	const size_t N = size();
	const size_t M = AxesCount();
	if (units.size() != M)
		units.resize(M);
	if (N % M != 0)
		throw std::runtime_error(
				"DependentVector::Sync - Row or column is not completely filled.");
	if (Size(0) != N / M)
		SetSize(N / M, M);
}

void DependentVector::Access(size_t idxChild) {
	// When an algorithm needs more than max. size_t / 2 axes, this is the
	// wrong class for that algorithm. It is more important to detect the
	// error in the program-logic, than to run out of memory.
	if (idxChild > ((size_t) -1) / 2)
		throw std::runtime_error(
				"DependentVector::Access - A negative index was requested for axis-access.");
	ReorderDimensions(Order::NORMAL);
	if (idxChild < AxesCount())
		return;
	auto dims = Size();
	if (dims.size() < 2)
		dims.resize(2, 1);
	dims[1] = idxChild + 1;
	SetSize(dims);
	Sync();
}

DependentVector DependentVector::Linspace(double v0, double v1, size_t N) {
	DependentVector ret;
	if (N == 0) {
		return ret;
	}
	ret.SetSize(N);
	const double d = (v1 - v0) / (double) (N - 1);
	for (size_t n = 0; n < N; n++) {
		ret[n] = v0;
		v0 += d;
	}
	return ret;
}

DependentVector DependentVector::Chebyshev(double v0, double v1, size_t N) {
	DependentVector ret;
	if (N == 0) {
		return ret;
	}
	ret.SetSize(N);
	const double c = (v1 + v0) / 2.0;
	const double r = (v1 - v0) / 2.0;
	for (size_t n = 0; n < N; n++)
		ret[n] = c - r * cos(M_PI * (double) n / (double) (N - 1));
	return ret;
}

void DependentVector::Clear() {
	Matrix::Reset();
	Sync();
}

void DependentVector::SetSize(size_t N, size_t NAxes) {
	if (!empty() && N != 0 && NAxes != 0) {
		throw std::runtime_error(
				"Not implemented (implement in Matrix::SetSize()");
	}
	Matrix::SetSize(N, NAxes);
	Sync();
}

void DependentVector::SetSize(const std::vector<size_t> &dims) {
	Matrix::SetSize(dims);
	Sync();
}

void DependentVector::SetSize(const DependentVector &other) {
	Matrix::SetSize(other.Size());
	Sync();
}

size_t DependentVector::Length() const {
	return Size(0);
}

size_t DependentVector::AxesCount() const {
	if (empty() || Size(0) == 0)
		return 1; // There is always at least one empty axis.
	return Numel() / Size(0);
}

void DependentVector::SetCyclic(double cycleLength) {
	cyclic = true;
	this->cycleLength = cycleLength;
}

void DependentVector::SetLinear() {
	cyclic = false;
	cycleLength = 0.0;
}

bool DependentVector::IsCyclic() const {
	return cyclic;
}

double DependentVector::CycleLength() const {
	return cycleLength;
}

DependentVector DependentVector::Range(size_t xstart, size_t xend) const {
	if (xend >= Size(0))
		xend = Size(0) - 1;
	if (xstart > xend)
		throw(std::runtime_error(
		__FILE__"DependentVector::Range: idxStart > idxEnd"));

	DependentVector temp;
	const size_t N = xend + 1 - xstart;
	const size_t M = AxesCount();
	temp.SetSize(N, M);

	if (order == Order::NORMAL) {
		for (size_t n = 0; n < M; n++)
			std::copy_n(begin() + n * Size(0) + xstart, N,
					temp.begin() + N * n);
	} else {
		std::copy_n(begin() + xstart * AxesCount(), N * M, temp.begin());
	}
	return temp;
}

DependentVector& DependentVector::operator +=(const DependentVector &a) {
	if (a.size() != size())
		throw(std::range_error(
				"DependentVector::operator += Both arrays have different sizes."));
	std::transform(begin(), end(), a.begin(), begin(), std::plus<double>());
	return *this;
}

DependentVector& DependentVector::operator -=(const DependentVector &a) {
	if (a.size() != size())
		throw(std::range_error(
				"DependentVector::operator -= Both arrays have different sizes."));
	std::transform(begin(), end(), a.begin(), begin(), std::minus<double>());
	return *this;
}

DependentVector& DependentVector::operator *=(const DependentVector &a) {
	if (a.size() != size())
		throw(std::range_error(
				"DependentVector::operator *= Both arrays have different sizes."));
	std::transform(begin(), end(), a.begin(), begin(),
			std::multiplies<double>());
	return *this;
}

DependentVector& DependentVector::operator /=(const DependentVector &a) {
	if (a.size() != size())
		throw(std::range_error(
				"DependentVector::operator /= Both arrays have different sizes."));
	std::transform(begin(), end(), a.begin(), begin(), std::divides<double>());
	return *this;
}

void DependentVector::PushBack(double x, double y) {
	const size_t Nax = AxesCount();
	if (Nax < 2)
		Access(1);
	ReorderDimensions(Order::TWO_REVERSED);
	push_back(x);
	push_back(y);
	// Fill in the rest of the axes to keep the insertion point in sync.
	for (size_t n = 2; n < Nax; n++)
		push_back(0.0);
	Sync();
}

DependentVector::AxisView::AxisView(DependentVector *const parent,
		size_t axBegin, size_t axEnd, size_t rowBegin, size_t rowEnd) :
		parent(parent), axBegin(axBegin), axEnd(axEnd), rowBegin(rowBegin), rowEnd(
				rowEnd) {
}

size_t DependentVector::AxisView::GetRowStart() const {
	return rowBegin;
}

size_t DependentVector::AxisView::GetRowEnd() const {
	const size_t N = parent->Length();
	if (rowEnd > N)
		return N;
	return rowEnd;
}

double& DependentVector::AxisView::operator[](size_t row) {
	const size_t pos = axBegin * parent->Length() + row;
	return parent->operator[](pos);
}

double DependentVector::AxisView::operator[](size_t row) const {
	const size_t pos = axBegin * parent->Length() + row;
	return parent->operator[](pos);
}

void DependentVector::AxisView::Init(double value) {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	const size_t Nd = N1 - N0;
	if (N0 == 0 && N1 == N) {
		std::fill_n(parent->begin() + axBegin * N, (axEnd - axBegin) * N,
				value);
	} else {
		for (size_t ax = axBegin; ax < axEnd; ax++) {
			const size_t offs = ax * N + N0;
			std::fill_n(parent->begin() + offs, Nd, value);
		}
	}
}

void DependentVector::AxisView::Linspace(double v0, double v1) {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	const size_t Nd = N1 - N0;
	size_t obo = (parent->cyclic && N1 == N) ? 0 : 1;
	for (size_t ax = axBegin; ax < axEnd; ax++) {
		const size_t offs = ax * N + N0;
		for (size_t n = 0; n < Nd; n++)
			parent->operator[](offs + n) = v0
					+ (double) n / (double) (Nd - obo) * (v1 - v0);
	}
}

void DependentVector::AxisView::Chebyshev(double v0, double v1) {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	const size_t Nd = N1 - N0;
	const double c = (v1 + v0) / 2.0;
	const double r = (v1 - v0) / 2.0;
	size_t obo = (parent->cyclic && N1 == N) ? 0 : 1;
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N + N0;
		for (size_t n = 0; n < Nd; n++)
			parent->operator[](offs + n) = c
					- r * cos(M_PI * (double) n / (double) (Nd - obo));
	}
}

void DependentVector::AxisView::Limit(double vMin, double vMax) {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		for (size_t n = N0; n < N1; n++)
			parent->operator[](offs + n) = fmin(
					fmax(parent->operator[](offs + n), vMin), vMax);
	}
}

void DependentVector::AxisView::Wrap(double v0, double v1) {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	const double d = v1 - v0;
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		double shift = 0.0;
		for (size_t n = N0; n < N1; n++) {
			double v = parent->operator[](offs + n) + shift;
			while (v > v1) {
				shift -= d;
				v -= d;
			}
			while (v < v0) {
				shift += d;
				v += d;
			}
			parent->operator[](offs + n) = v;
		}
	}
}

void DependentVector::AxisView::Unwrap(double tolerance) {
	if (tolerance <= 0.0)
		throw(std::domain_error(
				__FILE__"DependentVector::AxisView::Unwrap: 'tolerance' should be a positive number."));
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	const double d = 2.0 * tolerance;
	const size_t Nstart = (N0 > 0) ? N0 : N0 + 1;
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		double shift = 0.0;
		for (size_t n = Nstart; n < N1; n++) {
			double v0 = parent->operator[](offs + n - 1);
			double v = parent->operator[](offs + n) + shift;
			while (v > v0 + tolerance) {
				shift -= d;
				v -= d;
			}
			while (v < v0 - tolerance) {
				shift += d;
				v += d;
			}
			parent->operator[](offs + n) = v;
		}
	}
}

void DependentVector::AxisView::Normalize() {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const AxisView row(parent, ax, ax + 1, N0, N1);
		const double ymax = row.Max();
		const double ymin = row.Min();
		const double h = 1.0 / fmax(ymax - ymin, 1e-9);

		const size_t offs = ax * N;
		for (size_t n = N0; n < N1; n++) {
			parent->operator[](offs + n) = (parent->operator[](offs + n) - ymin)
					* h;
		}
	}
}

void DependentVector::AxisView::CumSum() {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	const size_t Nstart = (N0 > 0) ? N0 : N0 + 1;
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		for (size_t n = Nstart; n < N1; n++) {
			parent->operator[](offs + n) += parent->operator[](offs + n - 1);
		}
	}
}

void DependentVector::AxisView::CumProd() {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	const size_t Nstart = (N0 > 0) ? N0 : N0 + 1;
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		for (size_t n = Nstart; n < N1; n++) {
			parent->operator[](offs + n) *= parent->operator[](offs + n - 1);
		}
	}
}

void DependentVector::AxisView::Fill(std::function<double(double)> func) {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		if (ax == 0)
			continue;
		const size_t offs = ax * N;
		for (size_t n = N0; n < N1; n++) {
			parent->operator[](offs + n) = func(parent->operator[](n));
		}
	}
	if (axBegin == 0) {
		for (size_t n = N0; n < N1; n++)
			parent->operator[](n) = func(parent->operator[](n));
	}
}

void DependentVector::AxisView::Integrate() {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	const size_t Nstart = (N0 > 0) ? N0 : N0 + 1;
	std::vector<double> temp(N, 0.0);
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		if (N0 != 0)
			temp[N0] = parent->operator[](offs + N0);
		for (size_t n = Nstart; n < N1; n++) {
			const double x0 = parent->operator[](n - 1);
			const double x1 = parent->operator[](n);
			const double y0 = parent->operator[](offs + n - 1);
			const double y1 = parent->operator[](offs + n);
			temp[n] = temp[n - 1] + (y1 + y0) * (x1 - x0) / 2.0;
		}
		for (size_t n = Nstart; n < N1; n++)
			parent->operator[](n) = temp[n];
	}
}

void DependentVector::AxisView::Derive() {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	const size_t Nstart = (N0 > 0) ? N0 : N0 + 1;
	std::vector<double> temp(N, 0.0);
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		for (size_t n = Nstart; n < N1; n++) {
			const double x0 = parent->operator[](n - 1);
			const double x1 = parent->operator[](n);
			const double y0 = parent->operator[](offs + n - 1);
			const double y1 = parent->operator[](offs + n);
			temp[n] = (y1 - y0) / (x1 - x0);
		}
		for (size_t n = Nstart; n < N1; n++)
			parent->operator[](n) = temp[n];
	}
}

double DependentVector::AxisView::Mean() const {
	const double mean = Area();
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	if (parent->cyclic && N0 == 0 && N1 == N) {
		return mean / parent->cycleLength;
	} else {
		const double x0 = parent->operator[](N0);
		const double x1 = parent->operator[](N1 - 1);
		return mean / (x1 - x0);
	}
}

double DependentVector::AxisView::Area() const {
	double area = 0.0;
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		for (size_t n = N0 + 1; n < N1; n++) {
			const double x0 = parent->operator[](n - 1);
			const double x1 = parent->operator[](n);
			const double y0 = parent->operator[](offs + n - 1);
			const double y1 = parent->operator[](offs + n);
			area += (y1 + y0) * (x1 - x0) / 2.0;
		}
		if (parent->cyclic && N0 == 0 && N1 == N) {
			const double x0 = parent->operator[](N1 - 1);
			const double x1 = parent->operator[](N0);
			const double y0 = parent->operator[](offs + N1 - 1);
			const double y1 = parent->operator[](offs + N0);
			area += (y1 + y0) * (x1 - x0) / 2.0;
		}
	}
	return area;
}

double DependentVector::AxisView::Max() const {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	double ret = -DBL_MAX;
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		for (size_t n = N0; n < N1; n++) {
			const double v = parent->operator[](offs + n);
			if (v > ret)
				ret = v;
		}
	}
	return ret;
}

double DependentVector::AxisView::Min() const {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	double ret = DBL_MAX;
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		for (size_t n = N0; n < N1; n++) {
			const double v = parent->operator[](offs + n);
			if (v < ret)
				ret = v;
		}
	}
	return ret;
}

DependentVector::AxisView::Parabola::Parabola(double x0, double x1, double x2) {
	// In Maxima:
	// f(x):=c0+c1*x+c2*x^2;
	// df(x):=diff(f(x), x);
	// ddf(x):=diff(df(x), x);
	// S:solve([f(x0)=v0,f(x1)=v1,f(x2)=v2],[c0,c1,c2]);
	// solve(ev(df(x),S[1])=0, x);

	div = (x0 - x2) * (x2 - x1) * (x1 - x0);
	OK = fabs(div) > DBL_EPSILON;
	if (!OK)
		return;

	const double f = 1.0 / div;
	const double x02 = x0 * x0;
	const double x12 = x1 * x1;
	const double x22 = x2 * x2;
	ec0v0 = (x12 * x2 - x1 * x22) * f;
	ec0v1 = (x22 * x0 - x2 * x02) * f;
	ec0v2 = (x02 * x1 - x0 * x12) * f;
	ec1v0 = (x22 - x12) * f;
	ec1v1 = (x02 - x22) * f;
	ec1v2 = (x12 - x02) * f;
	ec2v0 = (x1 - x2) * f;
	ec2v1 = (x2 - x0) * f;
	ec2v2 = (x0 - x1) * f;
}

bool DependentVector::AxisView::Parabola::IsOK() const {
	return OK;
}

void DependentVector::AxisView::Parabola::Calc(double v0, double v1,
		double v2) {
	if (!OK)
		return;
	c0 = v0 * ec0v0 + v1 * ec0v1 + v2 * ec0v2;
	c1 = v0 * ec1v0 + v1 * ec1v1 + v2 * ec1v2;
	c2 = v0 * ec2v0 + v1 * ec2v1 + v2 * ec2v2;
}

bool DependentVector::AxisView::Parabola::IsMax() const {
	if (!OK)
		return false;
	return c2 < (-DBL_EPSILON);
}

bool DependentVector::AxisView::Parabola::IsMin() const {
	if (!OK)
		return false;
	return c2 > DBL_EPSILON;
}

double DependentVector::AxisView::Parabola::XExtremum() const {
	if (!OK || fabs(c2) < DBL_EPSILON)
		return 0.0;
	return -c1 / (2.0 * c2);
}

double DependentVector::AxisView::Parabola::Y(double x) const {
	return c0 + (c1 + c2 * x) * x;
}

DependentVector::Point DependentVector::AxisView::ExtremumP(bool useParabolas,
		bool invert, bool immediatlyReturn) const {
	const size_t N = parent->Length();
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	DependentVector::Point ret(parent->AxesCount() - 1);
	double vm = invert ? (DBL_MAX) : (-DBL_MAX);
	for (size_t ax = axBegin; ax <= axEnd; ax++) {
		const size_t offs = ax * N;
		for (size_t idx0 = N0; idx0 < N1; idx0++) {
			const double v0 = parent->operator[](offs + idx0);
			if (((v0 > vm && !invert) || (v0 < vm && invert))
					&& !immediatlyReturn) {
				ret.idx = idx0;
				vm = v0;
			}
			if (useParabolas
					&& ((idx0 + 2 < N1) || (N == N1 && parent->cyclic))) {
				const size_t idx1 = (idx0 + 1) % N;
				const size_t idx2 = (idx0 + 2) % N;
				const double x0 = parent->operator[](idx1);
				const double x1 = parent->operator[](idx1);
				const double x2 = parent->operator[](idx2);
				Parabola p(x0, x1, x2);
				if (p.IsOK()) {
					const double v1 = parent->operator[](offs + idx1);
					const double v2 = parent->operator[](offs + idx2);
					p.Calc(v0, v1, v2);
					if ((p.IsMax() && !invert) || (p.IsMin() && invert)) {
						const double x = p.XExtremum();
						if (x >= x0 && x <= x2) {
							const double y = p.Y(x);
							if ((y > vm && !invert) || (y < vm && invert)) {
								vm = y;
								if (x >= x1) {
									ret.idx = idx1;
									ret.rel = (x - x1) / (x2 - x1);
								} else {
									ret.idx = idx0;
									ret.rel = (x - x0) / (x1 - x0);
								}
								ret.x = x;
								ret.y[ax - 1] = y;
								for (size_t ax2 = 0; ax2 < ret.y.size();
										ax2++) {
									if (ax2 + 1 == ax)
										continue;
									const size_t offs2 = (ax2 + 1) * N;
									const double w0 = parent->operator[](
											offs2 + idx1);
									const double w1 = parent->operator[](
											offs2 + idx1);
									const double w2 = parent->operator[](
											offs2 + idx2);
									p.Calc(w0, w1, w2);
									ret.y[ax2] = p.Y(x);
								}
								if (immediatlyReturn)
									return ret;
							}
						}
					}
				}
			}
		}
	}
	return ret;
}

DependentVector::Point DependentVector::AxisView::MaxP(
		bool useParabolas) const {
	if (axBegin == 0 && useParabolas)
		throw std::runtime_error(
				"DependentVector::AxisView::MaxP - This function cannot be applied to the X axis whilst evaluating parabolas.");
	return ExtremumP(useParabolas, false, false);
}

DependentVector::Point DependentVector::AxisView::MinP(
		bool useParabolas) const {
	if (axBegin == 0 && useParabolas)
		throw std::runtime_error(
				"DependentVector::AxisView::MaxP - This function cannot be applied to the X axis whilst evaluating parabolas.");
	return ExtremumP(useParabolas, true, false);
}

std::vector<DependentVector::Point> DependentVector::AxisView::FindPeaks(
		const double vMin) {
	std::vector<Point> result;
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	size_t Nc = N0;
	while (Nc < N1) {
		AxisView view(parent, axBegin, axEnd, Nc, N1);
		DependentVector::Point pt = view.ExtremumP(true, false, true);
		if (pt.idx == (size_t) -1)
			break;
		if (axBegin > 0 && pt.y[axBegin - 1] > vMin)
			result.push_back(pt);
		Nc = pt.idx + 1;
	}
	auto ptlt = [&ax=axBegin](const DependentVector::Point &lhs,
			const DependentVector::Point &rhs) {
		return lhs.y[ax - 1] < rhs.y[ax - 1];
	};
	std::sort(result.begin(), result.end(), ptlt);
	return result;
}

std::vector<DependentVector::Point> DependentVector::AxisView::FindValleys(
		const double vMax) {
	std::vector<Point> result;
	const size_t N0 = GetRowStart();
	const size_t N1 = GetRowEnd();
	size_t Nc = N0;
	while (Nc < N1) {
		AxisView view(parent, axBegin, axEnd, Nc, N1);
		DependentVector::Point pt = view.ExtremumP(true, true, true);
		if (pt.idx == (size_t) -1)
			break;
		if (axBegin > 0 && pt.y[axBegin - 1] < vMax)
			result.push_back(pt);
		Nc = pt.idx + 1;
	}
	auto ptlt = [&ax=axBegin](const DependentVector::Point &lhs,
			const DependentVector::Point &rhs) {
		return lhs.y[ax - 1] < rhs.y[ax - 1];
	};
	std::sort(result.begin(), result.end(), ptlt);
	return result;
}

DependentVector::AxisView DependentVector::All() {
	Access(AxesCount() - 1);
	return AxisView(this, 0, AxesCount());
}

DependentVector::AxisView DependentVector::AllY() {
	Access(AxesCount() - 1);
	return AxisView(this, 1, AxesCount());
}

DependentVector::AxisView DependentVector::Ax(size_t axis) {
	Access(axis);
	return AxisView(this, axis, axis + 1);
}

DependentVector::AxisView DependentVector::X() {
	Access(0);
	return AxisView(this, 0, 1);
}

DependentVector::AxisView DependentVector::Y() {
	Access(1);
	return AxisView(this, 1, 2);
}

//DependentVector::AxisView DependentVector::Y(size_t axis) {
//	Access(axis + 1);
//	return AxisView(this, axis + 1, axis + 2);
//}

void DependentVector::Sort(size_t axisIdx) {
	Sync();
	ReorderDimensions();
	const size_t N = Length();
	if (N <= 1)
		return;

	// Deferred sorting: First idx is sorted, then all vectors are remapped
	// with idx.
	std::vector<size_t> idx(N);
	std::iota(idx.begin(), idx.end(), 0);

	const size_t offs = axisIdx * N;
	const double v0 = operator [](offs);
	const double v1 = operator [](offs + N - 1);

	// Shortcut, maybe the vectors are simply ordered in reverse.
	if (v0 > v1)
		std::reverse(idx.begin(), idx.end());

	auto less_than = [&v = *this, &offs](size_t idx0, size_t idx1) {
		return v[offs + idx0] < v[offs + idx1];
	};
	std::sort(idx.begin(), idx.end(), less_than);

	Map(idx);
}

void DependentVector::Reverse() {
	Sync();
	ReorderDimensions();
	const size_t N = Length();
	for (size_t ax = 0; ax < AxesCount(); ax++)
		std::reverse(begin() + ax * N, begin() + (ax + 1) * N);
}

void DependentVector::Resample(size_t Nnew) {
	//TODO Test all edge-cases: linear, cyclic, cyclic with overlapping ends
	Sync();
	ReorderDimensions();
	if (empty())
		return;
	std::vector<Point> pos;
	if (Nnew == 0) {
		SetSize(Nnew, AxesCount());
		return;
	}
	if (Nnew == 1 && !cyclic) {
		std::vector<size_t> idx = { 0 };
		Map(idx);
		return;
	}
	const double v0 = front();
	const double v1 = cyclic ? (v0 + cycleLength) : (operator[](Size(0) - 1));
	std::vector<double> x;
	x.reserve(Nnew);
	const double h = (double) Nnew - (cyclic ? 2 : 1);
	for (size_t n = 0; n < Nnew; n++) {
		x.push_back((double) n / h * (v1 - v0) + v0);
	}
	*this = Sample(x);
}

DependentVector DependentVector::Sample(const std::vector<double> &x) const {
	std::vector<Point> pos;
	pos.reserve(x.size());
	size_t idx = 0;
	const size_t N = Length();
	const double x0 = front();
	for (double s : x) {
		if (cyclic) {
			s = fmod(s - x0, cycleLength) + x0;
			if ((s - x0) < 0.0)
				s += cycleLength;
		}
		while (idx + 1 < N && operator[](idx) < s)
			idx++;
		while (idx > 0 && operator[](idx - 1) > s)
			idx--;
		size_t idx1 = idx + 1;
		if (cyclic) {
			idx1 %= N;
		} else if (idx1 >= N) {
			idx--;
			idx1--;
		}
		const double x0 = operator[](idx);
		const double x1 = operator[](idx1) + ((idx1 < idx) ? cycleLength : 0.0);
		pos.emplace_back(idx, (s - x0) / (x1 - x0), s);
	}
	return Sample(pos);
}

DependentVector DependentVector::Sample(const std::vector<Point> &pos) const {
	const size_t N = Length();
	const size_t Ax = AxesCount();
	DependentVector ret(pos.size(), Ax);
	ret.units = units;
	ret.cyclic = cyclic;
	ret.cycleLength = cycleLength;
	ret.clear();
	ret.reserve(pos.size() * Ax);
	for (size_t ax = 0; ax < Ax; ax++) {
		size_t offs = ax * N;

		for (Point p : pos) {
			size_t idx0 = p.idx;
			size_t idx1 = (p.idx + 1);
			if (cyclic) {
				idx1 %= N;
			} else if (idx1 >= N) {
				idx0--;
				idx1--;
			}
			const double v0 = operator[](offs + idx0);
			const double v1 = operator[](offs + idx1);
			ret.push_back(v0 + (v1 - v0) * p.rel);
		}
	}
	return ret;
}

void DependentVector::Map(const std::vector<size_t> &indices) {
	Sync();
	ReorderDimensions();
	const size_t N = Length();
	DependentVector temp = *this;
	clear();
	for (size_t ax = 0; ax < AxesCount(); ax++) {
		size_t offs = ax * N;
		for (size_t i : indices)
			push_back(temp[offs + i]);
	}
	SetSize(indices.size(), AxesCount());
}

size_t DependentVector::IatV(double value, size_t axis, Direction direction,
		size_t rowStart, size_t rowEnd) const {
	const size_t N = Length();
	if (N == 0)
		return (size_t) -1;
	if (N == 1)
		return 0;

	if (axis == 0 && direction == Direction::first_risingabove) {
		// Special handling for the x-axis
		if (cyclic && cycleLength > 0.0) {
			while (value < front())
				value += cycleLength;
			value = fmod(value - front(), cycleLength) + front();
		}
		if (idxSearchSpeedup >= size())
			idxSearchSpeedup = 0;
		if (operator[](idxSearchSpeedup) > value)
			idxSearchSpeedup = 0;
		while ((idxSearchSpeedup + 1) < size()
				&& operator[](idxSearchSpeedup + 1) < value)
			idxSearchSpeedup++;
		return idxSearchSpeedup;
	}

	const size_t offs = axis * N;
	rowStart++;
	if (rowEnd >= N)
		rowEnd = N;
	else
		rowEnd++;
	if (rowStart > rowEnd)
		return (size_t) -1;
	switch (direction) {
	case Direction::first_risingabove: {
		for (size_t n = rowStart; n < rowEnd; n++)
			if (operator[](offs + n - 1) <= value
					&& operator[](offs + n) > value)
				return n;
	}
		break;
	case Direction::first_fallingbelow: {
		for (size_t n = rowStart; n < rowEnd; n++)
			if (operator[](offs + n - 1) >= value
					&& operator[](offs + n) < value)
				return n;
	}
		break;
	case Direction::first_passing: {
		for (size_t n = rowStart; n < rowEnd; n++)
			if ((operator[](offs + n - 1) >= value
					&& operator[](offs + n) <= value)
					|| (operator[](offs + n - 1) <= value
							&& operator[](offs + n) >= value))
				return n;
	}
		break;
	case Direction::last_risingabove: {
		for (size_t n = rowEnd; n-- > rowStart;)
			if (operator[](offs + n - 1) <= value
					&& operator[](offs + n) > value)
				return n;
	}
		break;
	case Direction::last_fallingbelow: {
		for (size_t n = rowEnd; n-- > rowStart;)
			if (operator[](offs + n - 1) >= value
					&& operator[](offs + n) < value)
				return n;
	}
		break;
	case Direction::last_passing: {
		for (size_t n = rowEnd; n-- > rowStart;)
			if ((operator[](offs + n - 1) >= value
					&& operator[](offs + n) <= value)
					|| (operator[](offs + n - 1) <= value
							&& operator[](offs + n) >= value))
				return n;
	}
		break;
	}
	return (size_t) -1;
}

DependentVector::Point DependentVector::PatV(double value, size_t axis,
		Direction direction, size_t rowStart, size_t rowEnd) const {
	const size_t N = Length();
	const size_t Ax = AxesCount();
	if (axis > Ax)
		throw std::runtime_error("DependentVector::PatV - axis out of range.");
	size_t idx0 = IatV(value, axis, direction, rowStart, rowEnd);
	size_t idx1 = idx0 - 1;
	if (cyclic) {
		idx1 %= N;
	} else if (idx1 >= N) {
		idx0--;
		idx1--;
	}
	const double x0 = operator[](idx0);
	const double x1 =
			operator[](idx1) + (cyclic && idx1 < idx0) ? cycleLength : 0.0;
	const size_t offs = axis * N;
	const double y0 = operator[](offs + idx0);
	const double y1 = operator[](offs + idx1);

	Point ret(Ax - 1);
	ret.idx = idx0;
	if (axis == 0) {
		ret.rel = (value - x0) / (x1 - x0);
	} else {
		if (fabs(y1 - y0) > DBL_EPSILON)
			ret.rel = (value - y0) / (y1 - y0);
		else
			ret.rel = 0.0;
	}
	ret.x = x0 + (x1 - x0) * ret.rel;
	if (axis > 0)
		ret.y[axis - 1] = value;
	for (size_t ax = 1; ax < Ax; ax++) {
		if (ax == axis)
			continue;
		const double v0 = operator[](ax * N + idx0);
		const double v1 = operator[](ax * N + idx1);
		ret.y[ax - 1] = v0 + (v1 - v0) * ret.rel;
	}
	return ret;
}

double DependentVector::VatV(const double value, size_t axisFrom, size_t axisTo,
		Direction direction, size_t idxStart, size_t idxEnd) const {
	Point p = PatV(value, axisFrom, direction, idxStart, idxEnd);
//	if (size() == 0)
//		return DBL_MAX;
//	if (size() == 1)
//		return y[0];
//	size_t idx = IatX(xval);
//	if (idx == (size_t) -1)
//		return 0.0;
//	size_t idx = IatY(yval, direction, xstart, xend);
//	if (idx > size()) {
//		if (direction == Direction::first_risingabove
//				|| direction == Direction::first_fallingbelow
//				|| direction == Direction::first_passing)
//			return DBL_MAX;
//		return -DBL_MAX;

	if (axisTo == 0)
		return p.x;
	return p.y[axisTo - 1];
}

void DependentVector::Paint() const {
	const size_t N = Length();
	const size_t Ax = AxesCount();
	for (size_t ax = 1; ax < Ax; ax++) {
		const size_t offs = ax * N;
		glBegin(GL_LINE_STRIP);
		if (cyclic) {
			for (size_t n = 0; n < N; n++) {
				double co = cos(operator[](n));
				double si = sin(operator[](n));
				double d = operator[](offs + n);
				glVertex2d(co * d, si * d);
			}
		} else {
			for (size_t n = 0; n < N; n++)
				glVertex2d(operator[](n), operator[](offs + n));
		}
		glEnd();
	}
}

void DependentVector::PaintCircle(double radius) {
	glBegin(GL_LINE_LOOP);
	for (size_t n = 0; n < 100; n++) {
		const float co = cos(2 * M_PI / 100 * n);
		const float si = sin(2 * M_PI / 100 * n);
		glNormal3f(co, si, 0);
		glVertex2f(co * radius, si * radius);
	}
	glEnd();
}
