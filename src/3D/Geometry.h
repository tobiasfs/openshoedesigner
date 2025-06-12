///////////////////////////////////////////////////////////////////////////////
// Name               : Geometry.h
// Purpose            : Store and manipulate 3D objects
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : -std=c++14 or greater
// Author             : Tobias Schaefer
// Created            : 22.09.2023
// Copyright          : (C) 2023 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_GEOMETRY_H
#define L3D_GEOMETRY_H

/**\class Geometry
 * \code #include "Geometry.h"\endcode
 * \ingroup Base3D
 * \brief Self contained class for the storage and manipulation of 3D objects
 *
 * Using sorted stores simplifies the process of manipulating 3D data even if
 * it originates from a triangle soup.
 *
 * If possible a closed sorted surface is generated. After adding geometry,
 * call the Sort() or the Join() function to clean up the geometry.
 *
 * # Selection
 *
 * Selection is done with respect to vertices. If all vertices of an edge are
 * selected, the edge is selected. If all vertices of a triangle are selected,
 * the triangle is selected.
 *
 * The functions PaintTriangle() and PaintEdge() offer passing in
 * a std::set<size_t> of group-ids. Only the elements of the selected groups
 * are painted. If no set is passed, all elements are drawn.
 */

#include "AffineTransformMatrix.h"
#include "Vector3.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

class Polygon3;

class Geometry {
public:

	class Color {
	public:
		Color() = default;

		float r = 0.8f;
		float g = 0.8f;
		float b = 0.8f;
		float a = 1.0f;
	};

	class Vertex: public Vector3 {
	public:
		Vertex() = default;
		Vertex(double x_, double y_, double z_ = 0.0, double u_ = 0.0,
				double v_ = 0.0);
		Vertex(const Vector3 &vector, const Vector3 &normal = Vector3());

		void Transform(const AffineTransformMatrix &m);
		void FlipNormal();

		Vertex Interp(const Vertex &b, const double mix) const;

		Vector3 n = Vector3();
		Color c = Color();
		size_t group = (size_t) -1;
		double u = 0.0; // Texture u coordinate
		double v = 0.0; // Texture v coordinate
	};

	/**\brief Edge between two vertices
	 *
	 * The vertices of the edge are stored so, that the first index is smaller
	 * then the second one. This speeds up searching for edges.
	 */
	class Edge {
	public:
		Edge() = default;

		/**\brief Invert the order of vertices for this edge, if necessary
		 *
		 * This function only changes the order of the vertices. The order of
		 * triangles stays unchanged.
		 */
		void Fix();

		void FlipNormal();

		/**\brief Return the index of the other vertex connected
		 *
		 *  If the given index is the index of on of the triangle attached, the
		 *  other triangle is returned. If the triangle was not found, the index
		 *  of the first triangle is returned.
		 *
		 *  This function never throws. (Even if the index is out of range.)
		 *
		 * \param index of one of the triangles. to the other end of the edge given the vertex.
		 * \return The index of the other triangle of the index of the first triangle.
		 */
		size_t GetOtherVertex(size_t index) const;

		/**\brief Return the index of the other triangle connected
		 *
		 *  This function never throws. (Even if the index is out of range.)
		 *
		 * \param index Index to one of the vertices connected to the edge.
		 * \return The index of the other vertex.
		 */
		size_t GetOtherTriangle(size_t index) const;

		size_t GetVertexIndex(uint_fast8_t index) const;

		/** \brief Check if an edge is collapsed
		 *
		 * An edge serves no function and is collapsed, if both vertices are
		 * the same vertex.
		 *
		 * \return True if collapsed.
		 */
		bool IsCollapsed() const;

		size_t va = (size_t) -1;
		size_t vb = (size_t) -1;
		size_t ta = (size_t) -1;
		size_t tb = (size_t) -1;

		Vector3 n = Vector3();
		Color c = Color();
		size_t group = (size_t) -1;

		uint_least8_t trianglecount = 0;
		bool sharp = false;
		bool flip = false;
	};

	/** \brief Triangle between three vertices
	 *
	 * The vertices of the triangle are stored in ascending order. The flip flag
	 * indicates if the order has to be inverted (for painting). This speeds up
	 * searching by making triangles sortable and comparable.
	 */
	class Triangle {
	public:
		Triangle() = default;

		/**\brief Sort the order of the vertices for this triangle
		 *
		 * This function sorts the order of vertices into ascending order. It
		 * also sorts the associated edges and flips the direction of the
		 * triangle if needed.
		 */
		void Fix();

		void FlipNormal();

//		bool FromString(const std::string &string);
//		std::string ToString() const;

		/** \brief Test the direction of rotation of two indices respective to
		 *         the triangle.
		 *
		 *	Checks if two indices are in clockwise or counterclockwise
		 *	orientation on the given triangle.
		 *
		 * \param index0 Reference
		 * \param index1 Next index
		 * \return +1 for mathematically positiv, -1 for mathematically
		 * 	       negative, and 0 if one or both vertices do not belong to
		 * 	       the triangle.
		 */
		int GetDirection(size_t index0, size_t index1) const;

		size_t GetVertexIndex(uint_fast8_t index) const;
		size_t GetEdgeIndex(uint_fast8_t index) const;
		uint_fast8_t GetVertexPosition(size_t idx) const;
		uint_fast8_t GetEdgePosition(size_t idx) const;

		/** \brief Check if a triangle is collapsed
		 *
		 * A triangle has collapsed, if two of the vertices are the same
		 * vertex. This triangle has no volume.
		 *
		 * \return True is collapsed.
		 */
		bool IsCollapsed() const;

		size_t va = (size_t) -1;
		size_t vb = (size_t) -1;
		size_t vc = (size_t) -1;
		size_t ea = (size_t) -1;
		size_t eb = (size_t) -1;
		size_t ec = (size_t) -1;
		double tua = 0.0;
		double tva = 0.0;
		double tub = 0.0;
		double tvb = 0.0;
		double tuc = 0.0;
		double tvc = 0.0;
		bool flip = false; ///< A flipped triangle is painted in the order v0, v2, v1 instead of v0, v1, v2.
		Vector3 t = Vector3();
		Vector3 b = Vector3();
		Vector3 n = Vector3();
		Color c = Color();
		size_t group = (size_t) -1;
	};

public:
	Geometry() = default;
	virtual ~Geometry();

	/**
	 * \name Initializing
	 * \{
	 */

	/**\brief Set the smallest distance for two separate vertices
	 *
	 * If a distance between two vertices is smaller than epsilon, the vertices
	 * are considered to be the same vertex.
	 */
	void SetEpsilon(double newEpsilon);

	void CopyPropertiesFrom(const Geometry &other);

	void Clear(); ///< Remove all triangles, edges and vertices from the object.
	bool IsEmpty() const;
	bool IsClosed() const; ///< Test, if the hull is perfectly closed.
	bool IsFinished() const; ///< Test if the Finish() function has been called after adding geometries.

	/**\}
	 * \name Presets for adding geometry
	 * \{
	 */

	/**\brief Add a vertex normal, that is used with all following add-operations.
	 *
	 * \param n Normal to use.
	 */
	void SetAddNormal(const Vector3 &n);
	void SetAddNormal(double nx, double ny, double nz);
	void ResetAddNormal();

	void SetAddColor(const Vector3 &c);
	void SetAddColor(double r, double g, double b, double a = 1.0);
	void ResetAddColor();

	void SetAddMatrix(const AffineTransformMatrix &m);
	void ResetAddMatrix();

	void ResetPresets();

	/**\}
	 * \name Insert Geometry
	 *
	 * Call the add-functions of this group to add raw geometry to this object.
	 * After all elements have been added call the Finish() function. This
	 * function joins the vertices and edges into one single hull (if possible).
	 *
	 * Afterwards CalculateNormals() recalculated the normal vectors.
	 *
	 * CalcGroups() adds group labels to all triangles. Groups are separate if
	 * the angle between normals is greater than a given angle.
	 *
	 * \{
	 */

	/** \brief Adds a vertex and sets up vmap for the given source-index.
	 *
	 * This function is used to copy vertices between geometries. by updating
	 * the vmap, later after adding all vertices, the function Remap() can be
	 * called to change the vertices in the edges and in the triangles.
	 *
	 * Note, that there is also a corresponding AddEdge and AddTriangle
	 * function, that use a source-index.
	 *
	 * \param vertex Vertex to add
	 * \param sourceIndex Index of the vertex in the source Geometry.
	 */
	void AddVertex(const Geometry::Vertex &vertex, size_t sourceIndex =
			(size_t) -1);
//	void AddVertex(const Vector3 &vertex);
	void AddVertex(const std::vector<Vector3> &vertices);

	/** \brief Adds an edge and sets up emap for the given source-index.
	 *
	 * This function is used to copy edges between geometries. by updating
	 * the emap, later after adding all vertices, the function Remap() can be
	 * called to change the edges in the triangles.
	 *
	 * Note, that there is also a corresponding AddVertex and AddTriangle
	 * function, that use a source-index.
	 *
	 * \param edge Edge to add
	 * \param sourceIndex Index of the edge in the source Geometry.
	 */
	void AddEdge(const Geometry::Edge &edge, size_t sourceIndex = (size_t) -1);
	void AddEdge(const Geometry::Vertex &vertex0,
			const Geometry::Vertex &vertex1);
//	void AddEdge(const Vector3 &vertex0, const Vector3 &vertex1);
	void AddEdge(size_t index0, size_t index1);

	/** \brief Adds a triangle and sets up tmap for the given source-index.
	 *
	 * This function is used to copy triangles between geometries. by updating
	 * the tmap, later after adding all triangles, the function Remap() can be
	 * called to change the triangles in the edges.
	 *
	 * Note, that there is also a corresponding AddVertex and AddEdge
	 * function, that use a source-index.
	 *
	 * \param triangle Triangle to add
	 * \param sourceIndex Index of the triangle in the source Geometry.
	 */
	void AddTriangle(const Geometry::Triangle &triangle, size_t sourceIndex =
			(size_t) -1);
	void AddTriangle(const Geometry::Vertex &vertex0,
			const Geometry::Vertex &vertex1, const Geometry::Vertex &vertex2);
//	void AddTriangle(const Vector3 &vertex0, const Vector3 &vertex1,
//			const Vector3 &vertex2);
	void AddTriangle(size_t index0, size_t index1, size_t index2);

	void AddQuad(const Vector3 &vertex0, const Vector3 &vertex1,
			const Vector3 &vertex2, const Vector3 &vertex3);
	void AddQuad(size_t index0, size_t index1, size_t index2, size_t index3);

//	Geometry& operator=(const std::vector<Triangle> &triangles);
//	void AddTrianglesFrom(const std::vector<Triangle> &triangles);

	void AddFrom(const Geometry &other);

	/**\brief Copies the selected triangles, edges and vertices.
	 *
	 * Copies all selected vertices first, updates the vertex mapping. Copies
	 * all edges, where both vertices are selected, updates the edge mapping.
	 * Copies all triangles where all vertices are selected, updates the
	 * triangle mapping.
	 *
	 * \note This function does not clear the current geometry.
	 */
	void AddSelectedFrom(const Geometry &other);

	/**\brief Fix the ordering of indices in edges and triangles
	 *
	 * Edges and triangles have references to vertices and edges/vertices
	 * respectively. To compare these efficiently, the vertex/edge indices have
	 * to be ordered. This function does that by calling the Fix function for
	 * edges and triangles.
	 *
	 * For triangles only the vertex-indices are ordered. The edge-indices are
	 * not ordered, because for this the edges need to be sorted in a way that
	 * is not possible because one edge can belong to two triangles.
	 *
	 * The actual direction of the edges/triangles is indicated by the .flip
	 * member variable.
	 *
	 * Sort() is an extension to Fix() doing both operations.
	 */
	void Fix();

	/**\brief Sort vertices, edges, triangle
	 *
	 * Unlike Join() this function sorts all elements, but does not join
	 * elements that are close together.
	 *
	 * This function also fixes the orientations, so that always va <= vb <= vc.
	 * And also, because the edges are also sorted ea <= eb <= ec.
	 *
	 * Join() is an extension to Sort() that does the steps Fix(), Sort() and
	 * finally joins duplicated vertices, edges and triangles.
	 *
	 * \note The .group member variables of the vertices, edges and triangles
	 * are set to incrementing values during this process.
	 */
	void Sort();

	/**\brief Join vertices and edges, that are closer than epsilon
	 *
	 * Fixes and sorts all elements in the geometry and joins vertices, that
	 * are closer than epsilon together. Afterwards some edges might be
	 * identical because they connect the same vertices. These are dissolved.
	 * (The normals and colors are interpolated between the two joined edges.)
	 * The same is done for triangles.
	 */
	void Join();

	/**\brief Finish the insertion process.
	 *
	 * Spreads the normals, fixes, sorts and joins the vertices, edges and
	 * triangles. Calculates the UV coordinate-systems in the triangles.
	 */
	void Finish();

	/** \brief Update the references based on the maps.
	 *
	 * The references to the indices in the other elements are updated. The
	 * updating is only applied to elements equal or greater than the given
	 * start values.
	 *
	 * If no start values are passed, everything is updated. When copying
	 * data between geometries, this parameters are used to only update the
	 * copied elements.
	 *
	 * \param vstart Starts updating the vmap from this index on.
	 * \param estart Starts updating the emap from this index on.
	 * \param tstart Starts updating the tmap from this index on.
	 */
	void Remap(int vstart, int estart, int tstart);

	/**\brief Do an integrity check on the elements in this geometry.
	 *
	 * This function can be called at any time, because at the lowest level
	 * the geometry should always be consistent. (After construction, after
	 * adding shapes, before and after calling Final(), after Remap(), ...)
	 *
	 * Usage:
	 *
	 * ~~~~~{.cpp}
	 * if(obj.PassedSelfCheck()){
	 *   std::cout << "Object is correct.\n";
	 * }else{
	 *   std::cout << "Object has not passed the self-check.\n";
	 * }
	 * ~~~~~
	 *
	 * \param maxErrorsPerType  How many errors to report to std::cerr before
	 * 						    moving to the next problem-type
	 * \param checkWellOrdering Additional check if the vertices in edges and
	 *                          Triangles are ordered ascending. Needs to be
	 *                          turned off for checking some polygon structures.
	 * \return True, if there are no errors in the geometry.
	 */
	bool PassedSelfCheck(bool checkWellOrdering = false,
			size_t maxErrorsPerType = 10) const;

	/**\}
	 */

	/**\brief Recalculate all normals
	 *
	 * Discards of all normals (vertex, edge, triangle) and recalculates them
	 * anew.
	 */
	void CalculateNormals();

	/**\brief Spread normals from the vertices to the edges and triangles
	 *
	 * Is called internally by Finish(), because that function averages some of
	 * the normal information by combining vertices and edges.
	 *
	 * If some normals are defined for the geometry (for triangles, edges or
	 * vertices) these are spread to the elements without normals. If no
	 * normals are available at all, the function CalculateNormals() is called.
	 */
	void PropagateNormals();

	/**\brief Update the normals of the vertices, edges, or triangles
	 *
	 * Use this function, if the vertices were updated together with the
	 * vertex normals externally and need to propagate to the edges and
	 * triangles.
	 */
	void UpdateNormals(bool updateVertices = false, bool updateEdges = true,
			bool updateTriangles = true);

	/**\brief Flip the normal vectors of vertices, edges and triangles.
	 */
	void FlipNormals();

	void FlagUV(bool inVertices, bool inTriangles);

	void CalculateUVFromBox();
	void CalculateUVFromAxis(const Vector3 &n, bool symmetric = false);
	void CalculateUVFromCylinder(const Vector3 &n);
	void CalculateUVFromSphere(const Vector3 &n);

	void TransformUV(const AffineTransformMatrix &matrix);

	/**\brief Calculate the UV coordinate system with normal, tangent and
	 * bi-tangent.
	 *
	 * These coordinate systems are used by the shaders to display the textures
	 * correctly.
	 */
	void CalculateUVCoordinateSystems();

	/**\brief Flip all edges and all triangles, normals are not changed
	 *
	 * Inverts the drawing order of the vertices in the triangles. This affects the
	 * backface-culling operation of OpenGL.
	 */
	void FlipInsideOutside();

	/**\brief For each edge decide, if sharp, given an max angle.
	 *
	 */
	void CalculateSharpEdges(double angle);

	/**\brief Reset group indices
	 *
	 * Reset the group-labels for all triangles, edges, and vertices.
	 * The whole object is made into a single group (= group 0)
	 */
	void ResetGroups();

	/**\brief Group triangles and edges by sharpness of edges
	 *
	 * Calls the function to find sharp edges CalcSharpEdges(...). Adds group
	 * labels to each triangle for every group of triangles surrounded
	 * completely by sharp edges or the outside.
	 *
	 * Groups are not assigned to edges and vertices, because these belong to
	 * more than one group (most of the time).
	 *
	 *  \param angle The angle of an edge above which the edge is considered "sharp".
	 */
	void CalculateGroups(double angle);

	/**\brief Group triangles into objects
	 *
	 * Triangles, that share an edge are considered to belong to the same
	 * object. All connected triangles are assigned the same group. This is
	 * propagated to the vertices and edges as well.
	 *
	 * \return Number of groups assigned.
	 */
	size_t CalculateObjects();

	/**\name Accessing vectors and triangles
	 * \{
	 */

	size_t CountVertices() const; ///< Size of the vector with the vertices.
	size_t CountEdges() const; ///< Size of the vector with the vertices.
	size_t CountTriangles() const; ///< Size of the vector with the vertices.

	const Vertex& operator[](size_t index) const; ///< Overloaded operator to view the vertices
	Vertex& operator[](size_t index); ///< Overloaded operator to manipulate the vertices

	const Vertex& GetVertex(size_t index) const; ///< Operator to view the vertices
	Vertex& GetVertex(size_t index); ///< Operator to manipulate the vertices

	/** \brief Return a vertex of a given edge.
	 *
	 * \param indexEdge Index of the edge to investigate
	 * \param indexVertex 0 = vertex at start of edge, 1 = vertex at the end
	 * \return Vertex
	 */
	const Vertex& GetEdgeVertex(const size_t indexEdge,
			uint_fast8_t indexVertex) const;
	/** \brief Return the vertices of a triangle.
	 *
	 * \param indexTriangle Return a vertex of a given triangle.
	 * \param indexVertex 0..2 Counter-clockwise vertex.
	 * \return Vertex
	 */
	const Vertex& GetTriangleVertex(const size_t indexTriangle,
			uint_fast8_t indexVertex) const;

	const Edge& GetEdge(const size_t index) const;
	Edge& GetEdge(const size_t index);

	const Triangle& GetTriangle(const size_t index) const;
	Triangle& GetTriangle(const size_t index);

	/**\}
	 */

	/**\brief Transform all vertices by an AffineTransformMatrix
	 *
	 * \note The Transform-matrix of the hull is not modified.
	 *
	 * Only the vertices are manipulated.
	 */
	void Transform(const AffineTransformMatrix &transformMatrix);

	/**\brief Apply a function to transform the vertices
	 *
	 * A std::function that can transform a Vector3 to a Vector3 is applied
	 * to all vertices in the hull.
	 */
	void Transform(std::function<Vector3(Vector3)> func);

	/**\brief Apply the transformation matrix and reset it
	 *
	 * The transformation matrix in this Hull is applied to all vertices
	 * directly and then reset to the unit matrix.
	 *
	 * The output of the Paint() function stays the same, but all vertices
	 * moved to the transformed position in memory.
	 */
	void ApplyTransformationMatrix();

	/**
	 * \name Extract information
	 * \{
	 */

	/**\brief Centroid of a triangle
	 *
	 * Assuming the mass is distributed equally in the area of the triangle, the
	 * center/centroid of the triangle is also the center of gravity.
	 *
	 * \param idx Index of the triangle in the geometry
	 * \return Vector3 of the center of mass of the triangle.
	 */
	Vector3 GetTriangleCenter(size_t idx) const;

	/**\brief Area of a triangle
	 *
	 * This is used to
	 *
	 * \param idx Index of the triangle
	 * \return Area of the triangle
	 */
	double GetTriangleArea(size_t idx) const;

	/**\brief Centroid of the tetraeder of a triangle and the center
	 *
	 * A triangle together with the center form a tetraeder. Together with the
	 * volume of the tetraeder the mass distribution of the geometry can be
	 * analyzed.
	 *
	 * Assuming the mass is distributed equally in the volume of the triangle, the
	 * center/centroid of the triangle is also the center of gravity.
	 *
	 * \param idx Index of the triangle in the geometry
	 * \return Vector3 of the center of mass of the tetraeder.
	 */
	Vector3 GetTetraederCenter(size_t idx) const;

	double GetTetraederVolume(size_t idx) const;

	Vector3 GetCenterOfVertices() const; // Center of all vertices.
	Vector3 GetCenterOfMass() const; // Center of gravity of geometry.
	double GetArea() const;
	double GetVolume() const;

	/**\brief Sum of all angles at edges with two triangles.
	 *
	 * Used to determine, if the normals are pointing inward or outwards.
	 *
	 * \return Summed-up angle in radians.
	 */
	double GetNormalCurvature() const;

	/**\brief Cut the geometry by a plane.
	 *
	 * \note Only one outline is returned. If there a multiple loops in the
	 * cutting plane, this function might return the wrong loop, as it only
	 * returns the first loop it finds.
	 *
	 * \param n Normal vector of the plane. Should have unit-length.
	 * \param d Distance of the plane to the origin.
	 * \return A Polygon3 with the cut outline.
	 */
	Polygon3 IntersectPlane(const Vector3 &n, double d) const;

	/**\brief Intersect the surface from a starting point in a given direction
	 *
	 * \param p0 Point where the search starts
	 * \param dir Vector3 in the direction to search in
	 * \return Vector3 of point of intersection
	 */
	Vector3 IntersectArrow(const Vector3 &p0, const Vector3 &dir) const;

	/**\}
	 * \name Selecting
	 * \{
	 */

	size_t Select(const std::set<size_t> &select);
	size_t SelectAll();
	size_t UnselectAll();
	size_t SelectByPlane(const Vector3 &n, double d);
	size_t UnselectByPlane(const Vector3 &n, double d);
	size_t SelectByNormal(const Vector3 &n, double limit = 0.0);
	size_t UnselectByNormal(const Vector3 &n, double limit = 0.0);
	size_t SelectByGroup(size_t group);
	size_t UnselectByGroup(size_t group);
	size_t CountSelected() const;

	/**\}
	 * \name Painting
	 * \{
	 */

	void Paint() const;
	void PaintTriangles(const std::set<size_t> &sel = std::set<size_t>(),
			bool invert = true) const;
	void PaintEdges(const std::set<size_t> &sel = std::set<size_t>(),
			bool invert = true) const;
	void PaintVertices() const;
	void PaintSelected() const;

	void SendToGLVertexArray(const std::string fields);
	void DeleteGLVertexArray();
	void Bind();

	/**\}
	 */

protected:
	void InitMap(); //< Initializes the maps to no-operation mappings.
	void FlipMap(); //< Sorting leaves the map inverted mapping a->b instead of b->a.

protected:
	inline static void GLVertex(const Vector3 &v_);
	inline static void GLNormal(const Vector3 &n);
	inline static void GLColor(const Color &c);

public:
	std::string name;
	AffineTransformMatrix matrix = AffineTransformMatrix::Identity(); ///< Transformation of the data.

	bool smooth = false;
	bool paintEdges = false;
	bool paintTriangles = true;
	bool paintVertices = false;
	bool paintNormals = false;
	bool paintDirection = false;
	bool paintSelected = false;

	size_t dotSize = 0; ///< If > 0, dots (GL_POINTS) of this size are shown at the vertices

protected:
	std::vector<Vertex> v = { }; ///< Vertices
	std::vector<Edge> e = { }; ///< Edges
	std::vector<Triangle> t = { }; ///< Triangles

	bool verticesHaveNormal = false;
	bool verticesHaveColor = false;
	bool verticesHaveTextur = false;
	bool edgesHaveNormal = false;
	bool edgesHaveColor = false;
	bool trianglesHaveNormal = false;
	bool trianglesHaveColor = false;
	bool trianglesHaveTexture = false;

	bool finished = false;

	/**\brief Maps the inserted vertices to the internal indices
	 *
	 * Triangles are stored in many file formats as a list of vectors and a
	 * list of triangles referencing these vertices. Because vertices are
	 * joined together upon insertion, the indices change and are mapped to the
	 * new positions in the vmap, emap and tmap.
	 */
	std::vector<size_t> vmap = { };
	std::vector<size_t> emap = { };
	std::vector<size_t> tmap = { };
	// While vmap, emap, tmap are vectors, the gmap is a map. The index of the
	// first three refers to a position of a v, e or t in a vector. The group
	// is an arbitrary probably non-continguous member value.
	std::map<size_t, size_t> gmap = { };

	std::set<size_t> selected = { }; ///< Selected Vertices

	/**\brief Minimum distance between vertices
	 *
	 * The distance is calculated in Manhattan-coordinates for speed reasons.
	 * If the distance in x, the distance in y and the distance in z is smaller
	 * or equal epsilon, the vertices are considered the same vertex. The first
	 * vertex in the list is used for the vertices close by.
	 */
	double epsilon = 1e-6;

	std::set<size_t> openvertices = { }; ///< Vertices not surrounded by triangles.
	std::set<size_t> openedges = { }; ///< Edges not yet assigned to two triangles.

	bool addNormals = false;
	Vector3 addNormal = { }; ///< Normal for new vertices, edges and triangles.
	bool addColors = false;
	Color addColor = { }; ///< Color for new vertices, edges and triangles.
	bool useAddMatrix = false;
	AffineTransformMatrix addMatrix = AffineTransformMatrix::Identity(); ///< Matrix used to modify newly added vertices.
	AffineTransformMatrix addNormalMatrix = AffineTransformMatrix::Identity(); ///< Matrix used to modify newly added normals.

	unsigned vertexArrayObject = 0;
	unsigned vertexBufferObject = 0;
	unsigned elementBufferObject = 0;
};

#endif /* L3D_GEOMETRY_H */
