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
#include <iterator>
#include <stdexcept>

#include "../3D/OpenGL.h"

void DependentVector::Clear() {
	x.clear();
	y.clear();
}

void DependentVector::Resize(size_t N) {
	x.resize(N, 0.0);
	y.resize(N, 0.0);
}

void DependentVector::PushBack(double x, double y) {
	this->x.push_back(x);
	this->y.push_back(y);
}

size_t DependentVector::Size() const {
	return x.size();
}

void DependentVector::XLinspace(double x0, double x1, size_t N) {
	if (N == 0 || N == (size_t) -1) {
		N = x.size();
	} else {
		x.assign(N, 0.0);
	}
	y.resize(N, 0.0);
	for (size_t n = 0; n < N; ++n)
		x[n] = x0 + (double) n / (double) (N - 1) * (x1 - x0);
}

void DependentVector::XSetCyclic(double cyclelength) {
	cyclic = true;
	this->cyclelength = cyclelength;
}

void DependentVector::XSetLinear() {
	cyclic = false;
	cyclelength = 0.0;
}

bool DependentVector::IsCyclic() const {
	return cyclic;
}

double DependentVector::CycleLength() const {
	return cyclelength;
}

void DependentVector::YInit(double value) {
	y.assign(x.size(), value);
}

void DependentVector::YLinspace(double y0, double y1) {
	const size_t N = x.size();
	y.resize(N);
	for (size_t n = 0; n < N; ++n)
		y[n] = y0 + (double) n / (double) (N - 1) * (y1 - y0);
}

void DependentVector::YFill(std::function<double(double)> func) {
	const size_t N = x.size();
	y.resize(N);
	for (size_t n = 0; n < N; ++n)
		y[n] = func(x[n]);
}

double& DependentVector::X(size_t index) {
	return x[index];
}

const double& DependentVector::X(size_t index) const {
	return x[index];
}

double& DependentVector::Y(size_t index) {
	return y[index];
}

const double& DependentVector::Y(size_t index) const {
	return y[index];
}

double& DependentVector::operator [](size_t index) {
	return y[index];
}

double DependentVector::operator [](size_t index) const {
	return y[index];
}

size_t DependentVector::IatX(const double xval) const {
	if (x.size() == 0)
		return (size_t) -1;
	if (x.size() == 1)
		return 0;
	if (cyclic && (xval < x[0] || xval > x[x.size() - 1]))
		throw(std::logic_error(
		__FILE__" - IatX : Cyclic lookup not implemented!"));
	if (x[0] > xval)
		return 0;
	if (searchspeedup >= x.size())
		searchspeedup = 0;
	if (x[searchspeedup] > xval)
		searchspeedup = 0;
	while ((searchspeedup + 1) < x.size() && x[searchspeedup + 1] <= xval)
		searchspeedup++;
	return searchspeedup;
}

double DependentVector::YatX(const double xval) const {
	if (x.size() == 0)
		return DBL_MAX;
	if (x.size() == 1)
		return y[0];
	if (cyclic && (xval < x[0] || xval > x[x.size() - 1]))
		throw(std::logic_error(
		__FILE__" - YatX : Cyclic lookup not implemented!"));

	size_t idx = IatX(xval);
	if (idx == (size_t) -1)
		return 0.0;
	if (idx + 1 == x.size())
		--idx;

	const double den = x[idx + 1] - x[idx];
	if (fabs(den) < 1e-9)
		return (y[idx + 1] + y[idx]) / 2.0;
	return ((xval - x[idx]) * y[idx + 1] - (xval - x[idx + 1]) * y[idx]) / den;
}

size_t DependentVector::IatY(const double yval, Direction direction,
		size_t xstart, size_t xend) const {
	const size_t N = x.size();
	++xstart;
	if (xend >= N)
		xend = N;
	else
		++xend;
	if (xstart > xend)
		return (size_t) -1;
	switch (direction) {
	case Direction::first_risingabove: {
		for (size_t n = xstart; n < xend; ++n)
			if (y[n - 1] <= yval && y[n] > yval)
				return n;
	}
		break;
	case Direction::first_fallingbelow: {
		for (size_t n = xstart; n < xend; ++n)
			if (y[n - 1] >= yval && y[n] < yval)
				return n;
	}
		break;
	case Direction::first_passing: {
		for (size_t n = xstart; n < xend; ++n)
			if ((y[n - 1] >= yval && y[n] <= yval)
					|| (y[n - 1] <= yval && y[n] >= yval))
				return n;
	}
		break;
	case Direction::last_risingabove: {
		for (size_t n = xend; n-- > xstart;)
			if (y[n - 1] <= yval && y[n] > yval)
				return n;
	}
		break;
	case Direction::last_fallingbelow: {
		for (size_t n = xend; n-- > xstart;)
			if (y[n - 1] >= yval && y[n] < yval)
				return n;
	}
		break;
	case Direction::last_passing: {
		for (size_t n = xend; n-- > xstart;)
			if ((y[n - 1] >= yval && y[n] <= yval)
					|| (y[n - 1] <= yval && y[n] >= yval))
				return n;
	}
		break;
	}
	return (size_t) -1;
}

double DependentVector::XatY(const double yval, Direction direction,
		size_t xstart, size_t xend) const {
	size_t idx = IatY(yval, direction, xstart, xend);
	if (idx > x.size()) {
		if (direction == Direction::first_risingabove
				|| direction == Direction::first_fallingbelow
				|| direction == Direction::first_passing)
			return DBL_MAX;
		return -DBL_MAX;
	}
	const double den = y[idx - 1] - y[idx];
	if (fabs(den) < 1e-9)
		return (x[idx - 1] + x[idx]) / 2.0;
	return ((x[idx - 1] - x[idx]) * yval - x[idx - 1] * y[idx]
			+ x[idx] * y[idx - 1]) / den;
}

DependentVector DependentVector::operator -() const {
	DependentVector temp = *this;
	for (auto &v : temp.y)
		v = -v;
	return temp;
}

DependentVector& DependentVector::operator +=(const double val) {
	for (auto &v : y)
		v += val;
	return (*this);
}

DependentVector& DependentVector::operator -=(const DependentVector &a) {
	if (a.Size() != this->Size())
		throw(std::range_error(
				"DependentVector::operator -= Both arrays have different sizes."));
	std::transform(this->y.begin(), this->y.end(), a.y.begin(), this->y.begin(),
			std::minus<double>());
	return *this;
}

DependentVector& DependentVector::operator -=(const double val) {
	for (auto &v : y)
		v -= val;
	return (*this);
}

DependentVector& DependentVector::operator *=(const double val) {
	for (auto &v : y)
		v *= val;
	return (*this);
}

DependentVector& DependentVector::operator /=(const double val) {
	for (auto &v : y)
		v /= val;
	return (*this);
}

void DependentVector::YLimit(double ymin, double ymax) {
	for (auto &v : y)
		v = fmin(fmax(v, ymin), ymax);
}

void DependentVector::Normalize(size_t xstart, size_t xend) {
	const auto ymax = Max(xstart, xend);
	const auto ymin = Min(xstart, xend);
	const double h = 1.0 / fmax(ymax.y - ymin.y, 1e-9);
	for (auto &v : y)
		v = (v - ymin.y) * h;
}

DependentVector::Point DependentVector::Max(size_t xstart, size_t xend) const {
	if (xend >= x.size())
		xend = x.size() - 1;
	Point temp;
	temp.idx = (size_t) -1;
	temp.y = -DBL_MAX;
	if (xstart > xend)
		return temp;
	for (size_t n = xstart; n <= xend; ++n) {
		if (y[n] > temp.y) {
			temp.idx = n;
			temp.x = x[n];
			temp.y = y[n];
		}
	}
	return temp;
}

DependentVector::Point DependentVector::Min(size_t xstart, size_t xend) const {
	if (xend >= x.size())
		xend = x.size() - 1;
	Point temp;
	temp.idx = (size_t) -1;
	temp.y = DBL_MAX;
	if (xstart > xend)
		return temp;
	for (size_t n = xstart; n <= xend; ++n) {
		if (y[n] < temp.y) {
			temp.idx = n;
			temp.x = x[n];
			temp.y = y[n];
		}
	}
	return temp;
}

double DependentVector::Mean() const {
	const double mean = Area();
	if (cyclic) {
		return mean / cyclelength;
	} else {
		double sumx = 0.0;
		const size_t N = x.size();
		for (size_t n = 1; n < N; ++n) {
//			if(y[n - 1] == DBL_MAX || y[n - 1] == -DBL_MAX || y[n] == DBL_MAX
//					|| y[n] == -DBL_MAX) continue;
			sumx += x[n] - x[n - 1];
		}
		return mean / sumx;
	}
}

double DependentVector::Area() const {
	const size_t N = x.size();
	double area = 0.0;
	for (size_t n = 1; n < N; ++n) {
//		if(y[n - 1] == DBL_MAX || y[n - 1] == -DBL_MAX || y[n] == DBL_MAX
//				|| y[n] == -DBL_MAX) continue;
		area += (y[n] + y[n - 1]) * (x[n] - x[n - 1]) / 2.0;
	}
	if (cyclic) {
		area += (y[0] + y[N - 1]) * (x[0] + cyclelength - x[N - 1]) / 2.0;
	}
	return area;
}

DependentVector DependentVector::Range(size_t xstart, size_t xend) const {
	if (xend >= x.size())
		xend = x.size() - 1;
	if (xstart > xend)
		throw(std::runtime_error(
		__FILE__"DependentVector::Range: i0 > i1"));
	DependentVector temp;
	temp.Resize(xend - xstart + 1);
	std::copy_n(x.begin() + xstart, xend - xstart + 1, temp.x.begin());
	std::copy_n(y.begin() + xstart, xend - xstart + 1, temp.y.begin());
	return temp;
}

void DependentVector::Sort() {
	const size_t N = x.size();
	if (N <= 1)
		return;
// Shortcut, maybe the x vector is simply ordered in reverse.
	if (x[0] > x[N - 1])
		Reverse();
	bool flag = true;
	while (flag) {
		flag = false;
		for (size_t n = 1; n < N; ++n) {
			if (x[n - 1] > x[n]) {
				flag = true;
				std::swap(x[n - 1], x[n]);
				std::swap(y[n - 1], y[n]);
			}
		}
		if (!flag)
			break;
		flag = false;
		for (size_t n = (N - 1); n > 0; --n) {
			if (x[n - 1] > x[n]) {
				flag = true;
				std::swap(x[n - 1], x[n]);
				std::swap(y[n - 1], y[n]);
			}
		}
	}
}

void DependentVector::Unwrap(double tol) {
	if (tol <= 0.0)
		throw(std::domain_error(
		__FILE__"DependentVector::Unwrap: 'tol' should be a positive number."));
	for (size_t n = 1; n < y.size(); ++n) {
		while (y[n] < y[n - 1] - tol)
			y[n] += 2 * tol;
		while (y[n] > y[n - 1] + tol)
			y[n] -= 2 * tol;
	}
}

void DependentVector::Reverse() {
	if (x.size() <= 1)
		return;
	std::reverse(x.begin(), x.end());
	std::reverse(y.begin(), y.end());
}

void DependentVector::Resample(size_t Nnew) {
//TODO Test all edge-cases: linear, cyclic, cyclic with overlapping ends
	if (Nnew < 2)
		return;
	size_t N = x.size();
	bool overlappingends = false;
	if (cyclic) {
		if (x[N - 1] > x[0]) {
			if (fabs(x[N - 1] - (x[0] + cyclelength)) < 1e-6) {
				overlappingends = true;
			} else {
				// Let the ends overlap, while interpolating
				x.push_back(x[0] + cyclelength);
				y.push_back(y[0]);
				N++;
				Nnew++;
			}
		} else {
			if (fabs(x[N - 1] - (x[0] - cyclelength)) < 1e-6) {
				overlappingends = true;
			} else {
				// Let the ends overlap, while interpolating
				x.push_back(x[0] - cyclelength);
				y.push_back(y[0]);
				N++;
				Nnew++;
			}
		}
	}
	std::vector<double> xnew(Nnew);
	std::vector<double> ynew(Nnew);
// First and last point are fixed
	xnew[0] = x[0];
	ynew[0] = y[0];
	xnew[Nnew - 1] = x[N - 1];
	ynew[Nnew - 1] = y[N - 1];
	{
		// Linear x
		double xinterp = x[0];
		const double dxi = (x[N - 1] - x[0]) / (double) (Nnew - 1);
		for (size_t n = 1; n < (Nnew - 1); ++n) {
			xinterp += dxi;
			xnew[n] = xinterp;
		}
	}
	if (x[N - 1] > x[0]) {
		// Interpolate y normally
		size_t p = 0;
		for (size_t n = 1; n < (Nnew - 1); ++n) {
			while (x[p] < xnew[n])
				++p;
			ynew[n] = y[p - 1]
					+ (y[p] - y[p - 1]) / (x[p] - x[p - 1])
							* (xnew[n] - x[p - 1]);
		}
	} else {
		// Interpolate y if x is reversed
		size_t p = 0;
		for (size_t n = 1; n < (Nnew - 1); ++n) {
			while (x[p] > xnew[n])
				++p;
			ynew[n] = y[p - 1]
					+ (y[p] - y[p - 1]) / (x[p] - x[p - 1])
							* (xnew[n] - x[p - 1]);
		}
	}
	if (cyclic && !overlappingends) {
		// Remove extra points
		xnew.pop_back();
		ynew.pop_back();
	}
	x.swap(xnew);
	y.swap(ynew);
}

void DependentVector::CumSum() {
	const size_t N = y.size();
	for (size_t n = 1; n < N; ++n)
		y[n] += y[n - 1];
}

void DependentVector::CumProd() {
	const size_t N = y.size();
	for (size_t n = 1; n < N; ++n)
		y[n] *= y[n - 1];
}

void DependentVector::Integrate() {
	const size_t N = y.size();
	std::vector<double> temp(N);
	temp[0] = 0.0;
	for (size_t n = 1; n < N; ++n)
		temp[n] = temp[n - 1] + (y[n] + y[n - 1]) * (x[n] - x[n - 1]) / 2.0;
	y.swap(temp);
}

void DependentVector::Derive() {
	const size_t N = y.size();
	std::vector<double> temp(N);
	for (size_t n = 1; (n + 1) < N; ++n) {
		const double den = x[n + 1] - x[n - 1];
		if (fabs(den) < 1e-9) {
			temp[n] = 0.0;
		} else {
			temp[n] = (y[n + 1] - y[n - 1]) / den;
		}
	}
	{
		const double den = x[1] - x[0];
		if (fabs(den) < 1e-9) {
			temp[0] = 0.0;
		} else {
			temp[0] = (y[1] - y[0]) / den;
		}
	}
	{
		const double den = x[N - 1] - x[N - 2];
		if (fabs(den) < 1e-9) {
			temp[N - 1] = 0.0;
		} else {
			temp[N - 1] = (y[N - 1] - y[N - 2]) / den;
		}
	}

	y.swap(temp);
}

std::vector<DependentVector::Point> DependentVector::FindPeaks(
		const double minvalue, size_t xstart, size_t xend) {
	std::vector<Point> result;
	size_t N = Size();
	if (N < 2)
		return result;

	bool usecyclicevaluation = cyclic && xstart == 0 && xend == (size_t) -1;
	if (usecyclicevaluation) {
		// Prevent errors in cases, where the first and last x match up by
		// removing the last sample.
		if (fabs(x[N - 1] - x[0] - cyclelength) < 1e-6)
			--N;
		xend = N - 1;
	} else {
		if (N < 3)
			return result;
		if (xend > N - 3)
			xend = N - 3;
		if (xstart > xend)
			return result;
	}

	for (size_t n = xstart; n <= xend; ++n) {
		const double x0 = x[n];
		const double x1 = (
				(n + 1 >= N) ? (x[(n + 1) % N] + cyclelength) : x[n + 1]);
		const double x2 = (
				(n + 2 >= N) ? (x[(n + 2) % N] + cyclelength) : x[n + 2]);
		const double y0 = y[n];
		const double y1 = y[(n + 1) % N];
		const double y2 = y[(n + 2) % N];
		if (y1 < minvalue)
			continue;
		if (y0 >= y1 || y2 >= y1)
			continue;

		// Interpolate peaks with 2nd order polynoms to find the position and value of the maximum.
		const double dx1 = x1 - x0;
		const double dx2 = x2 - x0;
		const double den1 = 2 * (dx1 * y2 - dx2 * y1 + (dx2 - dx1) * y0);
		const double den2 = -2 * den1 * dx1 * dx2 * (dx2 - dx1);
		if (den1 == 0.0 || den2 == 0.0)
			continue;
		const double pos = (dx1 * (2 * x0 + dx1) * y2
				- dx2 * (dx2 + 2 * x0) * y1
				+ (dx2 - dx1) * (2 * x0 + dx2 + dx1) * y0) / den1;
		const double value = (((dx2 - dx1) * (dx2 - dx1) * (dx2 - dx1)
				* (dx2 - dx1) * y0 * y0)
				- (2 * dx2 * dx2 * (dx2 - dx1) * (dx2 - dx1) * y0 * y1)
				+ (dx2 * dx2 * dx2 * dx2 * y1 * y1)
				+ (dx1 * dx1 * dx1 * dx1 * y2 * y2)
				- (2 * dx1 * dx1
						* (dx1 * dx1 * y0 - 2 * dx1 * dx2 * y0 + dx2 * dx2 * y0
								+ dx2 * dx2 * y1) * y2)) / den2;

		auto ptpos = result.begin();
		while (ptpos != result.end() && ptpos->y > value) {
			++ptpos;
		}
		if (ptpos == result.end()) {
			result.push_back(Point((n + 1) % N, pos, value));
		} else {
			result.insert(ptpos, Point((n + 1) % N, pos, value));
		}
	}
	return result;
}

std::vector<DependentVector::Point> DependentVector::FindValleys(
		const double maxvalue, size_t xstart, size_t xend) {
	std::vector<Point> result;
	size_t N = Size();
	if (N < 2)
		return result;
	bool usecyclicevaluation = cyclic && xstart == 0 && xend == (size_t) -1;
	if (usecyclicevaluation) {
		// Prevent errors in cases, where the first and last x match up by
		// removing the last sample.
		if (fabs(x[N - 1] - x[0] - cyclelength) < 1e-6)
			--N;
		xend = N - 1;
	} else {
		if (N < 3)
			return result;
		if (xend > N - 3)
			xend = N - 3;
		if (xstart > xend)
			return result;
	}
	for (size_t n = xstart; n <= xend; ++n) {
		const double x0 = x[n];
		const double x1 = (
				(n + 1 >= N) ? (x[(n + 1) % N] + cyclelength) : x[n + 1]);
		const double x2 = (
				(n + 2 >= N) ? (x[(n + 2) % N] + cyclelength) : x[n + 2]);
		const double y0 = y[n];
		const double y1 = y[(n + 1) % N];
		const double y2 = y[(n + 2) % N];
		if (y1 >= maxvalue)
			continue;
		if (y0 <= y1 || y2 <= y1)
			continue;

		// Interpolate peaks with 2nd order polynoms to find the position and value of the maximum.
		const double dx1 = x1 - x0;
		const double dx2 = x2 - x0;
		const double den1 = 2 * (dx1 * y2 - dx2 * y1 + (dx2 - dx1) * y0);
		const double den2 = -2 * den1 * dx1 * dx2 * (dx2 - dx1);
		if (den1 == 0.0 || den2 == 0.0)
			continue;
		const double pos = (dx1 * (2 * x0 + dx1) * y2
				- dx2 * (dx2 + 2 * x0) * y1
				+ (dx2 - dx1) * (2 * x0 + dx2 + dx1) * y0) / den1;
		const double value = (((dx2 - dx1) * (dx2 - dx1) * (dx2 - dx1)
				* (dx2 - dx1) * y0 * y0)
				- (2 * dx2 * dx2 * (dx2 - dx1) * (dx2 - dx1) * y0 * y1)
				+ (dx2 * dx2 * dx2 * dx2 * y1 * y1)
				+ (dx1 * dx1 * dx1 * dx1 * y2 * y2)
				- (2 * dx1 * dx1
						* (dx1 * dx1 * y0 - 2 * dx1 * dx2 * y0 + dx2 * dx2 * y0
								+ dx2 * dx2 * y1) * y2)) / den2;

		auto ptpos = result.begin();
		while (ptpos != result.end() && ptpos->y < value) {
			++ptpos;
		}
		if (ptpos == result.end()) {
			result.push_back(Point((n + 1) % N, pos, value));
		} else {
			result.insert(ptpos, Point((n + 1) % N, pos, value));
		}
	}
	return result;
}

void DependentVector::Paint() const {
	glBegin(GL_LINE_STRIP);
	if (cyclic) {
		for (size_t n = 0; n < x.size(); ++n) {
			double co = cos(x[n]);
			double si = sin(x[n]);
			double d = y[n];
			glVertex2d(co * d, si * d);
		}
	} else {
		for (size_t n = 0; n < x.size(); ++n)
			glVertex2d(x[n], y[n]);
	}
	glEnd();
}

void DependentVector::PaintCircle(double radius) {
	glBegin(GL_LINE_LOOP);
	for (size_t n = 0; n < 100; ++n) {
		const float co = cos(2 * M_PI / 100 * n);
		const float si = sin(2 * M_PI / 100 * n);
		glNormal3f(co, si, 0);
		glVertex2f(co * radius, si * radius);
	}
	glEnd();
}

