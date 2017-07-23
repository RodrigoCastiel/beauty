//  +-----------------------------------------------------+
//  |   File: MathFunctions.h                             |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: July 08, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include <vector>
#include "Primitives.h"

namespace renderer
{

const double kEpsilon = 1e-3;

// in triangle: triangle in 3D-space.
// in ray: unit direction of ray.
// in O:   ray origin (3D-point).
// out intersection:    intersection point.
// out baryCoord:       barycentric coordinates of intersection.
// out t:               parametric value of ray that leads to the intersection.
// Returns TRUE if there was an intersection;
// Returns FALSE, otherwise.
bool IntersectionRayTriangle(const Triangle & triangle, const glm::vec3 & ray, const glm::vec3 & O,
                             glm::vec3 & intersection, glm::vec3 & baryCoord, float & t);

// in sphere: sphere in 3D-space.
// in ray: unit direction of ray.
// in O:   ray origin (3D-point).
// out intersection:    intersection point.
// out t:               parametric value of ray that leads to the intersection.
// Returns TRUE if there was an intersection;
// Returns FALSE otherwise.
bool IntersectionRaySphere(const Sphere & sphere, const glm::vec3 & r, const glm::vec3 & O,
                           glm::vec3 & intersection, glm::vec3 & n, float & t);


// Returns TRUE if triangle has an intersection with input half-space.
// Returns FALSE otherwise.
bool IntersectionHalfSpaceTriangle(const Triangle & triangle, float value, int axis, int type);


bool IntersectionRayBB(const glm::vec3 & r, const glm::vec3 & O, const BoundingBox & bb);



bool SubdivideTriangle(const Triangle & triangle, const TriangleAttrib& attrib, float value, int axis,
                       std::vector<Triangle> & newTriangles, std::vector<TriangleAttrib> & newAttribs);

// Computes the variance along x, y and z axis for the subset specified by indices.
glm::vec3 SubsetVariance(const std::vector<int> & subset, const std::vector<glm::vec3> & points);

}  // namespace renderer.