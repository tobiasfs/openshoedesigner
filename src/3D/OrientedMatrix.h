///////////////////////////////////////////////////////////////////////////////
// Name               : OrientedMatrix.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 05.11.2016
// Copyright          : (C) 2016 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef ORIENTEDMATRIX_H
#define ORIENTEDMATRIX_H

/*!\class OrientedMatrix
 * \brief Field in 3D-space
 *
 * This matrix has an origin and a size.
 */

#include "../math/Matrix.h"
#include "Vector3.h"

class OrientedMatrix: public Matrix {
public:
	enum class Display3D {
		Points3D
	};
	enum class Display2D {
		Points2D, Grid, Image
	};

public:
	OrientedMatrix() = default;
	virtual ~OrientedMatrix() = default;

	/*! \brief Set the Origin
	 *
	 * Set the origin of the Volume to this point. Together with SetCount or SetSize
	 * it determines the start and end coordinates of the Volume.
	 *
	 * @param origin Vector3 of origin
	 */
	void SetOrigin(Vector3 origin);
	void SetSize(unsigned int nx, unsigned int ny, unsigned int nz,
			float resolution = 1.0);
	void SetExtent(float x, float y, float z, float resolution = 1.0);

	/*! \brief Return the Value at a point
	 *
	 * Returns the interpolated value of the field at the point p. Points
	 * outside the volume are 0.
	 *
	 * @param p Vector3 point
	 * @return Value at the point p
	 */
	double GetValue(Vector3 p) const;

	/*! \brief Return the Value at a point
	 *
	 * Returns the interpolated value of the field at the point x,y,z. Points
	 * outside the volume are 0.
	 *
	 * @param x coordinate
	 * @param y coordinate
	 * @param z coordinate
	 * @return Value at the point p
	 */
	double GetValue(double x, double y, double z) const;

	/*! \brief Calculate the gradient at point p
	 *
	 * Calculates the gradient in a point by deriving the value of the field in x,y and z.
	 * The returned vector points in the direction of the steppest ascent in the field.
	 * @param p Vector3 point
	 * @return Vector3 gradient at point p
	 */
	Vector3 GetGrad(Vector3 p) const;

	/*! \brief Fill a Volume with the underside of the volume
	 *
	 * Calculate the lower surface of the volume and returns this data in a 2D-%Volume.
	 *
	 * @return Volume with heightfield
	 */
	OrientedMatrix SurfaceField() const;

	/*! \brief Render the data
	 *
	 * After the Marching-Cubes algorithm has run, the generated Geometry can be
	 * rendered with OpenGL commands.
	 */
	void Paint() const;

	/*! \brief Rotate Volume
	 *
	 * Rotate the Volume around an axis. Some functions use this feature to access the model from all sides.
	 *
	 * @param a Axis of rotation (enum X,Y or Z).
	 * @param quarters Number of 90 degree turns to do.
	 */
	void Rotate(Axis a, int quarters);

	OrientedMatrix XRay(Method method) const;

public:
	Vector3 origin; ///< Origin of the volume
	float dx = 1.0;
	float dy = 1.0;
	float dz = 1.0;

	Display2D display2D = Display2D::Image;
	Display3D display3D = Display3D::Points3D;

	double surface = 0.0; ///< Value of the surface: values greater than surface are inside the Volume
};

#endif /* ORIENTEDMATRIX_H */
