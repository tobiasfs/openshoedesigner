///////////////////////////////////////////////////////////////////////////////
// Name               : DesignSolution.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 19.03.2026
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

#ifndef SRC_PROJECT_OBJECT_DESIGNSOLUTION_H_
#define SRC_PROJECT_OBJECT_DESIGNSOLUTION_H_

/*!\class DesignSolution
 * \brief ...
 *
 * ...
 */

#include "../../3D/Polygon3.h"
#include "../configuration/Design.h"
#include "Object.h"

class DesignSolution: public Object {
public:
	DesignSolution();
	virtual ~DesignSolution() = default;

	/**\brief Container for a patch
	 *
	 *
	 *
	 */
	class Patch: public Polygon3 {
	public:
		explicit Patch(const std::string &name = std::string(""));
		Patch(std::initializer_list<size_t> eidx, const std::string &name =
				std::string(""));

		virtual ~Patch() = default;

		void AddSplitEdge(const Design::PatchEdge &ed, double p0, double p1,
				const Polynomial &scaleU, const Polynomial &scaleV,
				double maxErr, double maxDist);

		void Update(double edgeLength, const Polynomial &scaleU,
				const Polynomial &scaleV);
		void UpdateBoundingBox();

		std::string name;
		std::set<size_t> eidx;

		double Umin;
		double Umax;
		double Vmin;
		double Vmax;

	private:
		double SplitAddEdge(const Design::PatchEdge &ed, double p0, double p2,
				const Vector2 &v0, const Vector2 &v2, double d02,
				const Polynomial &scaleU, const Polynomial &scaleV,
				double maxErr2, double maxDist);
	};

public:
	std::vector<Patch> patches;

};

#endif /* SRC_PROJECT_OBJECT_DESIGNSOLUTION_H_ */
