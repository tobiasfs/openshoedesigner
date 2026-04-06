Project
=======

# Project.h / Project.cpp

The Project class contains the document, that is edited by the software. It contains objects for the foot-measurements, the upper-design, and various parameter for the way the shoe is build:

```
	Configuration config;
	FootMeasurements footL;
	FootMeasurements footR;
	std::shared_ptr<Design> design;
```

(The classes Configuration and FootMeasurements contain the shared_ptr<...> to the actual parameters. These classes are used as containers.)


The class also contains the `shared_ptr<...>` to all the objects, that are modified by the Builder. After a `builder.Update()` these objects contain all the parts, that make up the shoe.

The parameter- and the design-classes are stored in the *configuration* folder.

# ProjectView.h / ProjectView.cpp

The ProjectView class takes care of rendering the Project object to screen. It also stores what views are shown in the GUI. This is used by the builder to decide, what needs to be generated.

# Builder.h / Builder.cpp

The Builder class uses the data in Project to generate the shoe. Checks, what needs to be build from the ProjectView. Checks, what has been modified using the Modify(...) and IsModified() mechanism in the Parameter and Design objects. Only regenerates, what is really needed. Keeps a tree of operations to check dependencies.

---

# Subfolder

## command

The Project is not modified directly but by using command objects, that manipulate the project configuration (Model-View-Controller pattern). This enables Undo/Redo operation.

## configuration

Classes for the parameter and for the design. Used in shared_ptr<...> in the Project.h. 

## foot

Skeleton defininition for a bone-based foot design. Not used ATM.

## object

Classes for all the objects generated or loaded during the generation of the shoe.

## operation

Operations on the objects. Orchestrated by the Builder.

---

# Dataflow 
This section is a scratchpad to think about the rather complicated mechanisms.

## Design

The design to flattening flow is the most complicated in the building of the shoe.
The design is first solved, then it is mapped onto the `Surface` `CoordinateSystem', the result is flattened.

### Object: Design

The design has four public containers (std::vector): vertices, edges, constraints, patches. The vertices are moved around by the user. The positions of the vertices are constrained in different ways (lines, angles, or symmetry). The certices are collected into edges (Bezier edges: All vertices except the first and last are used as control vertices.). The edges are collected into patches.

### Operation: DesignSolve

`design::Update()` solves the positions of the vertices and updates two protected container with intersection points and the edge pieces between these intersection points.

The DesignSolve operation collects the edge pieces for a patch and constructs the outline (with cutouts) of the patch. The constructed patches are piecewise linear, i.e. The Bezier Curves are interpolated.

### Object: DesignSolution

The DesignSolution is used together with the Design in the pattern-tab of the editor. This is still in the 2D space. All coordinates are U,V only.

### Operation: UpperConstruct

The CoordinateSystem, that was generated from the Last, the FootModel, or generated directly, is merged with the DesignSolution. Here it is important to note, that some of the straight lines in 2D are now curved lines in 3D. These lines need an additional interpolation to follow the curves of the surface exactly.

The bulk of the operation is the generation of a surface out of the outline. 

In 2D: The most of the patches is filled with a regular triangle grid. The gap to the outline is triangulated. The resulting mesh is Delaunay regularized. 

The 2D coordinates of all vertices are then mapped onto the coordinate system to generate x, y, z, n.x, n.y, n.z.

### Object: Upper

The upper is a shaped onto the surface/coordinate system of the foot. By using the normal vector of the vertices, thick and overlapping pieces can be rendered.

The upper is not necessarily flattenable without stretching.

### Operation: FlattenUpper

The length of the edges in 3D are determined. The border of the patches is flattened so, that the length of the outer-edges is exactly the same as in the 3D representation. The deviation of the angles between two edges is minimized. This is done separately for the outer edge and for the inner cutouts.

The shape of the cutouts and of the outer edge is frozen. The inner vertices and edges are added. The length of the inner edges is brought as close as possible to the length of the edges in 3D by moving the vertices and the cutouts (as a whole without changing the shape).

This results in the flattened patches.

### Object: Flattening

2D patches used for displaying in the flattening-tab of the editor and for exporting to SVG images for further processing (plotting, laser-cutting, drag-knive cutting, ...). These might later be also used in the test-stitch-tab to check if the flattening is reasonable, if it is assembled into the final upper.


 

 


