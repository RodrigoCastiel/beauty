//  +-----------------------------------------------------+
//  |   File: MathFunctions.h                             |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: July 08, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

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
// Returns FALSE, otherwise.
bool IntersectionRaySphere(const Sphere & sphere, const glm::vec3 & r, const glm::vec3 & O,
                           glm::vec3 & intersection, glm::vec3 & n, float & t);

}  // namespace renderer.