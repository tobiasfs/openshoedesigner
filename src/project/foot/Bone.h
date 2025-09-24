///////////////////////////////////////////////////////////////////////////////
// Name               : Bone.h
// Purpose            : Simple bone
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 12.08.2015
// Copyright          : (C) 2015 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef BONE_H
#define BONE_H
/*!\class Bone
 * \brief Simple bone approximated by a capsule.
 *
 * Simple bone model with two radii for both ends.
 * Definition of a link may be confusing but is choosen
 * this way to make every bone scaleable while keeping
 * the skeleton intact.
 *
 * Rotations are done with 50-50 split rotations.
 *
 * The parent bone provides an anchor, a bone normal and a length.
 *
 * First calculate the normal intersection from link and parent bond.
 *
 * p1Parent + normalParent*lengthParent*linkD
 *
 * add the offset vector linkN times the radius of the parent bone at this spot.
 *
 * + linkN*rLocalParent
 *
 * Second calculate p1. It is the link-point + anchor*(rLocalParent+r1)
 *
 * Third calculate p2 by adding p1 + normal*length
 *
 */

#include "../../3D/AffineTransformMatrix.h"
#include "../../3D/Vector3.h"

#include <memory>
#include <stddef.h>
#include <string>

class Bone {
	friend class Skeleton;
public:
	Bone() = default;
	explicit Bone(const std::string &name);

	void Update();
	void UpdateHierarchy();

	double GetXMax() const;
	double GetXMin() const;
	double GetZMin() const;

	void PushRotation();
	void PopRotation();

	friend bool operator==(const std::shared_ptr<Bone> &bone,
			const std::string &name) {
		return name.compare(bone->name) == 0;
	}

	void Paint() const;

private:
	double CalculateAnchorPoint(const Vector3 &p) const;

public:
	std::string name;
	AffineTransformMatrix matrixinit; //!< Describing the basic coordinate system of the bone.
	double roty = 0.0; //!< Rotation around (up x normal) axis
	double rotz = 0.0; //!< Rotation around up axis

	double r1 = 0.0; //!< Radius of 1st sphere
	double r2 = 0.0; //!< Radius of 2nd sphere. If the bone has an parent, this is calculated automatically
	double s1 = 0.01; //!< Skin thickness on 1st sphere
	double s2 = 0.01; //!< Skin thickness on 2nd sphere

	Vector3 p1; //!< Second point
	Vector3 p2; //!< Second point

	std::string formulaLength;
	std::string formulaR1;
	std::string formulaR2;
	std::string formulaS1;
	std::string formulaS2;

private:
	std::weak_ptr<Bone> parentTo;
	size_t hierarchyLevel = 0;

	bool initialized = false;

	AffineTransformMatrix matrix; //!< Rotated coordinate-system
	double lengthinit = 0.0; //!< Length from p1 to p2
	double length = 0.0;

	double r1init = 0.0; //!< Radius of 1st sphere
	double r2init = 0.0; //!< Radius of 2nd sphere. If the bone has an parent, this is calculated automatically

	double rparentinit = 0.0;

	double anchorDinit = 1.0; ///< Relative position of link in parent bone along the vector p1 .. p2. Value range from 0..1
	double mixing = 0.5;
	AffineTransformMatrix link;
	double linkLengthInit = 0.0;

	double rotyBackup = 0.0; //!< Rotation along the x axis
	double rotzBackup = 0.0; //!< Rotation along the y axis
};

#endif /* BONE_H */
