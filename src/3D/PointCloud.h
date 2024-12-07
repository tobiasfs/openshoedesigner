///////////////////////////////////////////////////////////////////////////////
// Name               : PointCloud.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 14.04.2019
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

#ifndef L3D_POINTCLOUD_H
#define L3D_POINTCLOUD_H

/*!\class PointCloud
 * \brief Stores a point cloud
 *
 * Also contains functions for the reconstruction of a Geometry object from
 * the point cloud data.
 */

#include "Vector3.h"

#include <vector>

class PointCloud {
public:
	PointCloud() = default;
	virtual ~PointCloud() = default;

	void InitExample(void);

	void Paint(void);

	std::vector<Vector3> p;
};

#endif /* L3D_POINTCLOUD_H */
