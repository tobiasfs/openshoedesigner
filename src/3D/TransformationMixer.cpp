///////////////////////////////////////////////////////////////////////////////
// Name               : TransformationMixer.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.06.2020
// Copyright          : (C) 2020 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "TransformationMixer.h"

#include <numeric>

void TransformationMixer::SetBackground(double strength,
		std::function<Vector3(Vector3)> matrix) {
	this->backgroundstrength = strength;
	this->backgroundtransformation = matrix;
}
size_t TransformationMixer::AddSphere(Vector3 center,
		std::function<double(double)> kernel, std::function<Vector3(Vector3)> matrix) {
	auto temp = elements.emplace(elements.end());
	temp->type = Element::Type::Sphere;
	temp->v = center;
	temp->kernel = kernel;
	temp->m = matrix;
	mixing.resize(elements.size());
	return elements.size() - 1;
}

size_t TransformationMixer::AddCylinder(Vector3 center, Vector3 normal,
		std::function<double(double)> kernel, std::function<Vector3(Vector3)> matrix) {
	auto temp = elements.emplace(elements.end());
	temp->type = Element::Type::Cylinder;
	temp->v = center;
	temp->n = normal.Normal();
	temp->kernel = kernel;
	temp->m = matrix;
	mixing.resize(elements.size());
	return elements.size() - 1;
}

size_t TransformationMixer::AddPlane(double distance, Vector3 normal,
		std::function<double(double)> kernel, std::function<Vector3(Vector3)> matrix) {
	auto temp = elements.emplace(elements.end());
	temp->type = Element::Type::Plane;
	temp->n = normal.Normal();
	temp->d = distance;
	temp->kernel = kernel;
	temp->m = matrix;
	mixing.resize(elements.size());
	return elements.size() - 1;
}

size_t TransformationMixer::AddPlane(Vector3 pointonplane, Vector3 normal,
		std::function<double(double)> kernel, std::function<Vector3(Vector3)> matrix) {
	auto temp = elements.emplace(elements.end());
	temp->type = Element::Type::Plane;
	temp->n = normal.Normal();
	temp->d = pointonplane.Dot(normal);
	temp->kernel = kernel;
	temp->m = matrix;
	mixing.resize(elements.size());
	return elements.size() - 1;
}

size_t TransformationMixer::Size(void) const {
	return elements.size();
}

Vector3 TransformationMixer::operator ()(const Vector3 &v) const {
	if (elements.empty())
		return backgroundtransformation(v);
	size_t n = 0;
	for (auto &e : elements) {
		switch (e.type) {
		case Element::Type::Sphere: {
			const double d = (v - e.v).Abs();
			mixing[n] = e.kernel(d);
			break;
		}
		case Element::Type::Cylinder: {
			const double a = (v - e.v).Dot(e.n);
			const double d = (v - e.v - e.n * a).Abs();
			mixing[n] = e.kernel(d);
			break;
		}
		case Element::Type::Plane: {
			const double d = v.Dot(e.n) - e.d;
			mixing[n] = e.kernel(d);
			break;
		}
		}
		++n;
	}

	double sum = mixing.sum();
	double back = backgroundstrength - sum;
	if (back < 0.0)
		back = 0.0;
	sum += back;
	if (fabs(sum) <= 1e-12)
		return backgroundtransformation(v);

	mixing /= sum;
	back /= sum;
	Vector3 temp = backgroundtransformation(v) * back;
	n = 0;
	for (auto &e : elements) {
		++n;
		if (fabs(mixing[n - 1]) < 1e-12)
			continue;
		temp += e.m(v) * mixing[n - 1];
	}
	return temp;
}

