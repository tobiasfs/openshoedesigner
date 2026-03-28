///////////////////////////////////////////////////////////////////////////////
// Name               : DesignSolve.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 18.03.2026
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

#include "DesignSolve.h"

#include <sstream>
#include <stdexcept>
#include <iostream>

#define RUNTIME_ERROR(x) { \
	std::ostringstream err; \
	err << __FILE__ << ":" << __LINE__ << "::" << __FUNCTION__ << " - "; \
	err << x ; \
    throw std::runtime_error(err.str()); \
    }

#define LOGIC_ERROR(x) { \
	std::ostringstream err; \
	err << __FILE__ << ":" << __LINE__ << "::" << __FUNCTION__ << " - "; \
	err << x ; \
    throw std::logic_error(err.str()); \
    }

static const size_t nothing = (size_t) -1;

DesignSolve::DesignSolve() {
	out = std::make_shared<DesignSolution>();
}

std::string DesignSolve::GetName() const {
	return "DesignSolve";
}

bool DesignSolve::CanRun() {
	std::string missing;

	if (!design)
		missing = "\"design\"";
	if (!out)
		missing += missing.empty() ? "\"out\"" : ", \"out\"";

	if (!missing.empty()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " -";
		err << "The variables " << missing << " are not connected.";
		error = err.str();
		throw std::logic_error(err.str());
	}

	error.clear();

	return error.empty();
}

bool DesignSolve::Propagate() {
	if (!design || !out)
		return false;

	bool modify = false;
	if (design->IsModified()) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}
	return modify;
}

bool DesignSolve::HasToRun() {
	return out && !out->IsValid() && out->IsNeeded();
}

void DesignSolve::Run() {

	design->Update();

	UpdatePatches(0.05);

	design->Modify(false);
	out->MarkValid(true);
	out->MarkNeeded(false);
}

void DesignSolve::UpdatePatches(double res, const Polynomial &scaleU,
		const Polynomial &scaleV) {
#ifdef DEBUG
	double upShift = 0.00;
#endif

	out->patches.clear();

	// Search for the outlines of each patch.
	for (Design::Patch &p : design->patches) {
//		p.UpdateBoundingBox();

#ifdef DEBUG
		std::cout << "-----------\n";
		std::cout << "p.name = " << p.name << "\n";
#endif

//		std::vector<size_t> eInVCount(splitV.size(), 0);

		auto patchContainsEdge = [&p](const Design::SplitEdge &e) {
			return p.eidx.count(e.eidx) != 0;
		};

		// Generate a set of all possible edges
		std::set<Design::SplitEdge> pe;
		for (const Design::SplitEdge &e : design->splitE) {
			if (!patchContainsEdge(e))
				continue;
			pe.insert(e);
			// Add reverse of edge as well
			Design::SplitEdge e2 = e;
			e2.Flip();
			pe.insert(e2);
		}
		// Collect all loops
		std::vector<std::vector<Design::SplitEdge>> loops;
		std::vector<int> loopDirection;

		while (!pe.empty()) {
			std::set<Design::SplitEdge>::const_iterator en = pe.begin();
			std::vector<Design::SplitEdge> loop = design->FindLoop(*en, p.eidx);

			// Remove the edges of the loop found from the list of remaining
			// edges.
#ifdef DEBUG
			std::cout << "pe.size() = " << pe.size() << "\n";
#endif
			std::set<Design::SplitEdge> loopSet(loop.begin(), loop.end());
			std::set<Design::SplitEdge> remaining;
			std::set_difference(pe.begin(), pe.end(), loopSet.begin(),
					loopSet.end(), std::inserter(remaining, remaining.begin()));
#ifdef DEBUG
//			std::cout << "remaining.size() = " << remaining.size() << "\n";
//			std::cout << "------------\n pe:\n";
//			for (const Design::SplitEdge &e : pe)
//				std::cout << e.eidx << ", " << e.vidx0 << ", " << e.vidx1
//						<< "\n";
//			std::cout << "------------\n loopSet:\n";
//			for (const Design::SplitEdge &e : loopSet)
//				std::cout << e.eidx << ", " << e.vidx0 << ", " << e.vidx1
//						<< "\n";
//			std::cout << "------------\n remaining:\n";
//			for (const Design::SplitEdge &e : remaining)
//				std::cout << e.eidx << ", " << e.vidx0 << ", " << e.vidx1
//						<< "\n";
#endif
			pe.swap(remaining);
#ifdef DEBUG

//			const Vector2 &v0 = splitV[loop.front().vidx0];
//			std::cout << "(" << v0.u << ", " << v0.v << ")";
			std::cout << loop.front().vidx0;

			for (const Design::SplitEdge &e : loop) {
				const Vector2 &v1 = design->splitV[e.vidx1];
//				std::cout << " -> (" << v1.u << ", " << v1.v << ")";
				std::cout << " -> " << e.vidx1;
			}
			std::cout << "\n";
#endif

			// Check orientation of loop
			double s = 0.0;
			int zeroCount = 0;
			Vector2 pLast;
			double uOffset = 0;
			for (const Design::SplitEdge &e : loop) {
				const Design::PatchEdge &ed = design->edges[e.eidx];
				const Vector2 &v0 = design->splitV[e.vidx0];
				const Vector2 &v1 = design->splitV[e.vidx1];
#ifdef DEBUG
				// Up to this point all vertex positions should have been
				// cleaned up.
				if (v0.u <= -M_PI - FLT_EPSILON)
					RUNTIME_ERROR(
							"v0.u below negative boundary. (" << v0.u << " < -M_PI)");
				if (v0.u > M_PI - FLT_EPSILON)
					RUNTIME_ERROR(
							"v0.u above positive boundary. (" << v0.u << " > M_PI)");
				if (v1.u <= -M_PI - FLT_EPSILON)
					RUNTIME_ERROR(
							"v1.u below negative boundary. (" << v1.u << " < -M_PI)");
				if (v1.u > M_PI - FLT_EPSILON)
					RUNTIME_ERROR(
							"v1.u above positive boundary. (" << v1.u << " > M_PI)");
#endif

				const size_t N = ed.Size() * 2 - 1;
				Polynomial pol = Polynomial::ByValue(0, e.r0, N - 1, e.r1);
				Vector2 p0 = ed(pol(0));
				Vector2 p1 = ed(pol(N - 1));

				// Count the crossings at +/- M_PI.
				double du0 = p0.u - v0.u;
				double du1 = p1.u - v1.u;

				zeroCount -= (int) std::round(du0 / 2.0 / M_PI);
				zeroCount += (int) std::round(du1 / 2.0 / M_PI);

				uOffset -= p0.u - pLast.u;
				for (size_t n = 1; n < N; n++) {
					p1 = ed(pol(n));
					s += (p0.u + uOffset) * (p1.v - p0.v)
							- p0.v * (p1.u - p0.u);
					p0 = p1;
				}
				pLast = p1;
			}

#ifdef DEBUG
			std::cout << "zeroCount = " << zeroCount;
			std::cout << "   s = " << s << "\n";
#endif

			if (zeroCount != 0)
				continue;

			loopDirection.push_back((s < -FLT_EPSILON) ? (-1) : (1));
			loops.push_back(loop);
//			if (s < -FLT_EPSILON) {
//				std::cout << "Area of loop is negative.\n";
//				loop.clear();
//				continue;
//			}
		}
#ifdef DEBUG
		std::cout << "pe.size() = " << pe.size() << "\n";
		std::cout << "-----------\n";
#endif
		DesignSolution::Patch pout(p.name);
		if (!loops.empty()) {

			// Check if an outer loop completes all edges or if an outer loop
			// plus some of the inner loops complete the edges exactly.

			size_t idxLoopOuter = nothing;
			std::set<size_t> idxLoopInner;

			std::vector<std::set<size_t>> coveredEdges;
			coveredEdges.resize(loops.size());
			for (size_t idx = 0; idx < loops.size(); idx++) {
				for (const Design::SplitEdge &e : loops[idx])
					coveredEdges[idx].insert(e.eidx);
			}

			for (size_t idx = 0; idx < loops.size(); idx++) {
				if (loopDirection[idx] <= 0)
					continue;

				// A valid loop contains all edges, that are specified in the
				// patch.
				if (coveredEdges[idx] == p.eidx) {
					// Perfect fit
					idxLoopOuter = idx;
#ifdef DEBUG
					std::cout << "Valid loop found: " << idx << "\n";
#endif
					break;
				}
				if (std::includes(p.eidx.begin(), p.eidx.end(),
						coveredEdges[idx].begin(), coveredEdges[idx].end())) {

					std::set<size_t> missing;
					std::set_difference(p.eidx.begin(), p.eidx.end(),
							coveredEdges[idx].begin(), coveredEdges[idx].end(),
							std::inserter(missing, missing.begin()));

					// Check if the inner area can be covered by reverse loops.

					for (size_t idx2 = 0; idx2 < loops.size(); idx2++) {
						if (loopDirection[idx2] >= 0)
							continue;
						if (std::includes(missing.begin(), missing.end(),
								coveredEdges[idx2].begin(),
								coveredEdges[idx2].end())) {
							idxLoopInner.insert(idx2);
							for (size_t v : coveredEdges[idx2])
								missing.erase(v);
						}

						if (missing.empty()) {
							idxLoopOuter = idx;
							break;
						}
						idxLoopInner.clear();
					}
				}
			}
			// Tesselate patch

			if (idxLoopOuter != nothing) {
#ifdef DEBUG
				std::cout << "Patch tesselation of " << idxLoopOuter << "\n";
#endif
				const size_t loopBack = pout.CountVertices();
				// Some edged are not connected to a loop but are off by +/-
				// 2 * M_PI.

				Vector2 lastU;
				bool first = true;
				for (const Design::SplitEdge &se : loops[idxLoopOuter]) {
					Design::PatchEdge &ed = design->edges[se.eidx];
					if (first) {
						first = false;
					} else {
						double Ushift = lastU.u - ed(se.r0).u;
						Ushift = std::round(Ushift / (2.0 * M_PI)) * 2.0 * M_PI;
						if (fabs(Ushift) > FLT_EPSILON)
							ed.ShiftU(Ushift);
					}
					pout.AddSplitEdge(ed, se.r0, se.r1, scaleU, scaleV, 0.001,
							0.005);
					lastU = ed(se.r1);
				}
				for (size_t n = loopBack + 1; n < pout.CountVertices(); n++)
					pout.AddEdge(n - 1, n);
				pout.AddEdge(pout.CountVertices() - 1, loopBack);
#ifdef DEBUG
				std::cout << "-----------\n";
#endif
			}
		}

#ifdef DEBUG
		pout.matrix = AffineTransformMatrix::Translation(-upShift * 0.00, 0.0,
				0.00*upShift);
		upShift += 1;
		if (!pout.PassedSelfCheck(false)) {
			std::cout << p.name << " has broken geometry.\n";
		}
#endif
		pout.UpdateBoundingBox();

		// Interpolate and tesselate the output-patch.
		pout.Update(0.005, scaleU, scaleV);

//		p.paintDirection = true;
		pout.paintVertices = true;
//		p.paintNormals = true;
		pout.paintTriangles = true;
		pout.paintEdges = true;
		pout.dotSize = 3;

		out->patches.push_back(pout);

	}
}

