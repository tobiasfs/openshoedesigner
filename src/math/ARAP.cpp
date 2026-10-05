///////////////////////////////////////////////////////////////////////////////
// Name               : ARAP.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 02.09.2026
// Copyright          : (C) 2026 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "ARAP.h"

#include "Exporter.h"
#include "Matrix.h"

#include <algorithm>
#include <Eigen/Cholesky>
#include <Eigen/SparseQR>
#include <Eigen/Sparse>
#include <float.h>
#include <iostream>
#include <limits>
#include <numeric>
#include <execution>

ARAP::ARAP(Geometry &other) :
		Geometry(other) {
}

ARAP& ARAP::operator =(Geometry &other) {
	if (this == &other)
		return *this;
	Geometry::operator=(other);
	return *this;
}

void ARAP::MeasureDistances() {
	distances.clear();
	distances.reserve(CountEdges());
	for (size_t eidx = 0; eidx < CountEdges(); eidx++) {
		const Geometry::Vertex &v0 = GetEdgeVertex(eidx, 0);
		const Geometry::Vertex &v1 = GetEdgeVertex(eidx, 1);
		distances.push_back((v1 - v0).Abs());
	}
}

void ARAP::InitByUV() {
	for (size_t vidx = 0; vidx < CountVertices(); vidx++) {
		Geometry::Vertex &v = GetVertex(vidx);
		v.x = v.u;
		v.y = v.v;
		v.z = 0.0;
	}
}

void ARAP::RelaxUniform() { // Initial solution and scaling

	// Pseudoinverse solution of the overdetermined equation:
	// dist^2 == ax^2 * scalex^2 + ay^2 * scaley^2
	// via x = (A^t * A)^-1 * A^t * y
	Matrix A = Matrix::Zeros(2, 2);
	Matrix y = Matrix::Zeros(2, 1);
	for (size_t eidx = 0; eidx < CountEdges(); eidx++) {
		const Geometry::Vertex &v0 = GetEdgeVertex(eidx, 0);
		const Geometry::Vertex &v1 = GetEdgeVertex(eidx, 1);
		const double ax2 = std::pow(v1.x - v0.x, 2.0);
		const double ay2 = std::pow(v1.y - v0.y, 2.0);
		A(0, 0) += ax2 * ax2;
		A(1, 0) += ax2 * ay2;
		A(0, 1) += ax2 * ay2;
		A(1, 1) += ay2 * ay2;
		y(0, 0) += distances[eidx] * distances[eidx] * ax2;
		y(1, 0) += distances[eidx] * distances[eidx] * ay2;
	}

	double scalex = 1.0;
	double scaley = 1.0;
	if (A.Invert()) {
		Matrix x = A * y;
		scalex = sqrt(x(0));
		scaley = sqrt(x(1));
	}

	std::vector<Geometry::Vertex> q(CountVertices()); // Position
	for (size_t vidx = 0; vidx < CountVertices(); vidx++) {
		Geometry::Vertex &v = GetVertex(vidx);
		v.x *= scalex;
		v.y *= scaley;
		v.z = 0.0;
	}
}

void ARAP::Calculate() {

	typedef std::complex<double> Complex;

	ColorInitScale();

//	std::vector<Edge2D> e2ds;
//	e2ds.assign(CountEdges(), { 0.0, 0 });

	InitLocalTriangles();

	const double sigma = 1.0; // 1.0 = ARAP, 0.0 = ASAP

	const double phaseSolverRegularization = 0.0;

	// Pre-solve the position interpolation

	Eigen::SimplicialCholesky<Eigen::SparseMatrix<double>> chol;

	// The "+ 1" adds an additional condition, that v[0] is a fixed point at
	// the origin. It increases the rank of A to a full rank so that no
	// regularization is needed.
	Eigen::SparseMatrix<double> A(CountEdges() + 1, CountVertices());

	{
		std::vector<Eigen::Triplet<double>> T;
		// The 0 is the index of the vertex to be fixed to 0.0 or whatever is
		// assigned in bx and by at this position.
		T.push_back(Eigen::Triplet<double>(CountEdges(), 0, 1.0));
		for (size_t tidx = 0; tidx < CountTriangles(); tidx++) {
			const Triangle &tri = GetTriangle(tidx);
			const Triangle2D &t2d = t2ds[tidx];
			if (t2d.A < 1e-9)
				continue;
			for (int_fast8_t idx = 0; idx < 3; idx++) {
				const size_t eidx = tri.GetEdgeIndex(idx);
				const size_t vidx0 = tri.GetVertexIndex((idx + 0) % 3);
				const size_t vidx1 = tri.GetVertexIndex((idx + 1) % 3);
				const double f = t2d.omega[idx]
						* ((vidx0 < vidx1) ? 1.0 : -1.0);
				T.emplace_back(Eigen::Triplet<double>(eidx, vidx0, -f));
				T.emplace_back(Eigen::Triplet<double>(eidx, vidx1, f));
			}
		}
		A.setFromTriplets(T.begin(), T.end());

		Eigen::SparseMatrix<double> I(v.size(), v.size());
		I.setIdentity();

		Eigen::SparseMatrix<double> Areg = A.transpose() * A
				+ phaseSolverRegularization * I;

#ifdef DEBUG
//		std::cout << "Non-zeros: " << Areg.nonZeros() << std::endl;
//		std::cout << "Density: "
//				<< static_cast<double>(Areg.nonZeros())
//						/ (static_cast<double>(Areg.rows()) * Areg.cols())
//				<< std::endl;
//		std::cout << "Solving\n";
#endif
		chol.compute(Areg);
		if (chol.info() != Eigen::Success) {
			std::cout
					<< "Could not decompose matrix for the position alignment.\n";
		} else {
#ifdef DEBUG
			std::cout << "Decomposed matrix A successfully.\n";
#endif
		}
	}

	// Global angle alignment
	PhaseAlignRotations();

	// Main iteration loop
	for (size_t n = 0; n < Nmax; n++) {

		// Local Phase only after first round, so that the already calculated
		// angle alignment is not overwritten.
		if (n > 0)
			UpdateLocalRotations(sigma);

		// Global Phase: Position alignment

		Eigen::VectorXd bx = Eigen::VectorXd::Zero(CountEdges() + 1);
		Eigen::VectorXd by = Eigen::VectorXd::Zero(CountEdges() + 1);

		for (size_t tidx = 0; tidx < CountTriangles(); tidx++) {
			const Triangle &tri = GetTriangle(tidx);
			const Triangle2D &t2d = t2ds[tidx];
			if (t2d.A < 1e-9)
				continue;
			for (int_fast8_t idx = 0; idx < 3; idx++) {
				const size_t eidx = tri.GetEdgeIndex(idx);
				const size_t vidx0 = tri.GetVertexIndex((idx + 0) % 3);
				const size_t vidx1 = tri.GetVertexIndex((idx + 1) % 3);
				const double f = t2d.omega[idx]
						* ((vidx0 < vidx1) ? 1.0 : -1.0);

				const Complex xx = t2d.getEdgeVector(idx);
				const Complex res = t2d.L * xx;

				bx[eidx] += f * res.real();
				by[eidx] += f * res.imag();
			}
		}

		Eigen::VectorXd bregx = A.transpose() * bx;
		Eigen::VectorXd bregy = A.transpose() * by;
		Eigen::VectorXd Ux = chol.solve(bregx);
		Eigen::VectorXd Uy = chol.solve(bregy);
		for (size_t vidx = 0; vidx < CountVertices(); vidx++) {
			v[vidx].x = Ux(vidx);
			v[vidx].y = Uy(vidx);
		}

#ifdef DEBUG
		if (n + 1 == Nmax) {
//			Eigen::MatrixXd T = A.toDense();
//			Matrix Aexp(T.rows(), T.cols());
//			Matrix Bexp(T.rows(), 2);
//			Aexp = Matrix::Zeros(Aexp.GetDimensions());
//			for (size_t i = 0; i < A.rows(); i++) {
//				for (size_t j = 0; j < A.cols(); j++)
//					Aexp(i, j) = T(i, j);
//				Bexp(i, 0) = bx(i);
//				Bexp(i, 1) = by(i);
//			}
//			Exporter ex("/tmp/arap.mat");
//			ex.Add(Aexp, "A");
//			ex.Add(Bexp, "B");
		}
#endif
	}

	// Normalize rotation and position of the output mesh:
	{
		Vertex sum = std::reduce(std::execution::par, v.begin(), v.end(),
				Vertex { 0, 0, 0, 0, 0 },
				[](const Vertex &a, const Vertex &b) {
					return Vertex { a.x + b.x, a.y + b.y, a.z + b.z, a.u + b.u,
							a.v + b.v };
				});
		const double vertexCount = (double) (v.size());
		sum /= vertexCount;
		sum.u /= vertexCount;
		sum.v /= vertexCount;

		std::array<double, 4> S = { 0.0, 0.0, 0.0, 0.0 };
		for (const Vertex &vert : v) {
			S[0] += (vert.x - sum.x) * (vert.u - sum.u);
			S[1] += (vert.y - sum.y) * (vert.u - sum.u);
			S[2] += (vert.x - sum.x) * (vert.v - sum.v);
			S[3] += (vert.y - sum.y) * (vert.v - sum.v);
		}
		std::complex<double> rot = SolveProcrustes(S);
		AffineTransformMatrix m;
		m[0] = std::real(rot);
		m[1] = -std::imag(rot);
		m[4] = std::imag(rot);
		m[5] = std::real(rot);
		m *= AffineTransformMatrix::Translation(-sum.x, -sum.y, 0.0);
		Transform(m);
	}

#ifdef DEBUG
	debug.Clear();
//	std::cout << "X=[";
	for (size_t tidx = 0; tidx < CountTriangles(); tidx++) {
		const Triangle2D &t2d = t2ds[tidx];

//		std::cout << t2d.L.real() << "+" << t2d.L.imag() << "j";
//		if (tidx + 1 < CountTriangles())
//			std::cout << ";";

		const double s = 0.5;

		Vertex vd0;
		Vertex vd1;
		Vertex vd2;

		Complex res0 = t2d.L * Complex(t2d.x0, t2d.x1);
		Complex res1 = t2d.L * t2d.getEdgeVector(0);
		Complex res2 = t2d.L * t2d.getEdgeVector(1);

		vd0.x = t2d.xc - res0.real() * s;
		vd0.y = t2d.yc - res0.imag() * s;
		vd1.x = vd0.x + res1.real() * s;
		vd1.y = vd0.y + res1.imag() * s;
		vd2.x = vd1.x + res2.real() * s;
		vd2.y = vd1.y + res2.imag() * s;

		const Triangle &tr = GetTriangle(tidx);
		const size_t vidx0 = tr.GetVertexIndex(0);
		const size_t vidx1 = tr.GetVertexIndex(1);
		const size_t vidx2 = tr.GetVertexIndex(2);

		vd0.c = IndexToColor(vidx0);
		vd1.c = IndexToColor(vidx1);
		vd2.c = IndexToColor(vidx2);
		debug.AddTriangle(vd0, vd1, vd2);
		//			Triangle &dt = debug.GetTriangle(debug.CountTriangles() - 1);
		//			const size_t eidx0 = dt.GetEdgeIndex(0);
		//			const size_t eidx1 = dt.GetEdgeIndex(1);
		//			const size_t eidx2 = dt.GetEdgeIndex(2);
		//
		//			debug.GetEdge(eidx0).c = IndexToColor(vidx0);
		//			debug.GetEdge(eidx1).c = IndexToColor(vidx1);
		//			debug.GetEdge(eidx2).c = IndexToColor(vidx2);

	}
//	std::cout << "];" << std::endl;
	debug.verticesHaveColor = true;
#endif

	ColorErrors();
}

void ARAP::ColorInitScale() {
	std::vector<double> dev(e.size(), 0.0);
	size_t eidx = 0;
	for (const Edge &ed : e) {
		const Vertex &v0 = v[ed.va];
		const Vertex &v1 = v[ed.vb];
		const Vector3 di = v1 - v0;
		const double s = di.Abs() - distances[eidx];
		dev[eidx] = s;
		eidx++;
	}
	const double sum = std::accumulate(dev.begin(), dev.end(), 0.0);
	const double mean = sum / (double) dev.size();
	const double var = std::transform_reduce(dev.begin(), dev.end(), 0.0,
			std::plus<double>(), [mean](const double v) {
				const double d = v - mean;
				return d * d;
			});
	errorToColor = { -mean / sqrt(var), 40.0 / sqrt(var) };
}

void ARAP::ColorErrors() {
	size_t eidx = 0;
	for (Edge &ed : e) {
		const Vertex &v0 = v[ed.va];
		const Vertex &v1 = v[ed.vb];
		const Vector3 di = v1 - v0;
		const double s = di.Abs() - distances[eidx];
		double col = fmin(fmax(errorToColor(s), -1.0), 1.0);

		if (col > 0.0) {
			ed.c.r = 0.0;
			ed.c.g = col;
			ed.c.b = 0.0;
		} else {
			ed.c.r = -col;
			ed.c.g = 0.0;
			ed.c.b = 0.0;
		}

		eidx++;
	}
	edgesHaveColor = true;
}

void ARAP::InitLocalTriangles() {
	//	std::vector<Edge2D> e2ds;
	//	e2ds.assign(CountEdges(), { 0.0, 0 });
	// Calculate the initialization for the locally flattened triangles. 3D -> 2D
	// Vertices: (0,0), (x1,0), (x2,x2)
	t2ds.clear();
	t2ds.reserve(CountTriangles());
	for (const Geometry::Triangle &tri : t) {
		Triangle2D t2d;
		const size_t eidx0 = tri.GetEdgeIndex(0);
		const size_t eidx1 = tri.GetEdgeIndex(1);
		const size_t eidx2 = tri.GetEdgeIndex(2);
		size_t vidx0 = tri.GetVertexIndex(0);
		size_t vidx1 = tri.GetVertexIndex(1);
		size_t vidx2 = tri.GetVertexIndex(2);
		const Vertex &v0 = GetVertex(vidx0);
		const Vertex &v1 = GetVertex(vidx1);
		const Vertex &v2 = GetVertex(vidx2);
		const Vertex c = (v0 + v1 + v2) / 3.0;
		t2d.xc = c.x;
		t2d.yc = c.y;
		const double da = distances[tri.GetEdgeIndex(0)];
		const double db = distances[tri.GetEdgeIndex(1)];
		const double dc = distances[tri.GetEdgeIndex(2)];
		if (da < 1e-6) {
			t2d.x1 = 0.0;
			t2d.x2 = 0.0;
			t2d.y2 = db;
		} else {
			t2d.x1 = da;
			t2d.x2 = (da * da - db * db + dc * dc) / (2.0 * da);
			const double h = dc * dc - t2d.x2 * t2d.x2;
			if (h < 1e-6)
				t2d.y2 = 0.0;
			else
				t2d.y2 = sqrt(h);
		}
		t2d.x0 = (t2d.x1 + t2d.x2) / 3.0;
		t2d.y0 = t2d.y2 / 3.0;
		t2d.A = t2d.x1 * t2d.y2 / 2.0;
		t2d.omega[0] = VectorCotangens(t2d.x2 - t2d.x1, t2d.y2, -t2d.x2,
				-t2d.y2);
		t2d.omega[1] = VectorCotangens(-t2d.x2, -t2d.y2, t2d.x1, 0.0);
		t2d.omega[2] = VectorCotangens(t2d.x1, 0.0, t2d.x2 - t2d.x1, t2d.y2);
		//		t2d.omega = { 1, 1, 1 };
		//		e2ds[tri.ea].countTheta++;
		//		e2ds[tri.ea].sumTheta += t2d.omega[0];
		//		e2ds[tri.eb].countTheta++;
		//		e2ds[tri.eb].sumTheta += t2d.omega[1];
		//		e2ds[tri.ec].countTheta++;
		//		e2ds[tri.ec].sumTheta += t2d.omega[2];
		t2ds.push_back(t2d);
	}
}

void ARAP::PhaseAlignRotations() {
	// Global angle alignment
	{
		typedef std::complex<double> Complex;
		typedef Eigen::SparseMatrix<Complex> SpComplexMatrix;
		typedef Eigen::VectorXcd SpComplexVector;
		SpComplexMatrix L(CountEdges() + 1, CountTriangles());
		std::vector<Eigen::Triplet<Complex> > triplets;
		for (size_t eidx = 0; eidx < CountEdges(); eidx++) {
			const Edge &ed = GetEdge(eidx);
			if (ed.trianglecount < 2)
				continue;

			const Triangle &t0 = GetTriangle(ed.ta);
			const Triangle &t1 = GetTriangle(ed.tb);
			int_fast8_t en0 = t0.GetEdgePosition(eidx);
			int_fast8_t en1 = t1.GetEdgePosition(eidx);
			Complex d0 = t2ds[ed.ta].getEdgeVector(en0);
			Complex d1 = t2ds[ed.tb].getEdgeVector(en1);
			//				d0 += t2ds[ed.ta].L * d0;
			//				d1 += t2ds[ed.tb].L * d1;
			triplets.emplace_back(eidx, ed.ta, d0);
			triplets.emplace_back(eidx, ed.tb, d1);
		}
		triplets.emplace_back(CountEdges(), 0, Complex(1, 0));
		L.setFromTriplets(triplets.begin(), triplets.end());
		//		{
		//			std::cout << "Matrix L:\n" << L << std::endl;
		//			Eigen::MatrixXcd Ld = L.toDense();
		//			std::cout << "L=[";
		//			for (int i = 0; i < Ld.rows(); i++) {
		//				for (int j = 0; j < Ld.cols(); j++) {
		//					std::cout << Ld(i, j).real() << "+" << Ld(i, j).imag()
		//							<< "j";
		//					if (j + 1 < Ld.cols())
		//						std::cout << ",";
		//				}
		//				if (i + 1 < Ld.rows())
		//					std::cout << ";";
		//			}
		//			std::cout << "];" << std::endl;
		//		}
		SpComplexMatrix AtA = L.adjoint() * L;
		SpComplexVector b(CountEdges() + 1);
		b.setZero();
		b[CountEdges()] = std::complex(1.0, 0.0);
		Eigen::SimplicialLLT<SpComplexMatrix> solver;
		Eigen::SparseMatrix<std::complex<double> > Lreg = L.adjoint() * L;
		//		{
		//			Eigen::MatrixXcd Ld = Lreg.toDense();
		//			std::cout << "Lreg=[";
		//			for (int i = 0; i < Ld.rows(); i++) {
		//				for (int j = 0; j < Ld.cols(); j++) {
		//					std::cout << Ld(i, j).real() << "+" << Ld(i, j).imag()
		//							<< "j";
		//					if (j + 1 < Ld.cols())
		//						std::cout << ",";
		//				}
		//				if (i + 1 < Ld.rows())
		//					std::cout << ";";
		//			}
		//			std::cout << "];" << std::endl;
		//		}
		solver.compute(Lreg);
		if (solver.info() != Eigen::Success) {
			std::cerr << "Decomposition for initial angle alignment failed!"
					<< std::endl;
		}
		Eigen::VectorXcd breg = L.adjoint() * b;
		SpComplexVector x = solver.solve(breg);
		if (solver.info() != Eigen::Success) {
			std::cerr << "Solving for initial angle alignment failed!"
					<< std::endl;
		}
		for (size_t tidx = 0; tidx < CountTriangles(); tidx++)
			t2ds[tidx].L = x[tidx] / std::abs(x[tidx]);
	}
}

void ARAP::UpdateLocalRotations(const double sigma) {
	// Calculation the rotation matrices L.
	typedef std::complex<double> Complex;
	for (size_t tidx = 0; tidx < CountTriangles(); tidx++) {
		std::array<double, 4> S = { 0, 0, 0, 0 };
		Triangle2D &t2d = t2ds[tidx];
		for (int_fast8_t idx = 0; idx < 3; idx++) {
			const Vertex &u0 = GetTriangleVertex(tidx, (idx + 0) % 3);
			const Vertex &u1 = GetTriangleVertex(tidx, (idx + 1) % 3);
			const double f = t2d.omega[idx];
			const double ux = u1.x - u0.x;
			const double uy = u1.y - u0.y;
			const Complex xx = t2d.getEdgeVector(idx);
			S[0] += f * (ux * xx.real());
			S[1] += f * (uy * xx.real());
			S[2] += f * (ux * xx.imag());
			S[3] += f * (uy * xx.imag());
		}
		t2d.L = SolveProcrustes(S, sigma);
	}
}

inline std::complex<double> ARAP::SolveProcrustes(
		const std::array<double, 4> &S, double sigma) {
	const double a = S[0];
	const double b = S[1];
	const double c = S[2];
	const double d = S[3];

	// Mean singular value
	const double t = a * a + b * b + c * c + d * d;
	const double det = a * d - b * c;
	const double eigenvaluessum = std::sqrt(t + 2.0 * std::abs(det));
	// Interpolation
	const double neweigenvalues = sigma + (1.0 - sigma) * eigenvaluessum / 2.0;

	// R = (A*C) * neweigenvalues
	const double x = a + d;
	const double y = b - c;
	const double n2 = x * x + y * y;
	if (n2 < DBL_EPSILON) {
		return {neweigenvalues, 0.0};
	}
	const double inv = neweigenvalues / std::sqrt(n2);
	const double r0 = x * inv;
	const double r1 = y * inv;
	return {r0, r1};
}

double ARAP::VectorCotangens(double x0, double y0, double x1, double y1) {
	const double num = x0 * x1 + y0 * y1;
	const double den = -x0 * y1 + y0 * x1;
	if (std::fabs(den) < FLT_EPSILON) {
		if (std::fabs(num) < FLT_EPSILON)
			return std::numeric_limits<double>::quiet_NaN();
		return std::copysign(std::numeric_limits<double>::infinity(), den);
	} else {
		return num / den;
	}
}

ARAP::Color ARAP::IndexToColor(size_t idx) {
	double a = (double) idx * 2.0;
	Color c;
	c.r = cos(a) * 0.5 + 0.5;
	c.g = cos(a + M_PI / 3.0 * 2.0) * 0.5 + 0.5;
	c.b = cos(a - M_PI / 3.0 * 2.0) * 0.5 + 0.5;
	return c;
}

#ifdef DEBUG
void ARAP::Print2x2(const std::string &name,
		const std::array<double, 4> &matrix) {
	std::cout << name << " = reshape([";
	bool first = true;
	for (double s : matrix) {
		if (!first)
			std::cout << ",";
		first = false;
		std::cout << s;
	}
	std::cout << "],2,2)\n";
}
#endif
