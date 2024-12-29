///////////////////////////////////////////////////////////////////////////////
// Name               : AffineTransformMatrix.h
// Purpose            : A class to store a 3D affine transform matrix and provide operations upon.
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 22.07.2009
// Copyright          : (C) 2009 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_AFFINETRANSFORMMATRIX_H
#define L3D_AFFINETRANSFORMMATRIX_H

/** \class AffineTransformMatrix
 * 	\code #include "AffineTransformMatrix.h"\endcode
 * 	\ingroup Base3D
 *  \brief A class to store a 3D affine transform matrix and provide operations upon.
 *
 * This class stores and manages a 4x4 matrix. This matrix is organized the
 * same way, the transformation matrices of OpenGL are:
 *
 * <table>
 * <tr><td>R11</td><td>R12</td><td>R13</td><td>Tx</td></tr>
 * <tr><td>R21</td><td>R22</td><td>R23</td><td>Ty</td></tr>
 * <tr><td>R31</td><td>R32</td><td>R33</td><td>Tz</td></tr>
 * <tr><td>0</td><td>0</td><td>0</td><td>1</td></tr>
 * </table>
 *
 * Where R is responsible for rotation and scaling
 * and T for translation.
 * The vector \c a is organized as follows:
 *
 * <table>
 * <tr><td>0</td><td>4</td><td>8</td><td>12</td></tr>
 * <tr><td>1</td><td>5</td><td>9</td><td>13</td></tr>
 * <tr><td>2</td><td>6</td><td>10</td><td>14</td></tr>
 * <tr><td>3</td><td>7</td><td>11</td><td>15</td></tr>
 * </table>
 *
 * Right handed coordinate system:
 *  * X to the right of the screen
 *  * Y to the top of the screen
 *  * Z towards the viewer
 *
 */

#include <array>
#include <string>

class Vector3;

class AffineTransformMatrix {
public:
	enum class Orientation {
		RHS, ///< Right-handed system
		LHS ///< Left-handed system
	}; ///< Hande3dness of the coordinate system.
	enum class Style {
		Lines, BasePlane, Planes, Cylinder, Box, BoxWithoutColors
	}; ///< Drawing style for displaying the coordinate-system defined by the matrix.

	AffineTransformMatrix();
	explicit AffineTransformMatrix(Orientation orientation_);
	AffineTransformMatrix(const Vector3 &ex, const Vector3 &ey,
			const Vector3 &ez, const Vector3 &center);
	explicit AffineTransformMatrix(const std::string &numbers);

private:
	std::array<double, 16> a; ///< Transformation matrix
	Orientation orientation = Orientation::RHS; ///< Handedness of the coordinate system

public:
	void SetOrientation(Orientation orientation_); ///< Preset the behavior for future Set and Calculate operations
	Orientation GetOrientation() const; ///< Return the orientation flag
	Orientation CheckOrientation() const; ///< Calculate the orientations of the coordinate system by analyzing the matrix
	void UpdateOrientation(); ///< Set the orientation flag according the the matrix values

	void SetIdentity(); ///< Resets the matrix to the identity matrix.
	void SetZero(); ///< Set the matrix to all zeros.
	void ResetRotationAndScale(); ///< Resets the rotation and scale, but keeps the translation

	void SetOrigin(const Vector3 &center);

	void SetEx(const Vector3 &ex);
	void SetEy(const Vector3 &ey);
	void SetEz(const Vector3 &ez);
	void CalculateEx();
	void CalculateEy();
	void CalculateEz();

	Vector3 GetOrigin() const; ///< Returns the center point of the matrix.
	Vector3 GetEx() const;
	Vector3 GetEy() const;
	Vector3 GetEz() const;

	/**\brief Axis of rotation
	 * Not normalized vector of the axis of rotation.
	 *
	 * The length of the vector is 2*sin(angle).
	 *
	 * If there is no rotation the length of the vector is 0.0.
	 */
	Vector3 GetVectorOfRotation() const;

	/**\brief Return the so called "Normal-Matrix"
	 *
	 * The Normal-Matrix only rotates a vector. The vector is not scaled or
	 * translated.
	 *
	 * The Normal-matrix is the transpose of the inverse of the upper left
	 * 3x3-matrix. The rest is set to zero.
	 *
	 * (see also [Article](http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/))
	 *
	 * \return Normal-Matrix
	 */
	AffineTransformMatrix GetNormalMatrix() const;

	/**\brief Returns a normalized matrix
	 *
	 * The matrix is normalized by scaling the axes of the coordinate system
	 * to a length of 1. A normalized matrix is returned. The matrix this
	 * function is called on, stays unchanged.
	 *
	 * \return Normalized matrix
	 */
	AffineTransformMatrix Normal() const;
	void Normalize(); ///< Normalizes the matrix.

	/**\brief Normalizes and then orthogonalizes the matrix.
	 *
	 * Normalizes the vectors of the coordinate system to 1, orthogonalizes
	 * the resulting system and renormalizes the vectors again.
	 */
	void Orthogonalize();

	AffineTransformMatrix& operator*=(const AffineTransformMatrix &b); ///< Overloaded operator to allow correct multiplication of two matrices.
	AffineTransformMatrix operator*(const AffineTransformMatrix &b) const; ///< Overloaded operator to allow correct multiplication of two matrices.

	/*!\brief  Overloaded operator to allow correct division of two matrices.
	 *
	 * The division is done by inverting the second matrix and the multiplying both.
	 */
	AffineTransformMatrix& operator/=(const AffineTransformMatrix &b); ///< Overloaded operator to allow correct division of two matrices.
	AffineTransformMatrix operator/(const AffineTransformMatrix &b) const; ///< Overloaded operator to allow correct division of two matrices.

	void Invert(); ///< Inverts the matrix itself

	/*! \brief Returns the inverted matrix
	 *
	 * The transform used in here is optimized for matrices with 0,0,0,1 in the
	 * last row. It would not give the correct results for other matrices.
	 *
	 * \return Inverted matrix.
	 */
	AffineTransformMatrix Inverse() const;

	static AffineTransformMatrix Identity(); ///< Function returning an identity matrix.
	static AffineTransformMatrix Zero(); ///< Function returning an all-zero matrix.

	static AffineTransformMatrix Translation(double x, double y, double z);
	static AffineTransformMatrix Translation(Vector3 translation);

	static AffineTransformMatrix Scaling(double scale);
	static AffineTransformMatrix Scaling(double x, double y, double z);
	static AffineTransformMatrix Scaling(Vector3 scale);

	/*!\brief Rotation matrix around a given vector.
	 *
	 * Generates a rotation matrix around a given vector.
	 * \param vector Axis of rotation.
	 * \param phi Angle of rotation.
	 * \return Rotation matrix.
	 */
	static AffineTransformMatrix RotationAroundVector(Vector3 const &vector,
			double phi);

	/*!\brief An interwoven rotation matrix.
	 *
	 * Generates a rotation matrix around x,y,z.
	 * In this case the rotations are interwoven:
	 *
	 * Every rotation (around x, around y and around z) is done
	 * in infinitesimal small steps. On step around x, one step around y, ...
	 *
	 * This results in a rotation as expected from a 6-DOF controller.
	 */

	static AffineTransformMatrix RotationInterwoven(double x, double y,
			double z);

	/*! \brief Rotation matrix for rotation by mouse.
	 *
	 * This function is only a drop in until the RotateByTrackball function
	 * works.
	 *
	 * \param x Movement of mouse in x direction (=xnew-xold).
	 * \param y Movement of mouse in y direction (=ynew-yold).
	 * \param scale Scaling of the movement.
	 * \return Rotation matrix.
	 */
	static AffineTransformMatrix RotationXY(int x, int y, double scale);

	/*! \brief Rotation matrix composed by the Z,Y,X rule.
	 */
	static AffineTransformMatrix RotationXYZ(double x, double y, double z);

	/*!\brief Rotation matrix around a virtual trackball.
	 *
	 * @param x1 Old x-mouse position on screen
	 * @param y1 Old y-mouse position on screen
	 * @param x2 New x-mouse position on screen
	 * @param y2 New y-mouse position on screen
	 * @param r Radius of a sphere in screen units.
	 * @return Rotational Matrix
	 */
	static AffineTransformMatrix RotationTrackball(const double x1,
			const double y1, const double x2, const double y2, const double r);

	/**\brief Projection matrix for perspective projection.
	 *
	 * This matrix has element #11 set to -1 for perspective distortion.
	 * This matrix cannot be used with most operations in this class as
	 * the lowest row of the matrix is not [0, 0, 0, 1] but [0, 0, -1, 1].
	 *
	 * [Matrices - projection, view, model](https://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/)
	 */
	static AffineTransformMatrix Perspective(double fovy, double aspect,
			double znear, double zfar);

	/**\brief Projection matrix for orthogonal projection.
	 *
	 * [Matrices - projection, view, model](https://solarianprogrammer.com/2013/05/22/opengl-101-matrices-projection-view-model/)
	 */
	static AffineTransformMatrix Orthogonal(double left, double right,
			double bottom, double top, double znear, double zfar);

	void TranslateGlobal(double x, double y, double z); ///< Translate matrix in the global coordinate system.
	void TranslateLocal(double x, double y, double z); ///< Translate matrix in the local, rotated coordinate system.

	void ScaleGlobal(double x, double y, double z); ///< Scale matrix in the global coordinate system.
	void ScaleLocal(double x, double y, double z); ///< Scale matrix in the local coordinate system.

	void ShiftTransformPosition(const Vector3 &p); ///< Previous rotations and scalings will be transform around the given point p.

	Vector3 Transform(const Vector3 &v) const; ///< Apply the transformation matrix on a given vector.
	Vector3 Transform(const double x, const double y = 0.0,
			const double z = 0.0) const;
	Vector3 TransformWithoutShift(const Vector3 &v) const; ///< Apply the transformation matrix on a given vector without shifting the vector.
	Vector3 TransformWithoutShift(const double x, const double y = 0.0,
			const double z = 0.0) const;
	Vector3 operator*(const Vector3 &v) const;
	Vector3 operator()(const Vector3 &v) const; ///< Operator reference for Vector3 transformations.
	Vector3 operator()(const double x, const double y = 0.0, const double z =
			0.0) const;

	double& operator[](unsigned char index);
	double operator[](unsigned char index) const;

	double LocalX(const Vector3 &v) const; ///< Maps a global point onto the local coordinate system, returns the local x.
	double LocalY(const Vector3 &v) const; ///< Maps a global point onto the local coordinate system, returns the local y.
	double LocalZ(const Vector3 &v) const; ///< Maps a global point onto the local coordinate system, returns the local z.
	double GlobalX(double x = 0.0, double y = 0.0, double z = 0.0) const; ///< Calculates the global x, given a local point.
	double GlobalY(double x = 0.0, double y = 0.0, double z = 0.0) const; ///< Calculates the global y, given a local point.
	double GlobalZ(double x = 0.0, double y = 0.0, double z = 0.0) const; ///< Calculates the global z, given a local point.

	double Distance(const AffineTransformMatrix &other) const;

	std::string ToString(); ///< Output the 16 values of the matrix as a vector: "[a0, a1, a2, ..., a15]"

	void GLMultMatrix() const; ///< Multiply the matrix onto the active OpenGL matrix (right multiply)
	void GLSetUniformMatrix4(int location) const; ///< Set the mat4 as an uniform in a shader.
	void GLSetUniformMatrix3(int location) const; ///< Set the mat3 as an uniform in a shader.
	void Paint(const Style style, const double param0 = 1.0,
			const double param1 = 1.0, const double param2 = 1.0) const; ///< Display the coordinate system in OpenGL
private:
	/**\brief Set a stock color for displaying the coordinate system in OpenGL.
	 *
	 * \param useColors Activate the use of colors. Needed when using Stereo-3D.
	 * \param color Enumeration of colors: 0 = black, 1 = red, 2 = green, 3 = blue, 4 = white
	 * \param alpha Opacity
	 * \param bright Bright variant of the color
	 */
	static void SetColor(const bool useColors, const unsigned char color,
			const double alpha = 1.0, bool bright = false);
	static void GLVertex(const Vector3 &v); ///< Helper to send Vector3 to OpenGL as vertex.
	static void GLNormal(const Vector3 &n); ///< Helper to send Vector3 to OpenGL as normal.
};

#endif /* L3D_AFFINETRANSFORMMATRIX_H */

