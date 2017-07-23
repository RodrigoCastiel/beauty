//  +-----------------------------------------------------+
//  |   File: Primitives.h                                |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: July 08, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include <glm/glm.hpp>

namespace renderer
{

struct Triangle
{
    glm::vec3 v[3];
};

struct TriangleAttrib
{
    // Data is stored as a matrix so it is faster to interpolate using barycentric coordinates.
    glm::mat3 Kd;  // Each column corresponds to a vertex diffuse  material.
    glm::mat3 Ks;  // Each column corresponds to a vertex specular material.
    glm::mat3 n;   // Each column corresponds to a vertex normal.
    glm::vec3 shininess;  // Each element corresponds to a vertex shininess.
    float n_refr;         // Index of refraction.
};

struct Sphere  // Sphere intersection data.
{
    glm::vec3 pos;
    float radius;
};

struct SphereAttrib  // Sphere material attributes.
{
    glm::vec3 Kd;  // Material diffuse.
    glm::vec3 Ks;  // Material specular.
    float shininess;  // Alpha component.
    float n_refr;  // Index of refraction.
};

struct BoundingBox
{
    glm::vec3 mMin;  // (xmin, ymin, zmin).
    glm::vec3 mMax;  // (xmax, ymax, zmax).

    // Splits bounding box orthogonally to axis at 'value'.
    // Assumes that  xmin < value < xmax.
    // If side > 0, returns the greater-than bb.
    // Returns the less-than bb, otherwise.
    BoundingBox Split(float value, int axis, int side) const;
    void Print() const;
};


}  // namespace renderer.
