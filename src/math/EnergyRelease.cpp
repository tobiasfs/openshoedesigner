///////////////////////////////////////////////////////////////////////////////
// Name               : EnergyRelease.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : -std=c++17 or greater
// Author             : Tobias Schaefer
// Created            : 21.04.2025
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

#include "EnergyRelease.h"

#include "PCA.h"
#include "Matrix.h"
#include "Exporter.h"

#include <iostream>

static const size_t nothing = (size_t) -1;

EnergyRelease::EnergyRelease(Geometry &geo) {
	Calculate(geo);
}

void EnergyRelease::InitByPCA(Geometry &geo) {
	PCA pca;
	const Vector3 center = geo.GetCenterOfVertices();
	pca.SetCenter(center);
	for (size_t v = 0; v < geo.CountVertices(); v++)
		pca.Add(geo.GetVertex(v));
	pca.Calculate();
	AffineTransformMatrix m = pca.GetMatrix();
	for (size_t idx = 0; idx < geo.CountVertices(); idx++) {
		Geometry::Vertex &v = geo.GetVertex(idx);
		v.u = m.LocalX(v) * 1.0;
		v.v = m.LocalY(v) * 1.0;
	}
	geo.FlagUV(true, false);
}

void EnergyRelease::InitByUniformDimension(Geometry &geo,
		const Vector3 &uniformDimension) {
	AffineTransformMatrix m;
	m.SetEz(uniformDimension);
	m.SetEy(uniformDimension.Orthogonal());
	m.CalculateEx();
	geo.Fix();

	auto dist = [](double a, double b) {
		return sqrt(a * a + b * b);
	};
	std::set<size_t> fixed = { 0 };
	bool updated = true;
	while (updated) {
		updated = false;
		for (size_t idx = 0; idx < geo.CountEdges(); idx++) {
			const Geometry::Edge &ed = geo.GetEdge(idx);
			const size_t vidx0 = ed.GetVertexIndex(0);
			const size_t vidx1 = ed.GetVertexIndex(1);
			const bool found0 = fixed.find(vidx0) != fixed.end();
			const bool found1 = fixed.find(vidx1) != fixed.end();
			if (!found0 && !found1)
				continue;
			Geometry::Vertex &v0 = geo.GetVertex(vidx0);
			Geometry::Vertex &v1 = geo.GetVertex(vidx1);
			const Vector3 r0 = m.Transform(geo.GetEdgeVertex(idx, 0));
			const Vector3 r1 = m.Transform(geo.GetEdgeVertex(idx, 1));
			v0.v = r0.z;
			v1.v = r1.z;
			const double d = dist(r0.x - r1.x, r0.y - r1.y);
			if (found0 && !found1) {
				v1.u = v0.u + d;
				fixed.insert(vidx1);
				updated = true;
				continue;
			}
			if (!found0 && found1) {
				v0.u = v1.u + d;
				fixed.insert(vidx0);
				updated = true;
				continue;
			}
			if (v0.u < v1.u) {
				if (v0.u + d + FLT_EPSILON < v1.u) {
					v1.u = v0.u + d;
					updated = true;
					continue;
				}
			}
			if (v0.u > v1.u) {
				if (v1.u + d + FLT_EPSILON < v0.u) {
					v0.u = v1.u + d;
					updated = true;
					continue;
				}
			}
		}
	}
	geo.FlagUV(true, false);
}

void EnergyRelease::InitByConstruction(Geometry &geo) {
	throw std::runtime_error("Not implemented.");
}

void EnergyRelease::Calculate(Geometry &geo) {
	geo.CalculateNormals();

	// Properties for all vertices and edges
	std::vector<double> vMass(geo.CountVertices(), 0.0);
	std::vector<double> vDamp(geo.CountVertices(), 0.0);

	std::vector<double> eSpring(geo.CountEdges(), 0.0);
	std::vector<double> eLength0(geo.CountEdges(), 0.0);

	std::vector<double> tSpring(geo.CountTriangles(), 0.0);
	std::vector<double> tMass(geo.CountTriangles(), 0.0);
	std::vector<double> tArea0(geo.CountTriangles(), 0.0);
	std::vector<double> tHeight0(geo.CountTriangles() * 3, 0.0);
	std::vector<double> tAngle0(geo.CountTriangles() * 3, 0.0);

	double tAreaMin = DBL_MAX;

	for (size_t eidx = 0; eidx < geo.CountEdges(); eidx++) {
		// Calculate all edge length in 3D space.
		const Geometry::Edge &ed = geo.GetEdge(eidx);
		const auto &v0 = geo.GetEdgeVertex(eidx, 0);
		const auto &v1 = geo.GetEdgeVertex(eidx, 1);
		eLength0[eidx] = (v1 - v0).Abs();
	}
	for (size_t tidx = 0; tidx < geo.CountTriangles(); tidx++) {
		// Calculate the triangle area.
		tArea0[tidx] = geo.GetTriangleArea(tidx);

		// Calculate the mass of the triangles.
		const auto &tri = geo.GetTriangle(tidx);
		// Using the longest edge of the triangle, a minimal weight for
		// triangles is calculated.
		const double eLengthMax = std::fmax(
				std::fmax(eLength0[tri.ea], eLength0[tri.eb]),
				eLength0[tri.ec]);
		const double areaLimit = eLengthMax * eLengthMax * 0.25;
		const double area = std::fmax(tArea0[tidx], areaLimit);
		tAreaMin = std::fmin(tAreaMin, area);
		const double mass = area * rho;
		tMass[tidx] = mass;
		vMass[tri.va] += mass / 3.0;
		vMass[tri.vb] += mass / 3.0;
		vMass[tri.vc] += mass / 3.0;
	}
	// Use the masses to calculate the spring strength
	for (size_t eidx = 0; eidx < geo.CountEdges(); eidx++) {
		const Geometry::Edge &ed = geo.GetEdge(eidx);
		const double minMass = std::fmin(vMass[ed.va], vMass[ed.vb]);

		// This is more or less from the Provot1995 paper for the limit of
		// the simulation dt of mass-spring grids. This answers the question:
		// "How stiff can the mesh be given the masses and the simulation-time
		// constant?" (Yet a twiddle factor is needed.)
		const double cSpring = minMass * (M_PI * M_PI) / (dt * dt) / 300.0;
		eSpring[eidx] = accel * cSpring;
		if (ed.trianglecount == 1)
			tSpring[ed.ta] += accel2 * cSpring / 6.0;
		if (ed.trianglecount >= 2) {
			tSpring[ed.ta] += accel2 * cSpring / 12.0;
			tSpring[ed.tb] += accel2 * cSpring / 12.0;
		}
	}

	// For the penalty function collect the distances in each triangle of a
	// vertex to the opposite edge. This is used to detect collapsing
	// triangles.
	// The height is the distance of a triangle-vertex above the opposite edge
	// in 3D space.
	for (size_t tidx = 0; tidx < geo.CountTriangles(); tidx++) {
		const auto &tri = geo.GetTriangle(tidx);
		for (uint_fast8_t vidx = 0; vidx < 3; vidx++) {
			const auto &vert = geo.GetTriangleVertex(tidx, vidx);
			const size_t eidx = tri.GetEdgeIndex((vidx + 1) % 3);
			const auto &va = geo.GetEdgeVertex(eidx, 0);
			const auto &vb = geo.GetEdgeVertex(eidx, 1);
			const Vector3 dir = (vb - va).Normal();
			const double k = dir.Dot(vert - va);
			const Vector3 v0 = va + k * dir;
			tHeight0[tidx * 3 + vidx] = (vert - v0).Abs();
		}

		const Geometry::Vertex &va = geo.GetTriangleVertex(tidx, 0);
		const Geometry::Vertex &vb = geo.GetTriangleVertex(tidx, 1);
		const Geometry::Vertex &vc = geo.GetTriangleVertex(tidx, 2);
		// Note that the sum of all three angles is always 2*M_PI, because the
		// directional changes are measured.
		tAngle0[tidx * 3 + 0] = CalculateAngle3D(vc, va, vb);
		tAngle0[tidx * 3 + 1] = CalculateAngle3D(va, vb, vc);
		tAngle0[tidx * 3 + 2] = CalculateAngle3D(vb, vc, va);
	}
	{
		// Sum up all spring constants acting on a vertex to determine the
		// damping factor for this vertex.
		std::vector<double> vSpring(geo.CountVertices(), 0.0);
		for (size_t eidx = 0; eidx < geo.CountEdges(); eidx++) {
			const Geometry::Edge &ed = geo.GetEdge(eidx);
			vSpring[ed.va] += eSpring[eidx] / 2.0;
			vSpring[ed.vb] += eSpring[eidx] / 2.0;
		}

		//TODO Add triangle springs.

		for (size_t vidx = 0; vidx < geo.CountVertices(); vidx++) {
			vDamp[vidx] = delta / 8.0 * std::sqrt(vSpring[vidx] * vMass[vidx]);
		}
	}

#ifdef DEBUG
	// Storing triangles and heights for debugging
	{
		Matrix triangles("triangles", { geo.CountTriangles(), 9 },
				Matrix::Order::TWO_REVERSED);
		for (size_t tidx = 0; tidx < geo.CountTriangles(); tidx++) {
			const auto &tri = geo.GetTriangle(tidx);
			for (uint8_t vidx = 0; vidx < 3; vidx++) {
				const auto &vert = geo.GetTriangleVertex(tidx, vidx);
				triangles.Insert(vert.x);
				triangles.Insert(vert.y);
				triangles.Insert(vert.z);
			}
		}
		triangles.ReorderDimensions();

		Matrix heights("heights", { geo.CountTriangles(), 3 },
				Matrix::Order::TWO_REVERSED);
		heights.Insert(tHeight0);
		heights.ReorderDimensions();

		Exporter ex("/tmp/tri.mat");
		ex.WriteMatrix(triangles);
		ex.WriteMatrix(heights);
	}
#endif

	// For the simulation in 2D space:
	std::vector<Vector3> F(geo.CountVertices()); // Force
	std::vector<Vector3> ddq(geo.CountVertices()); // Acceleration
	std::vector<Vector3> dq(geo.CountVertices()); // Speed
	std::vector<Vector3> q(geo.CountVertices()); // Position

	// Initialize positions
	for (size_t n = 0; n < geo.CountVertices(); n++) {
		q[n].x = geo[n].u;
		q[n].y = geo[n].v;
	}

	// Values for the stop conditions
	double ES = DBL_MAX;
	double EC = DBL_MAX;
	double Ephi = DBL_MAX;
	double dEphi = DBL_MAX;

	std::vector<double> tArea(geo.CountTriangles(), 0.0);
	std::vector<double> tAngle(geo.CountTriangles() * 3, 0.0);

	std::vector<double> eLength(geo.CountEdges(), 0.0);

	std::vector<double> applyForce(geo.CountEdges());

	// Calculate the current area and the direction changes of all triangles
	for (size_t tidx = 0; tidx < geo.CountTriangles(); tidx++) {
		Geometry::Triangle &tri = geo.GetTriangle(tidx);
		const size_t vaidx = tri.GetVertexIndex(0);
		const size_t vbidx = tri.GetVertexIndex(1);
		const size_t vcidx = tri.GetVertexIndex(2);
		tArea[tidx] = CalculateTriangleArea(q[vaidx].x, q[vaidx].y, q[vbidx].x,
				q[vbidx].y, q[vcidx].x, q[vcidx].y);
		tAngle[tidx * 3 + 0] = FlipCompensate(
				CalculateAngle2D(q[vcidx], q[vaidx], q[vbidx]),
				tAngle0[tidx * 3 + 0]);
		tAngle[tidx * 3 + 1] = FlipCompensate(
				CalculateAngle2D(q[vaidx], q[vbidx], q[vcidx]),
				tAngle0[tidx * 3 + 1]);
		tAngle[tidx * 3 + 2] = FlipCompensate(
				CalculateAngle2D(q[vbidx], q[vcidx], q[vaidx]),
				tAngle0[tidx * 3 + 2]);
	}
	// Calculate the length of all edges
	for (size_t eidx = 0; eidx < geo.CountEdges(); eidx++) {
		auto &ed = geo.GetEdge(eidx);
		Vector3 dir = q[ed.vb] - q[ed.va];
		const double d = dir.Abs();
		eLength[eidx] = d;
	}

	size_t iter = 0;
	while ((ES > sigmaMax || EC > sigmaMax) && dEphi > epsMax && iter < Nmax) {
		iter++;

		if (iter + 1 == Nmax) {
			debug.Clear();
//			debug.paintNormals = true;
			debug.SetAddColor(0.7, 0.5, 0.8, 1.0);
			for (size_t idx = 0; idx < geo.CountVertices(); idx++)
				geo.GetVertex(idx).n.Zero();
		}

		// Init tensile forces on vertices
		F.assign(geo.CountVertices(), { 0, 0, 0 });
		applyForce.assign(geo.CountEdges(), 1.0); // Flag for switching off edge springs

		// Springs in edges, "structural springs" (Provot1995)
		for (size_t eidx = 0; eidx < geo.CountEdges(); eidx++) {
			auto &ed = geo.GetEdge(eidx);
			Vector3 dir = q[ed.vb] - q[ed.va];
			const double d = dir.Abs();
			eLength[eidx] = d;
			const double dLength = d - eLength0[eidx];
			const double Fs = dLength * eSpring[eidx];
			if (fabs(d) > FLT_EPSILON) {
				dir /= d;
				F[ed.va] += dir * Fs;
				F[ed.vb] -= dir * Fs;
			}
		}

		// Springs for the height of a triangle, "shear springs" (Provot1995)
		for (size_t tidx = 0; tidx < geo.CountTriangles(); tidx++) {
			//			if (tArea[tidx] >= 0.0)
			//				continue;
			const Geometry::Triangle &tri = geo.GetTriangle(tidx);
			const size_t eidxa = tri.GetEdgeIndex(0);
			const size_t eidxb = tri.GetEdgeIndex(1);
			const size_t eidxc = tri.GetEdgeIndex(2);
			const Geometry::Edge &ea = geo.GetEdge(eidxa);
			const Geometry::Edge &eb = geo.GetEdge(eidxb);
			const Geometry::Edge &ec = geo.GetEdge(eidxc);
			const size_t vidxa = tri.GetVertexIndex(0);
			const size_t vidxb = tri.GetVertexIndex(1);
			const size_t vidxc = tri.GetVertexIndex(2);

			const int_fast8_t state = (tArea[tidx] < 0.0) ? -1 : 1;

			// Check status of neighboring triangles.
			const int_fast8_t stateA =
					(ea.GetOtherTriangle(tidx) == nothing) ?
							0 :
							((tArea[ea.GetOtherTriangle(tidx)] < 0.0) ? -1 : 1);
			const int_fast8_t stateB =
					(eb.GetOtherTriangle(tidx) == nothing) ?
							0 :
							((tArea[eb.GetOtherTriangle(tidx)] < 0.0) ? -1 : 1);
			const int_fast8_t stateC =
					(ec.GetOtherTriangle(tidx) == nothing) ?
							0 :
							((tArea[ec.GetOtherTriangle(tidx)] < 0.0) ? -1 : 1);

			Vector3 dirA = q[vidxb] - q[vidxa];
			Vector3 dirB = q[vidxc] - q[vidxb];
			Vector3 dirC = q[vidxa] - q[vidxc];
			const double lengthA = dirA.Abs();
			const double lengthB = dirB.Abs();
			const double lengthC = dirC.Abs();
			dirA.Normalize();
			dirB.Normalize();
			dirC.Normalize();
			Vector3 nA(-dirB.y, dirB.x);
			Vector3 nB(-dirC.y, dirC.x);
			Vector3 nC(-dirA.y, dirA.x);
			nA.Normalize();
			nB.Normalize();
			nC.Normalize();

			const double aMean = (tAngle[tidx * 3 + 0] + tAngle[tidx * 3 + 1]
					+ tAngle[tidx * 3 + 2]) / 3.0;
			const double a0 = tAngle[tidx * 3 + 0] - tAngle0[tidx * 3 + 0];
			const double a1 = tAngle[tidx * 3 + 1] - tAngle0[tidx * 3 + 1];
			const double a2 = tAngle[tidx * 3 + 2] - tAngle0[tidx * 3 + 2];

//			if (tidx != 0)
//				continue;

//			if (tAngle[tidx * 3 + 0] > aMean)
			{
				F[vidxc] -= nB * a0 * accel3;
				F[vidxb] -= nC * a0 * accel3;
			}
//			if (tAngle[tidx * 3 + 1] > aMean)
			{
				F[vidxa] -= nC * a1 * accel3;
				F[vidxc] -= nA * a1 * accel3;
			}
//			if (tAngle[tidx * 3 + 2] > aMean)
			{
				F[vidxb] -= nA * a2 * accel3;
				F[vidxa] -= nB * a2 * accel3;
			}

			double hA = nA.Dot(q[vidxa] - q[vidxb]);
			double hB = nB.Dot(q[vidxb] - q[vidxc]);
			double hC = nC.Dot(q[vidxc] - q[vidxa]);

			if (iter + 1 == Nmax) {
				Geometry::Vertex v0 = (q[vidxb] + q[vidxc]) / 2.0;
				Geometry::Vertex v1 = v0 + nA * hA;
				v0 += nA * tHeight0[tidx * 3 + 0];
//				debug.SetAddNormal(-nA * hA);
				debug.SetAddColor(0.7, 0.7, 0.7, 1.0);
				debug.AddEdge(v0, v1);
				debug.SetAddColor(0.7, 0.5, 0.8, 1.0);
				debug.AddEdge(v1, q[vidxa]);
			}
			if (iter + 1 == Nmax) {
				Geometry::Vertex v0 = (q[vidxc] + q[vidxa]) / 2.0;
				Geometry::Vertex v1 = v0 + nB * hB;
				v0 += nB * tHeight0[tidx * 3 + 1];
//				debug.SetAddNormal(-nB * hB);
				debug.SetAddColor(0.7, 0.7, 0.7, 1.0);
				debug.AddEdge(v0, v1);
				debug.SetAddColor(0.7, 0.5, 0.8, 1.0);
				debug.AddEdge(v1, q[vidxb]);
			}
			if (iter + 1 == Nmax) {
				Geometry::Vertex v0 = (q[vidxa] + q[vidxb]) / 2.0;
				Geometry::Vertex v1 = v0 + nC * hC;
				v0 += nC * tHeight0[tidx * 3 + 2];
//				debug.SetAddNormal(-nC * hC);
				debug.SetAddColor(0.7, 0.7, 0.7, 1.0);
				debug.AddEdge(v0, v1);
				debug.SetAddColor(0.7, 0.5, 0.8, 1.0);
				debug.AddEdge(v1, q[vidxc]);
			}

			hA -= tHeight0[tidx * 3 + 0];
			hB -= tHeight0[tidx * 3 + 1];
			hC -= tHeight0[tidx * 3 + 2];

			//
//			if (stateA == 1)
			{
				F[vidxa] -= nA * hA * tSpring[tidx];
				F[vidxb] += nA * hA * tSpring[tidx] / 2.0;
				F[vidxc] += nA * hA * tSpring[tidx] / 2.0;
			}
//			if (stateB == 1)
			{
				F[vidxb] -= nB * hB * tSpring[tidx];
				F[vidxc] += nB * hB * tSpring[tidx] / 2.0;
				F[vidxa] += nB * hB * tSpring[tidx] / 2.0;
			}
//			if (stateC == 1)
			{
				F[vidxc] -= nC * hC * tSpring[tidx];
				F[vidxa] += nC * hC * tSpring[tidx] / 2.0;
				F[vidxb] += nC * hC * tSpring[tidx] / 2.0;
			}
		}

		/*

		 // Keep triangles un-flipped
		 Matrix storage("tri", Matrix::Order::TWO_REVERSED);

		 for (size_t tidx = 0; tidx < geo.TriangleCount(); tidx++) {
		 // For each triangle check, if the current motion would drive
		 // the vertex across the opposite edge.
		 auto &tri = geo.GetTriangle(tidx);

		 // Calculate the distances from all edges
		 // (also color the triangle red, if it is currently flipped.)
		 //			std::array<double, 3> h;
		 //			std::array<double, 3> dh;
		 //			std::array<Vector3, 3> n;

		 for (uint_fast8_t tvidx = 0; tvidx < 3; tvidx++) {
		 const size_t vidx = tri.VertexIndex(tvidx);
		 const size_t eaidx = tri.VertexIndex((tvidx + 1) % 3);
		 const size_t ebidx = tri.VertexIndex((tvidx + 2) % 3);

		 Geometry::Vertex &vert = geo.GetVertex(vidx);

		 Geometry::Vertex va = geo.GetVertex(eaidx);
		 Geometry::Vertex vb = geo.GetVertex(ebidx);

		 Vector3 dir0(vb.u - va.u, vb.v - va.v);
		 dir0.Normalize();
		 Vector3 n0 = { -dir0.y, dir0.x, 0 };

		 //				storage.push_back(q[vidx].x);
		 //				storage.push_back(q[vidx].y);
		 //
		 //				storage.push_back(dq[vidx].x);
		 //				storage.push_back(dq[vidx].y);
		 //
		 //				storage.push_back(q[vidx].x + dq[vidx].x * dt);
		 //				storage.push_back(q[vidx].y + dq[vidx].y * dt);

		 double h01 = n0.x * (q[vidx].x + dq[vidx].x * dt - va.u)
		 + n0.y * (q[vidx].y + dq[vidx].y * dt - va.v);
		 double h00 = n0.x * (q[vidx].x - va.u)
		 + n0.y * (q[vidx].y - va.v);

		 // Move the edge to the position after the current timestep.
		 va.u += dq[eaidx].x * dt;
		 va.v += dq[eaidx].y * dt;
		 vb.u += dq[ebidx].x * dt;
		 vb.v += dq[ebidx].y * dt;

		 // Direction and normal of the opposing edge
		 Vector3 dir(vb.u - va.u, vb.v - va.v);
		 dir.Normalize();
		 Vector3 n = { -dir.y, dir.x, 0 };

		 // Calculate the distance of the opposing vertex before and
		 // after moving.

		 double h11 = n.x * (vert.u + dq[vidx].x * dt - va.u)
		 + n.y * (vert.v + dq[vidx].y * dt - va.v);
		 double h10 = n.x * (vert.u - va.u) + n.y * (vert.v - va.v);

		 if (h11 > 0.0) {
		 continue;
		 }

		 std::cout << tidx << '[' << (int) tvidx << ']' << " / " << vidx
		 << '\n';
		 const bool alreadyCollapsed = h00 < 0.0;
		 const double tcoll = TCollapse(q[eaidx].x, q[eaidx].y,
		 q[ebidx].x, q[ebidx].y, q[vidx].x, q[vidx].y,
		 dq[eaidx].x, dq[eaidx].y, dq[ebidx].x, dq[ebidx].y,
		 dq[vidx].x, dq[vidx].y, alreadyCollapsed);

		 const double tcollcorr = tcoll - FLT_EPSILON;

		 //				std::cout << "TCollapse = " << tcoll << "\n";

		 double commu = dq[vidx].x;
		 double commv = dq[vidx].y;
		 commu += dq[eaidx].x;
		 commv += dq[eaidx].y;
		 commu += dq[ebidx].x;
		 commv += dq[ebidx].y;
		 commu /= 3.0;
		 commv /= 3.0;

		 double da = xInnerl;

		 dq[vidx].x = dq[vidx].x * tcollcorr
		 + commu * (dt - tcollcorr) * da;
		 dq[vidx].y = dq[vidx].y * tcollcorr
		 + commv * (dt - tcollcorr) * da;
		 dq[eaidx].x = dq[eaidx].x * tcollcorr
		 + commu * (dt - tcollcorr) * da;
		 dq[eaidx].y = dq[eaidx].y * tcollcorr
		 + commv * (dt - tcollcorr) * da;
		 dq[ebidx].x = dq[ebidx].x * tcollcorr
		 + commu * (dt - tcollcorr) * da;
		 dq[ebidx].y = dq[ebidx].y * tcollcorr
		 + commv * (dt - tcollcorr) * da;
		 break;
		 //				std::cout << h00 << " \t " << h01 << " \t " << h10 << " \t "
		 //						<< h11 << '\n';
		 if (iter + 1 == Nmax) {
		 debug.Clear();
		 debug.paintNormals = true;
		 debug.SetAddColor(0.7, 0.5, 0.8, 1.0);
		 for (size_t idx = 0; idx < geo.CountVertices(); idx++)
		 geo.GetVertex(idx).n.Zero();
		 }
		 //				dh[tvidx] = h1 - hstar[tidx * 3 + tvidx];
		 //				ddq[vidx] -= n[tvidx]
		 //						* (dh[tvidx] * kInner / masses[vidx] * xInnerl);
		 }

		 // Inner spring constant
		 //			double kInner = 0.0;
		 //			for (uint_fast8_t tvidx = 0; tvidx < 3; tvidx++) {
		 //				const size_t eidx = tri.EdgeIndex(tvidx);
		 //				kInner += 1.0 / springs[eidx];
		 //			}
		 //			kInner = 3.0 / kInner;
		 }
		 // Add penalty to position

		 //		for (size_t tidx = 0; tidx < geo.TriangleCount(); tidx++) {
		 //			auto &tri = geo.GetTriangle(tidx);

		 // All three edges have to be investigated to find out what edge
		 // the triangle is actually fipped over.

		 //			std::array<double, 3> h;
		 //			std::array<double, 3> hrel;
		 //			std::array<Vector3, 3> n;
		 //			for (uint8_t tvidx = 0; tvidx < 3; tvidx++) {
		 //				const auto &vert = geo.GetTriangleVertex(tidx, tvidx);
		 //				const auto &va = geo.GetTriangleVertex(tidx, (tvidx + 1) % 3);
		 //				const auto &vb = geo.GetTriangleVertex(tidx, (tvidx + 2) % 3);
		 //
		 //				Vector3 dir(vb.u - va.u, vb.v - va.v);
		 //				dir.Normalize();
		 //				n[tvidx] = { -dir.y, dir.x, dir.z };
		 //
		 //				h[tvidx] = n[tvidx].x * (vert.u - va.u)
		 //						+ n[tvidx].y * (vert.v - va.v);
		 //				hrel[tvidx] = h[tvidx] / hstar[tidx * 3 + tvidx];
		 //				hcurr[tidx * 3 + tvidx] = h[tvidx];
		 //			}

		 //			tri.c = { 0, 1, 0, 1 };
		 //			if (hrel[0] < 0.0 || hrel[1] < 0.0 || hrel[2] < 0.0) {
		 //				tri.c = { 1, 0, 0, 1 };
		 // Solve a flipped triangle as fast as possible by adding the
		 // penalty only to the smallest relative error.

		 //				Vector3 Fp;
		 //				const double hmed = (h[0] + h[1] + h[2]) / 3.0;
		 //				if (hrel[0] < 0.0 && h[0] > hmed) {
		 //					const size_t vidx = tri.VertexIndex(0);
		 //					Fp = -n[0] * ((h[0] - hstar[tidx * 3 + 0]) * ksum[vidx]);
		 //					geo.GetVertex(vidx).n += Fp * visPenalty;
		 //					ddq[vidx] += Fp * xPenalty;
		 //				}
		 //				if (hrel[1] < 0.0 && h[1] > hmed) {
		 //					const size_t vidx = tri.VertexIndex(1);
		 //					Fp = -n[1] * (h[1] - hstar[tidx * 3 + 1]) * ksum[vidx];
		 //					geo.GetVertex(vidx).n += Fp * visPenalty;
		 //					ddq[vidx] += Fp * xPenalty;
		 //				}
		 //				if (hrel[2] < 0.0 && h[2] > hmed) {
		 //					const size_t vidx = tri.VertexIndex(2);
		 //					Fp = -n[2] * (h[2] - hstar[tidx * 3 + 2]) * ksum[vidx];
		 //					geo.GetVertex(vidx).n += Fp * visPenalty;
		 //					ddq[vidx] += Fp * xPenalty;
		 //				}
		 //			}

		 //			ed.n = visNormal * n;
		 //
		 //			if (hx < hstar[tidx * 3 + tvidx]) {
		 //				// Correction distance (> 0)
		 //				const double d = hstar[tidx * 3 + tvidx] - hx;
		 //				const Vector3 Fp = n * d;
		 //
		 ////				geo.GetVertex(vidx).n += n
		 ////						* ((hx - hstar[tidx * 3 + tvidx])
		 ////								* debugMIDI_51->ToDouble() / 30.0);
		 //			}

		 //		}

		 {

		 //			storage.Reshape(-1, 3 * 6);
		 //			storage.ReorderDimensions();
		 //			Exporter ex("/tmp/tri.mat");
		 //			ex.WriteMatrix(storage);
		 }
		 */

		// Calculate integrals
		for (size_t idx = 0; idx < geo.CountVertices(); idx++) {
			ddq[idx] = F[idx] / vMass[idx];
			dq[idx] += ddq[idx] * dt;
			dq[idx] -= dq[idx] * vDamp[idx];
//			F[idx] -= dq[idx] * vDamp[idx];

			q[idx] += dq[idx] * dt + 0.5 * ddq[idx] * dt * dt;

			Geometry::Vertex &vert = geo.GetVertex(idx);
			vert.u = q[idx].x;
			vert.v = q[idx].y;

			// Adding debug-info.
			vert.n = F[idx] * visForce;
		}

		double sumArea = 0.0;
		double sumDeltaArea = 0.0;
		double sumLength = 0.0;
		double sumDeltaLength = 0.0;
		const double Ephi0 = Ephi;
		Ephi = 0.0;

		// Calculate the current tArea of all triangles
		// (for checking if flipped -> tArea < 0.0)
		for (size_t tidx = 0; tidx < geo.CountTriangles(); tidx++) {
			Geometry::Triangle &tri = geo.GetTriangle(tidx);
			const size_t vaidx = tri.GetVertexIndex(0);
			const size_t vbidx = tri.GetVertexIndex(1);
			const size_t vcidx = tri.GetVertexIndex(2);
			const double a = CalculateTriangleArea(q[vaidx].x, q[vaidx].y,
					q[vbidx].x, q[vbidx].y, q[vcidx].x, q[vcidx].y);
			tArea[tidx] = a;
			sumArea += tArea0[tidx];
			sumDeltaArea += std::fabs(a - tArea0[tidx]);

			// Color the flipped triangles red for debugging purposes.
			if (a < 0.0)
				tri.c = { 1, 0, 0, 1 }; // red
			else
				tri.c = { 0, 1, 0, 1 }; // green

			tAngle[tidx * 3 + 0] = FlipCompensate(
					CalculateAngle2D(q[vcidx], q[vaidx], q[vbidx]),
					tAngle0[tidx * 3 + 0]);
			tAngle[tidx * 3 + 1] = FlipCompensate(
					CalculateAngle2D(q[vaidx], q[vbidx], q[vcidx]),
					tAngle0[tidx * 3 + 1]);
			tAngle[tidx * 3 + 2] = FlipCompensate(
					CalculateAngle2D(q[vbidx], q[vcidx], q[vaidx]),
					tAngle0[tidx * 3 + 2]);
		}
		// Calculate edge lengths
		for (size_t eidx = 0; eidx < geo.CountEdges(); eidx++) {
			auto &ed = geo.GetEdge(eidx);
			Vector3 dir = q[ed.vb] - q[ed.va];
			const double d = dir.Abs();
			eLength[eidx] = d;
			const double dLength = d - eLength0[eidx];
			const double SpringEnergy = 0.5 * eSpring[eidx] * dLength * dLength;
			Ephi += SpringEnergy;

			sumLength += eLength0[eidx];
			sumDeltaLength += fabs(dLength);

			// Color edges
			if (dLength > 0.0) {
				ed.c.r = 0.0;
				ed.c.g = fmin(fmax(1e4 * dLength, 0), 255);
				ed.c.b = 0.0;
			} else {
				ed.c.r = fmin(fmax(-1e4 * dLength, 0), 255);
				ed.c.g = 0.0;
				ed.c.b = 0.0;
			}
		}
//		ES = sumDeltaArea / sumArea;
//		EC = sumDeltaLength / sumLength;
//		dEphi = (Ephi0 - Ephi) / dt;

	}
#ifdef DEBUG
		{

//			Matrix normals("normals", geo.CountTriangles(), 3);
//			for (size_t idx = 0; idx < geo.CountTriangles(); idx++) {
//				const auto &tri = geo.GetTriangle(idx);
//				normals.Insert(tri.n.x, idx, 0);
//				normals.Insert(tri.n.y, idx, 1);
//				normals.Insert(tri.n.z, idx, 2);
//			}
//
//			Exporter ex("/tmp/energyrelease.mat");
//
//			ex.WriteMatrix(normals);
//			ex.Add(vMass, "vMass");
//			ex.Add(tHeight0, "hstar");
//			ex.Add(hcurr, "h");

		}
	#endif

}

double EnergyRelease::CalculateTriangleArea(double Au, double Av, double Bu,
		double Bv, double Cu, double Cv) {
	return ((Au - Cu) * (Bv - Cv) - (Bu - Cu) * (Av - Cv)) / 2.0;
}

double EnergyRelease::CalculateAngle3D(const Geometry::Vertex &va,
		const Geometry::Vertex &vb, const Geometry::Vertex &vc) {
	const Vector3 da = (vb - va).Normal();
	const Vector3 db = (vc - vb).Normal();
	return std::acos(da.Dot(db));
}

double EnergyRelease::CalculateAngle2D(const Geometry::Vertex &va,
		const Geometry::Vertex &vb, const Geometry::Vertex &vc) {
	const Vector3 da = vb - va;
	const Vector3 db = vc - vb;
	const double xr = db.x * da.x + db.y * da.y;
	const double yr = -db.x * da.y + db.y * da.x;
	return std::atan2(yr, xr);
}

double EnergyRelease::FlipCompensate(double a, double ref) {
	const double d = (a - ref) / (2.0 * M_PI);
	return a - std::round(d) * 2.0 * M_PI;
}

double EnergyRelease::TCollapse(double Au, double Av, double Bu, double Bv,
		double Cu, double Cv, double dAu, double dAv, double dBu, double dBv,
		double dCu, double dCv, bool collapseAlreadyHappened) {
	// The formulation below is independent of the order of A, B, and C.

	const double T1 = Av - Bv;
	const double T2 = Bu - Au;
	const double c2 = (dBu - dAu) * dCv + (dAv - dBv) * dCu + dAu * dBv
			- dAv * dBu;
	const double c1 = T2 * dCv + T1 * dCu + (Au - Cu) * dBv + (Cv - Av) * dBu
			+ (Cu - Bu) * dAv + (Bv - Cv) * dAu;
	const double c0 = T2 * Cv + T1 * Cu + Au * Bv - Av * Bu;

	if (fabs(c2) < FLT_EPSILON) {
		// reduced to a linear problem: solve(c1*t+c0=0, t) => t=-c0/c1
		if (fabs(c1) < FLT_EPSILON) {
			// everything moves along parallel vectors
			return 0.0;
		}
		return -c0 / c1;
	}
	// optimize(solve(c2*t^2+c1*t+c0=0,t))
	const double T3 = 1.0 / c2;
	const double T4 = c1 * c1 - 4.0 * c0 * c2;
	if (T4 < 0.0) {
		// No solution
		return 0.0;
	}
	const double T5 = sqrt(T4);
	const double t1 = -((T3 * (T5 + c1)) / 2.0);
	const double t2 = (T3 * (T5 - c1)) / 2.0;
	if (collapseAlreadyHappened) {
		if (t1 <= 0.0 && t2 <= 0.0) {
			if (t2 > t1)
				return t2;
			return t1;
		}
		if (t2 < t1)
			return t2;
		return t1;
	} else {
		if (t1 >= 0.0 && t2 >= 0.0) {
			if (t2 < t1)
				return t2;
			return t1;
		}
		if (t2 > t1)
			return t2;
		return t1;
	}
}

double EnergyRelease::FlipT(double det, double Au, double Av, double Bu,
		double Bv, double Cu, double Cv, double dAu, double dAv, double dBu,
		double dBv, double dCu, double dCv) const {

	const double T1 = 1.0 / det;
	const double T2 = -Cv;
	const double T3 = -Bu;
	const double T4 = -Av;
	const double T5 = -Cu;
	const double T6 = -Bv;
	const double T7 = -Au;
	const double T8 = Bv * Bv;
	const double T9 = Cv * Cv;
	const double T10 = 2.0 * Bu;
	const double T11 = -(2.0 * Cu);
	const double T12 = Bu * Bu;
	const double T13 = Cu * Cu;
	const double T14 = -(2.0 * Av * Bv);
	const double T15 = 2.0 * Av;
	const double T16 = 2.0 * Bv;
	const double T17 = -(4.0 * Bv);
	const double T18 = -(4.0 * Au);
	const double T19 = 2.0 * Cu;
	const double T20 = Av * Av;
	const double T21 = -(4.0 * Av);
	const double T22 = 2.0 * Au;
	const double T23 = -(4.0 * Bu);
	const double T24 = -(2.0 * Au * Bu);
	const double T25 = T10 + T22;
	const double T26 = -(2.0 * Au * Av);
	const double T27 = Au * Au;
	const double T28 = 2.0 * Av * Bv;
	const double T29 = T16 - 2.0 * Av;
	const double T30 = 2.0 * Av * Bu;
	const double T31 = T15 - 2.0 * Bv;
	const double T32 = 2.0 * Au * Av;
	const double T33 = -(4.0 * Av * Bu);
	const double T34 = T10 - 2.0 * Au;
	const double T35 = 2.0 * Au * Bu;
	const double T36 = T22 - 2.0 * Bu;
	const double T37 =
			(T12 + T24 + T27) * dCv * dCv
					+ ((T36 * Bv + T30 + T26) * dCu
							+ (T36 * Cu + T35 - 2.0 * T27) * dBv
							+ (T36 * Cv + (4.0 * Bv + T21) * Cu - 4.0 * Au * Bv
									+ T30 + T32) * dBu
							+ (T34 * Cu - 2.0 * T12 + T35) * dAv
							+ (T34 * Cv + (T17 + 4.0 * Av) * Cu + T25 * Bv + T33)
									* dAu) * dCv + (T8 + T14 + T20) * dCu * dCu
					+ (((4.0 * Bu + T18) * Cv + T31 * Cu + 2.0 * Au * Bv + T33
							+ T32) * dBv + (T31 * Cv + T28 - 2.0 * T20) * dBu
							+ ((T23 + 4.0 * Au) * Cv + T29 * Cu
									+ (T10 + T18) * Bv + T30) * dAv
							+ (T29 * Cv - 2.0 * T8 + T28) * dAu) * dCu
					+ (T13 - 2.0 * Au * Cu + T27) * dBv * dBv
					+ (((T11 + T22) * Cv + 2.0 * Av * Cu + T26) * dBu
							+ (-(2.0 * T13) + T25 * Cu + T24) * dAv
							+ ((T19 + T23 + T22) * Cv + (T16 + T21) * Cu
									- 2.0 * Au * Bv + 4.0 * Av * Bu) * dAu)
							* dBv + (T9 - 2.0 * Av * Cv + T20) * dBu * dBu
					+ (((T19 + T10 + T18) * Cv + (T17 + T15) * Cu
							+ 4.0 * Au * Bv - 2.0 * Av * Bu) * dAv
							+ (-(2.0 * T9) + (T16 + T15) * Cv + T14) * dAu)
							* dBu + (T13 - 2.0 * Bu * Cu + T12) * dAv * dAv
					+ ((T11 + T10) * Cv + 2.0 * Bv * Cu - 2.0 * Bu * Bv) * dAu
							* dAv + (T9 - 2.0 * Bv * Cv + T8) * dAu * dAu;
	if (T37 < 0.0)
		return 0.0;
	const double T38 = sqrt(T37);
	double t1 = -(T1
			* (T38 + (Bu + T7) * dCv + (T6 + Av) * dCu + (T5 + Au) * dBv
					+ (Cv + T4) * dBu + (Cu + T3) * dAv + (T2 + Bv) * dAu));
	double t2 = T1
			* (T38 + (T3 + Au) * dCv + (Bv + T4) * dCu + (Cu + T7) * dBv
					+ (T2 + Av) * dBu + (T5 + Bu) * dAv + (Cv + T6) * dAu);
	std::cout << "t1 = " << t1 << '\n';
	std::cout << "t2 = " << t2 << '\n';

	const double c1 = (t1
			* ((dAv * dBu - dAu * dBv) * t1 - Au * dBv + Av * dBu + Bu * dAv
					- Bv * dAu) - Au * Bv + Av * Bu)
			/ ((dBu - dAu) * t1 + Bu - Au);
	const double c2 = (t2
			* ((dAv * dBu - dAu * dBv) * t2 - Au * dBv + Av * dBu + Bu * dAv
					- Bv * dAu) - Au * Bv + Av * Bu)
			/ ((dBu - dAu) * t2 + Bu - Au);
	const double t3 = 0.0196431;
	const double c3 = (t3
			* ((dAv * dBu - dAu * dBv) * t3 - Au * dBv + Av * dBu + Bu * dAv
					- Bv * dAu) - Au * Bv + Av * Bu)
			/ ((dBu - dAu) * t3 + Bu - Au);

	std::cout << "c1 = " << c1 << '\n';
	std::cout << "c2 = " << c2 << '\n';
	std::cout << "c3 = " << c3 << '\n';

	if (t1 >= 0.0 && t2 >= 0.0) {
		if (t1 < t2)
			return t1;
		return t2;
	}
	if (t1 >= 0.0 && t1 <= dt)
		return t1;
	return t2;
}

double EnergyRelease::FlipDet(double dAu, double dAv, double dBu, double dBv,
		double dCu, double dCv) {
	return (2 * dBu - 2 * dAu) * dCv + (2 * dAv - 2 * dBv) * dCu + 2 * dAu * dBv
			- 2 * dAv * dBu;
}

