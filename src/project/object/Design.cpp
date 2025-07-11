///////////////////////////////////////////////////////////////////////////////
// Name               : Design.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 04.02.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#include "Design.h"
#include "../../math/Exporter.h"
#include "../../math/Matrix.h"
#include "../../math/Polynomial.h"

#include <cfloat>
#include <Eigen/Dense>

static const size_t nothing = (size_t) -1;

Design::Vector2 Design::Vector2::Normal() const {
	const double d = Abs();
	if (d > FLT_EPSILON) {
		return Vector2(u / d, v / d);
	}
	return *this;
}

void Design::Vector2::Normalize() {
	*this = Vector2::Normal();
}

Design::Vector2 Design::Vector2::RotatedBy(double angle) const {
	const double c = std::cos(angle);
	const double s = std::sin(angle);
	return Vector2(c * u - s * v, c * v + s * u);
}

bool Design::Vector2::IsZero() const {
	return std::fabs(u) < FLT_EPSILON && std::fabs(v) < FLT_EPSILON;
}
void Design::Edge::UpdatePolynomials(const std::vector<Vertex> &v) {
	const size_t N = vidx.size();

	std::vector<Vector2> temp;
	temp.reserve(N);
	for (size_t idx : vidx)
		temp.push_back(v[idx]);

	if (N > 0) {
		while (temp.front().u <= -M_PI)
			temp.front().u += 2.0 * M_PI;
		while (temp.front().u > M_PI)
			temp.front().u -= 2.0 * M_PI;
		double ru = temp.front().u;
		for (size_t idx = 1; idx < N; idx++) {
			while (temp[idx].u - ru <= -M_PI)
				temp[idx].u += 2.0 * M_PI;
			while (temp[idx].u - ru > M_PI)
				temp[idx].u -= 2.0 * M_PI;
			ru = temp[idx].u;
		}
	}
	switch (N) {
	case 0:
		iu = Polynomial::ByBezier(0.0);
		iv = Polynomial::ByBezier(0.0);
		break;
	case 1:
		iu = Polynomial::ByBezier(temp[0].u);
		iv = Polynomial::ByBezier(temp[0].v);
		break;
	case 2:
		iu = Polynomial::ByBezier(temp[0].u, temp[1].u);
		iv = Polynomial::ByBezier(temp[0].v, temp[1].v);
		break;
	case 3:
		iu = Polynomial::ByBezier(temp[0].u, temp[1].u, temp[2].u);
		iv = Polynomial::ByBezier(temp[0].v, temp[1].v, temp[2].v);
		break;
	case 4:
		iu = Polynomial::ByBezier(temp[0].u, temp[1].u, temp[2].u, temp[3].u);
		iv = Polynomial::ByBezier(temp[0].v, temp[1].v, temp[2].v, temp[3].v);
		break;
	default:
		iu.clear();
		iv.clear();
		for (Vector2 &vect : temp) {
			iu.push_back(vect.u);
			iv.push_back(vect.v);
		}
		break;
	}
}

Design::Vector2 Design::Edge::operator ()(double r) const {
	const size_t N = iu.size();
	if (N <= 4) {
		double u = iu(r);
		double v = iv(r);
		while (u <= -M_PI)
			u += 2.0 * M_PI;
		while (u > M_PI)
			u -= 2.0 * M_PI;
		return {u,v};
	}
	std::vector<Vector2> temp;
	temp.reserve(N - 1);
	Vector2 t(iu.front(), iv.front());
	for (size_t idx = 1; idx < N; idx++) {
		Vector2 t2(iu[idx], iv[idx]);
		temp.push_back(t.Interp(t2, r));
		t = t2;
	}

	for (size_t n = 1; n < (N - 1); n++) {
		for (size_t m = 1; m < (N - n); m++) {
			temp[m - 1] = temp[m - 1].Interp(temp[m], r);
		}
	}
	while (temp.front().u <= -M_PI)
		temp.front().u += 2.0 * M_PI;
	while (temp.front().u > M_PI)
		temp.front().u -= 2.0 * M_PI;
	return temp.front();
}

void Design::Patch::Update(double edgeLength) {
	Geometry::Clear();
}

Design::Condition Design::Condition::VertexOnEdge(size_t vidx, size_t eidx) {
	Condition ret;
	ret.v.push_back(vidx);
	ret.vidx = vidx;
	ret.eidx = eidx;
	ret.inPoint = true;
	ret.rSearch = true;
	return ret;
}

Design::Condition Design::Condition::VertexOnEdge(size_t vidx, size_t eidx,
		double r) {
	Condition ret;
	ret.v.push_back(vidx);
	ret.vidx = vidx;
	ret.eidx = eidx;
	ret.inPoint = true;
	ret.r = r;
	return ret;
}

Design::Condition Design::Condition::VerticesOnHorizontalLine(
		std::initializer_list<size_t> idx, double v) {
	Condition ret;
	ret.v = idx;
	ret.p0.v = v;
	return ret;
}

Design::Condition Design::Condition::VerticesOnVerticalLine(
		std::initializer_list<size_t> idx, double u) {
	Condition ret;
	ret.v = idx;
	ret.p0.u = u;
	ret.angle = M_PI_2;
	return ret;
}

Design::Condition Design::Condition::VerticesOnLine(
		std::initializer_list<size_t> idx, double angle) {
	Condition ret;
	ret.v = idx;
	ret.angle = angle;
	ret.moveParallel = true;
	return ret;
}

Design::Condition Design::Condition::VerticesSymmetricOnLineThroughPoint(
		std::initializer_list<size_t> idx, size_t vidx, double angle) {
	Condition ret;
	ret.symmetric = true;
	ret.v = idx;
	ret.angle = angle;
	ret.vidx = vidx;
	return ret;
}

size_t Design::Condition::CountEquations() const {
	return v.size() * 2;
}

size_t Design::Condition::CountFreeParameter() const {
	if (symmetric) {
		return v.size();
	}
	return ((moveParallel && !inPoint) ? 1 : 0) + (inPoint ? 0 : v.size());
}

size_t Design::ConditionRow(size_t cidx, size_t vidx) const {
	size_t row = 0;
	size_t idx = 0;
	if (idx == cidx)
		return row + vidx * 2;
	for (const Condition &c : conditions) {
		row += c.CountEquations();
		idx++;
		if (idx == cidx)
			return row + vidx * 2;
	}
	return row;
}

size_t Design::ConditionCol(size_t cidx, size_t vidx) const {
	size_t col = vertices.size() * 2;
	size_t idx = 0;
	if (idx == cidx)
		return col + vidx;
	for (const Condition &c : conditions) {
		col += c.CountFreeParameter();
		idx++;
		if (idx == cidx)
			return col + vidx;
	}
	return col;
}
Design::Design() {
	vertices.emplace_back(-M_PI + 0.1, 0.8);
	vertices.emplace_back(-2, 0.25);
	vertices.emplace_back(-2.1, 0.1);
	vertices.emplace_back(-2, 0.8);
	vertices.emplace_back(-1.5, 0.75);

	vertices.emplace_back(-1.0, 0.0);
	vertices.emplace_back(-1.0, 0.6);
	vertices.emplace_back(-1.0, 0.5);

	vertices.emplace_back(-0.5, 0.0);
	vertices.emplace_back(-0.5, 0.2);

	vertices.emplace_back(0.0, 0.75);

	vertices.emplace_back(0.4, 0.0);
	vertices.emplace_back(0.5, 0.2);

	vertices.emplace_back(1.0, 0.0);
	vertices.emplace_back(1.0, 0.6);
	vertices.emplace_back(1.0, 0.5);

	vertices.emplace_back(1.5, 0.65);
	vertices.emplace_back(2, 0.0);
	vertices.emplace_back(2.3, 0.8);
	vertices.emplace_back(2, 0.25);

	edges.emplace_back((std::initializer_list<size_t> ) { 0, 3, 2 });
	edges.emplace_back((std::initializer_list<size_t> ) { 5, 6, 14, 13 });
	edges.emplace_back((std::initializer_list<size_t> ) { 8, 9 });
	edges.emplace_back((std::initializer_list<size_t> ) { 11, 12 });
	edges.emplace_back((std::initializer_list<size_t> ) { 1, 7, 4, 10 });
	edges.emplace_back((std::initializer_list<size_t> ) { 10, 16, 15, 19 });
	edges.emplace_back((std::initializer_list<size_t> ) { 17, 18, 0 });

	conditions.emplace_back(Condition::VertexOnEdge(1, 0));
	conditions.emplace_back(Condition::VertexOnEdge(19, 6));

	conditions.emplace_back(Condition::VerticesOnHorizontalLine( { 2, 5, 8, 11,
			13, 17 }, 0.0));
	conditions.emplace_back(Condition::VerticesOnVerticalLine( { 0 }, -M_PI));
	conditions.emplace_back(Condition::VerticesOnVerticalLine( { 10 }, 0.0));

	conditions.emplace_back(Condition::VerticesOnLine( { 4, 10, 16 }, 0));
	conditions.emplace_back(Condition::VerticesSymmetricOnLineThroughPoint( { 2,
			17, 3, 18, 1, 19, 4, 16, 5, 13, 6, 14, 7, 15, 8, 11, 9, 12 }, 10,
	M_PI_2));

	conditions.emplace_back(Condition::VerticesOnLine( { 5, 6 }, M_PI_2));
	conditions.emplace_back(Condition::VerticesOnLine( { 8, 9 }, M_PI_2));

	conditions.emplace_back(Condition::VerticesOnLine( { 0, 3 }, 0));
	conditions.emplace_back(Condition::VerticesOnLine( { 2, 3 }, M_PI_2));
}

void Design::PrepareBoundaryConditions() {

	for (Condition &c : conditions) {
		// 1. Init boundary lines
		if (c.eidx == nothing) {
			if (c.vidx != nothing)
				c.p0 = vertices[c.vidx];
			c.t.u = 1;
			c.t.v = 0;
		} else {
			const Edge &e = edges[c.eidx];
			if (c.rSearch) {
				auto dist = [&c, &e, &v = vertices](double r) {
					Vector2 p = e(r);
					if (c.vidx == (size_t) -1) {
						p -= c.p0;
					} else {
						p -= v[c.vidx];
					}
					return p.Abs();
				};
				// Initial search
				const size_t N = e.Size() * 2;
				Polynomial rTest = Polynomial::ByValue(0, 0, N - 1, 1);
				double dMin = DBL_MAX;
				for (size_t n = 0; n < N; n++) {
					double rCandidate = rTest(n);
					double d = dist(rCandidate);
					if (d < dMin) {
						dMin = d;
						c.r = rCandidate;
					}
				}
				// Newton-Raphson to find the minimum distance
				// Limited to N iterations (= 2 * the number of coefficients)
				// Limited to an improvement of min. FLT_EPSILON per iteration
				double dr = rTest(1.0) - rTest(0.0);
				double d1 = dist(c.r);
				size_t iterations = 0;
				while (iterations < N) {
					iterations++;
					const double d0 = dist(c.r - dr);
					const double d2 = dist(c.r + dr);
					// Parabula-fit and finding the minimum of the parabula
					const double T1 = (d0 - d2) * dr;
					const double T2 = 2.0 * (d2 - 2 * d1 + d0);
					if (fabs(T2) <= FLT_EPSILON)
						break;
					const double rUpdate = c.r + T1 / T2;
					const double dUpdate = dist(rUpdate);
					if (dUpdate > d1 - FLT_EPSILON)
						break;
					d1 = dUpdate;
					c.r = rUpdate;
					if (c.r < c.rmin) {
						c.r = c.rmin;
						break;
					}
					if (c.r > c.rmax) {
						c.r = c.rmax;
						break;
					}
					dr /= 2.0;
				}
			}
			c.p0 = e(c.r);

			if (c.angleOrigin == Condition::Angle::VERTEX && c.vidx != nothing
					&& !c.inPoint) {
				// Tangent points from edge to vertex
				c.t = vertices[c.vidx] - c.p0;
				c.t.Normalize();
			} else if (c.angleOrigin == Condition::Angle::EDGE && !c.inPoint) {
				// Tangent along the edge
				const double dr = 1e-6;
				const Vector2 e0 = e(c.r - dr);
				const Vector2 e1 = e(c.r + dr);
				c.t = (e1 - e0).Normal();
			} else {
				// Global tangent in 0 deg orientation
				c.t.u = 1.0;
				c.t.v = 0.0;
			}
		}

		// 2. Rotate the line by angle
		c.t = c.t.RotatedBy(c.angle);
		c.n = c.t.Orthogonal();

		// 4. Calculate initial solution
		c.vinit.clear();
		c.vinit.reserve(c.v.size());

		if (c.symmetric) {
			if (c.inPoint) {
				// Point symmetry
				for (size_t idx1 = 1; idx1 < c.v.size(); idx1 += 2) {
					size_t idx0 = idx1 - 1;
					Vector2 v0 = vertices[c.v[idx0]] - c.p0;
					Vector2 v1 = vertices[c.v[idx1]] - c.p0;
					Vector2 err = 0.5 * (v0 + v1);
					c.vinit.push_back(v0 - err + c.p0);
					c.vinit.push_back(v1 - err + c.p0);
				}
			} else {
				// Line Symmetry
				for (size_t idx1 = 1; idx1 < c.v.size(); idx1 += 2) {
					size_t idx0 = idx1 - 1;
					Vector2 v0 = vertices[c.v[idx0]] - c.p0;
					Vector2 v1 = vertices[c.v[idx1]] - c.p0;
					double x0 = v0 * c.t;
					double y0 = v0 * c.n;
					double x1 = v1 * c.t;
					double y1 = v1 * c.n;
					x0 = (x0 + x1) / 2.0;
					x1 = x0;
					y0 = (y0 - y1) / 2.0;
					y1 = -y0;
					c.vinit.push_back(Vector2(c.p0 + x0 * c.t + y0 * c.n));
					c.vinit.push_back(Vector2(c.p0 + x1 * c.t + y1 * c.n));
				}
			}
		} else {
			if (c.inPoint) {
				c.vinit.assign(c.v.size(), c.p0);
			} else {
				if (c.moveParallel) {
					// Move the line such that all vertices have the average
					// minimal distance to the line along n.
					double f = 0.0;
					for (size_t idx : c.v) {
						const Vertex &v = vertices[idx];
						f += (v - c.p0) * c.n;
					}
					c.p0 += (f / (double) c.v.size()) * c.n;
				}
				// Move all vertices onto the line along n.
				for (size_t idx : c.v) {
					const Vertex &v = vertices[idx];
					double f = (v - c.p0) * c.n;
					c.vinit.push_back(v - f * c.n);
				}
			}
		}
	}
}

void Design::UpdateEdges() {

	std::set<size_t> uncovered;
	std::set<size_t> interacting;
	for (size_t idx = 0; idx < vertices.size(); idx++)
		uncovered.insert(idx);
	for (const Condition &c : conditions) {
		for (size_t vidx : c.v)
			uncovered.erase(vidx);
		if (c.eidx != nothing)
			for (size_t vidx : edges[c.eidx].vidx)
				interacting.insert(vidx);
	}

	size_t M = ConditionRow(conditions.size()) + uncovered.size() * 2;
	size_t N = ConditionCol(conditions.size());
	Eigen::MatrixXd A;
	Eigen::MatrixXd b;

	int iteration = 0;
	while (iteration < 5) {
		iteration++;
		// UpdateEdges all edges for interpolation
		for (Edge &e : edges)
			e.UpdatePolynomials(vertices);

		PrepareBoundaryConditions();

		// Fill conditions into matrices A, b
		A = Eigen::MatrixXd::Zero(M, N);
		b = Eigen::MatrixXd::Zero(M, 1);
		size_t offset = 0;
		size_t offsetcol = vertices.size() * 2;
		for (const Condition &c : conditions) {

			if (c.symmetric) {
				for (size_t idx1 = 1; idx1 < c.v.size(); idx1 += 2) {
					size_t idx0 = idx1 - 1;
					size_t vidx0 = c.v[idx0];
					size_t vidx1 = c.v[idx1];
					A(offset + 0, vidx0 * 2 + 0) = 1;
					A(offset + 1, vidx0 * 2 + 1) = 1;
					A(offset + 2, vidx1 * 2 + 0) = 1;
					A(offset + 3, vidx1 * 2 + 1) = 1;
					b(offset + 0, 0) = c.vinit[idx0].u;
					b(offset + 1, 0) = c.vinit[idx0].v;
					b(offset + 2, 0) = c.vinit[idx1].u;
					b(offset + 3, 0) = c.vinit[idx1].v;
					A(offset + 0, offsetcol) = -c.t.u;
					A(offset + 1, offsetcol) = -c.t.v;
					if (c.inPoint) {
						A(offset + 2, offsetcol) = c.t.u;
						A(offset + 3, offsetcol) = c.t.v;
					} else {
						A(offset + 2, offsetcol) = -c.t.u;
						A(offset + 3, offsetcol) = -c.t.v;
					}
					offsetcol++;
					A(offset + 0, offsetcol) = -c.n.u;
					A(offset + 1, offsetcol) = -c.n.v;
					A(offset + 2, offsetcol) = c.n.u;
					A(offset + 3, offsetcol) = c.n.v;
					offsetcol++;
					offset += 4;
				}
			} else {
				size_t offsetcol2 = offsetcol
						+ ((c.moveParallel && !c.inPoint) ? 1 : 0);
				for (size_t idx0 = 0; idx0 < c.v.size(); idx0++) {
					size_t vidx = c.v[idx0];
					A(offset + 0, vidx * 2 + 0) = 1;
					A(offset + 1, vidx * 2 + 1) = 1;
					b(offset + 0, 0) = c.vinit[idx0].u;
					b(offset + 1, 0) = c.vinit[idx0].v;
					if (c.moveParallel && !c.inPoint) {
						A(offset + 0, offsetcol) = -c.n.u;
						A(offset + 1, offsetcol) = -c.n.v;
					}
					if (!c.inPoint) {
						A(offset + 0, offsetcol2) = -c.t.u;
						A(offset + 1, offsetcol2) = -c.t.v;
						offsetcol2++;
					}

					offset += 2;
				}
				offsetcol = offsetcol2;
			}
		}
		for (size_t vidx : uncovered) {
			A(offset + 0, vidx * 2 + 0) = 1;
			A(offset + 1, vidx * 2 + 1) = 1;
			b(offset + 0, 0) = vertices[vidx].u;
			b(offset + 1, 0) = vertices[vidx].v;
			offset += 2;
		}

		Eigen::CompleteOrthogonalDecomposition Dec =
				A.completeOrthogonalDecomposition();
		Eigen::MatrixXd x0 = Dec.solve(b);
		Eigen::MatrixXd H = Eigen::MatrixXd::Identity(A.cols(), A.cols())
				- Dec.solve(A);
		Eigen::FullPivLU lu = H.fullPivLu();
		Eigen::MatrixXd L =
				lu.matrixLU().triangularView<Eigen::UpLoType::Lower>();

		Eigen::MatrixXd U =
				lu.matrixLU().triangularView<Eigen::UpLoType::Upper>();
		Eigen::MatrixXd P = lu.permutationP();
		Eigen::MatrixXd Q = lu.permutationQ();

		Eigen::MatrixXd S =
				(U * Q.transpose()).block(0, 0, lu.rank(), Q.cols()).transpose();

		Eigen::MatrixXd U2 = S.block(vertices.size() * 2, 0,
				S.rows() - vertices.size() * 2, S.cols());

		Eigen::CompleteOrthogonalDecomposition Dec2 =
				U2.completeOrthogonalDecomposition();

		Eigen::MatrixXd xend = x0.block(vertices.size() * 2, 0,
				x0.rows() - vertices.size() * 2, 1);

		Eigen::MatrixXd diff = Dec2.solve(-xend);

		Eigen::MatrixXd x = x0 + S * diff;

		// Copy the solution back into the vertices and check, if one or more
		// of the interacting vertices have changed position.
		bool runAgain = false;

		for (size_t vidx = 0; vidx < vertices.size(); vidx++) {
			Vertex &v = vertices[vidx];

			Vertex vnew(x(vidx * 2 + 0, 0), x(vidx * 2 + 1, 0));
			if ((v - vnew).Abs() > 1e-6
					&& interacting.find(vidx) != interacting.end())
				runAgain = true;

			// Limit the vertices to the normal region.

			v.u = vnew.u;
			while (v.u <= -M_PI)
				v.u += 2.0 * M_PI;
			while (v.u > M_PI)
				v.u -= 2.0 * M_PI;
			v.v = std::fmin(std::fmax(vnew.v, 0.0), 3.0);
		}
		{
			auto toMatrix = [](const std::string &name,
					const Eigen::MatrixXd &m) {
				Matrix ret(name, { (size_t) m.rows(), (size_t) m.cols() });
				for (int idx1 = 0; idx1 < m.cols(); idx1++)
					for (int idx0 = 0; idx0 < m.rows(); idx0++)
						ret(idx0, idx1) = m(idx0, idx1);
				return ret;
			};

			Exporter ex("/tmp/lu.mat");
			ex.WriteMatrix(toMatrix("x0", x0));
			ex.WriteMatrix(toMatrix("x", x));
			ex.WriteMatrix(toMatrix("A", A));
			ex.WriteMatrix(toMatrix("b", b));
			ex.WriteMatrix(toMatrix("H2", H));
			ex.WriteMatrix(toMatrix("P2", P));
			ex.WriteMatrix(toMatrix("L2", L));
			ex.WriteMatrix(toMatrix("U2", U));
			ex.WriteMatrix(toMatrix("Q2", Q));
			ex.WriteMatrix(toMatrix("diff", diff));

		}
		if (!runAgain)
			break;

	}

	// UpdateEdges all edges for interpolation
	for (Edge &e : edges)
		e.UpdatePolynomials(vertices);

// Inject the outline into the patch
	Polynomial p = Polynomial::ByValue(0, 0, 39, 1);
	for (Edge &e : edges) {
		e.geo.Clear();
		for (size_t n = 0; n < 40; n++) {
			Geometry::Vertex v;
			Vector2 pos = e(p(n));
			v.u = pos.u;
			v.v = pos.v;
			e.geo.AddEdgeToVertex(v);
		}
		e.geo.paintEdges = true;
	}

	MarkValid(true);
}

void Design::UpdatePatches() {
// Interpolate each patch.
	for (Patch &p : patches)
		p.Update(0.05);

	MarkValid(true);
}

void Design::Paint() const {
	for (const Patch &p : patches)
		p.Paint();
	for (const Edge &e : edges)
		e.geo.Paint();
}

