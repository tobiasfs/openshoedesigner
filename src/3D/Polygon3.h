///////////////////////////////////////////////////////////////////////////////
// Name               : Polygon3.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : -std=c++14 or greater
// Author             : Tobias Schaefer
// Created            : 07.07.2011
// Copyright          : (C) 2011 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_POLYGON3_H
#define L3D_POLYGON3_H

/**\class Polygon3
 * \brief Polygon in 3D space.
 *
 * Overload of the Geometry class with special functions for polygon(s) in
 * 3D-space.
 *
 * A polygon is stored as a list of vertices and a list of edges referencing the
 * vertices.
 *
 * In contrast to Geometry objects, the lists are not sorted and duplicated
 * vertices are not identified to keep the structure of the polygons as they
 * are.
 *
 * Each vertex (and edge) belongs to a group. Groups start with index 0.
 *
 * The normal, and the color is stored in the vertex. These attributes in the edges are
 * ignored.
 */

#include "Geometry.h"

#include <stddef.h>
#include <functional>
#include <map>
#include <vector>

class Polygon3: public Geometry {
public:
	enum class CalculateNormalMethod {
		ByCenter, ///< Calculate normals with respect to the center of the polygon
		ByBends, ///< Calculate normals by examining the bends in the polygon
		InPlaneXY, ///< Calculate the normal as if the polygon is projected into the XY plane
		InPlaneYZ, ///< Calculate the normal as if the polygon is projected into the YZ plane
		InPlaneZX ///< Calculate the normal as if the polygon is projected into the ZX plane
	};

	/**\brief Result for the At() method.
	 *
	 * When a point on the polygon is requested, this function returns info on
	 * that point.
	 */
	struct Result {
		Geometry::Vertex pos; //< Position
		Vector3 normal;
		Vector3 dir;
		size_t idx = 0;
		size_t group = 0;
		double rel = 0.0;
	};

	Polygon3();

public:
	void Clear(); ///< Clear all points from the Polygon

	/**
	 * \name Initializing
	 *
	 * Adding vertices to the polygon.
	 *
	 * The functions NextGroup() and CloseLoopNextGroup() are optional, if the
	 * current line should loop back to the start and/or another line should be
	 * started.
	 * \{
	 */

	/**\brief Adds a vertex and an edge to the polygon
	 *
	 * If not the first vertex in a group or in the polygon, it also adds an
	 * edge from the last vertex to the given vertex.
	 *
	 * \param vertex Vertex with position and optionally normal vector and color.
	 *
	 */
	void AddEdgeToVertex(const Geometry::Vertex &vertex);

	/**\brief Closes the current group
	 *
	 * Closes the current group and starts the next by incrementing the group
	 * counter.
	 *
	 * This function has no effect, if there is no intention to add further
	 * vertices.
	 */
	void NextGroup();

	/** \brief Closes the current group and adds an edge to the start
	 *
	 * It closes the current group by adding an additional edge from the last
	 * vertex to the first vertex of the current group.
	 *
	 * Increments the group counter.
	 *
	 * If no further vertices are added, this function has only the effect of
	 * adding an additional edge from the end of line to the start.
	 */
	void CloseLoopNextGroup();

	/**\}
	 * \name Modification
	 * \{
	 */

	/** \brief Extract the outline from a given geometry.
	 *
	 *
	 *
	 */
	void ExtractOutline(const Geometry &other);

	/** \brief Sort the edges and vertices into a loop.
	 *
	 * The Finish() command resorts the vertices, edges (and triangles) for
	 * better accessibility.
	 *
	 * For loops it is in some algorithms necessary to re-sort the edges to
	 * form a loop or multiple loops.
	 *
	 */
	void SortLoop();

	Polygon3& operator+=(const Polygon3 &a); ///< Append another Polygon3 to this one
	const Polygon3 operator+(const Polygon3 &a) const; ///< Append two Polygon3%s
//	Polygon3& operator*=(const double val);
//	const Polygon3 operator*(const double val);
//	Polygon3& operator/=(const double val);
//	const Polygon3 operator/(const double val);

	size_t Size() const; //<Get the number of vertices
	Vertex& operator[](size_t index); //< Manipulate a vertex
	const Vertex& operator[](size_t index) const; //< Const inspect a vertex

	/** \brief Direction away from a vertex
	 *
	 *  Returns a zero vector, if the vertex is the last in a group or in the
	 *  polygon.
	 *
	 *  If there are two or more edges away from a given vertex, the first edge
	 *  found is used to calculate the direction.
	 *
	 *  The returned vector is not normalized. Its length is exactly the length
	 *  of the edge to the next vertex.
	 *
	 * \param index Index of vertex
	 * \return Not-normalized vector pointing to the next vertex in line
	 */
	Vector3 Direction(size_t index) const;
	Vector3& Normal(size_t index);
	const Vector3& Normal(size_t index) const;

	/**\brief Recalculate the normals
	 *
	 * The normals of the edges are recalculated using the method indicated by
	 * the parameter. The vertices have their normals interpolated between the
	 * adjacent edges.
	 */
	void CalculateNormals(const CalculateNormalMethod method =
			CalculateNormalMethod::ByCenter);
	/**\brief Recalculate the normals by giving the normal of a plane.
	 *
	 * Assuming the normals are running mathematically positive in the plane
	 * (counter clockwise), the normals are calculated by multiplying the
	 * direction of the edge by the given plane normal and re-normalizing the
	 * resulting vector.
	 *
	 * The vertex normals are interpolated between the adjacent edges.
	 */
	void CalculateNormalsAroundVector(const Vector3 &planenormal);

	double MapU();

	void Shift(double distance); ///< Move the polygon along the normals.

	void RemoveZeroLength(); ///< Remove all segments from the polygon, that have a length of zero

	void Reverse(); ///< Reverse all groups.
	void Reverse(size_t group); ///< Reverse the direction of one group
	void RotateOrigin(const Vector3 &p);
	void RotateOrigin(const Vector3 &p, size_t group); ///< Rotate the first point of the polygon close to the given point.

	void Triangulate();

//	/** \brief Resample the point in the polygon.
//	 *
//	 * The polygon is resampled into a polygon with N points. This can be an over- or undersampling of the original polygon.
//	 * @param Nnew New number of points
//	 */
//	void Resample(unsigned int Nnew);

	/** \brief Moving-average filter for the points of the polygon
	 *
	 * Use a simple MA FIR filter to filter the points of the polygon in all three dimensions.
	 * The filter length is chosen symmetric around the point filtered. E.g. if N = 3, the point
	 * n is averages to the mean value of n-1, n, n+1. For even numbers the extra point is take from the
	 * negative part. E.g. N = 4 results in the mean value of n-2, n-1, n, n+1.
	 *
	 *\todo Revisit and check, if everything is implemented according to the description.
	 *
	 * @param width length of the MA filter.
	 */
	void Filter(unsigned int width);

	/**
	 * \}
	 * \name Polygon properties
	 * \{
	 */

	/** \brief Returns the length of the polygon
	 */
	double GetLength() const;
	/** \brief
	 *
	 * \param group
	 * \return
	 */
	double GetLength(size_t group) const;

	/** \brief Returns a std::vector with all x values of the polygon.
	 *
	 * \note The datatype is converted from float to double upon export.
	 *
	 * @return std::vector \<double\> with all x values.
	 */
	std::vector<double> GetXVectorD() const;

	/** \brief Returns a std::vector with all y values of the polygon.
	 *
	 * \note The datatype is converted from float to double upon export.
	 *
	 * @return std::vector \<double\> with all y values.
	 */
	std::vector<double> GetYVectorD() const;

	/** \brief Returns a std::vector with all z values of the polygon.
	 *
	 * \note The datatype is converted from float to double upon export.
	 *
	 * @return std::vector \<double\> with all z values.
	 */
	std::vector<double> GetZVectorD() const;

	/** \brief Get the center of the polygon.
	 *
	 * This function integrates over the length of the polygon.
	 * This means, that the number of points on a line do not matter.
	 *
	 * \return Center of polygon.
	 */
	Vector3 GetCenter() const;

	/** \brief Get center of one group in the polygon.
	 *
	 * \param group Group to calculate the center for.
	 * \return Center for the given group.
	 */
	Vector3 GetCenter(size_t group) const;

	/** \brief Calculates the area of the polygon
	 *
	 * This function assumes the polygon is closed and uses the vectorproduct
	 * to calculate the area of the polygon. This approach works correctly
	 * with holes in the polygon.
	 *
	 * Counter-clockwise (=mathematically positive) loops add to the area,
	 * clockwise ones (=mathematically negative) subtract.
	 *
	 * This approach stops working, if the hole is outside the area. In this
	 * case it is still subtracted.
	 */
	double GetArea() const;

	/** \brief Get rotational axis
	 *
	 * This function calculates the axis of rotation of the polygon.
	 */
	Vector3 GetRotationalAxis() const;

	Result At(double L) const;
	std::tuple<size_t, size_t> ClosestPoint(const Vector3 &p) const;
	size_t ClosestPoint(const Vector3 &p, size_t group) const; ///< Returns the closest index for the given point.

	/**\}
	 */

	struct Intersections {
		std::vector<Geometry::Vertex> positive;
		std::vector<Geometry::Vertex> negative;
	};
	/**\brief Intersect the polygon with a plane.
	 *
	 * \param n Normal vector of plane
	 * \param d Distance from origin of the plane
	 * \return Structure with positive and negative intersections of the plane.
	 */
	Intersections Intersect(Vector3 n, double d) const;

	std::string ToString() const;

private:
	/** \brief Add a triangle reusing an already existing edge.
	 *
	 * This is low-level, needed for the Triangulate() function.
	 *
	 * \param idx0 Indices of the corners
	 * \param idx1 Indices of the corners
	 * \param idx2 Indices of the corners
	 */
	void AddTriangleMinimal(size_t idx0, size_t idx1, size_t idx2);

protected:
	size_t groupCount = 0;
	size_t firstIndex = 0;
	size_t lastIndex = 0;
};

#endif /* L3D_POLYGON3_H */
