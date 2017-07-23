//  +-----------------------------------------------------+
//  |   File: Scene.h                                     |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: July 06, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include <string>
#include <vector>

#include "Camera.h"
#include "../scene/KdTree.h"
#include "../geometries/Primitives.h"
#include "../geometries/MathFunctions.h"

// class Scene stores all scene, including triangles, spheres, light
// sources and camera.
// It has methods for intersection, querying and .obj file loading.

namespace renderer
{

struct Light
{
    glm::vec3 pos;
    glm::vec3 col;
};

class Scene
{
public:
    // Constructor.
    Scene();

    // +++ Main Methods +++ ------------

    // Loads (parses) scene from a file.
    // Returns TRUE if scene was successfully loaded.
    // Returns FALSE otherwise.
    // Error messages are written into 'outputMessage'.
    bool Load(const std::string & filePath, std::string & outputMessage);

    // Returns the index of the nearest triangle reachable from input ray.
    // If there is a valid triangle, it also outputs barycentric coordinates,
    // depth and intersection point.
    int NearestTriangle(const glm::vec3 & r, const glm::vec3 & O,
        glm::vec3 & intersection, glm::vec3 & baryCoord, float & t) const;

    // Returns the index of the nearest sphere reachable from input ray.
    // If there is a valid sphere, it also outputs the intersection and
    // the normal vector at intersection.
    int NearestSphere(const glm::vec3 & r, const glm::vec3 & O,
        glm::vec3 & intersection, glm::vec3 & n, float & t) const;

    // Computes per-fragment Phong Lighting given the current scene elements.
    glm::vec3 ComputePhongIllumination(const glm::vec3 & f_pos, const glm::vec3 & n, 
        const glm::vec3 & Kd, const glm::vec3 & Ks, float alpha) const;

    bool LoadObj(const std::string & objFilepath);

    // Prints a log containing general Scene data.
    void Log(std::ostream & stream);

    // +++ Getters / Setters +++ ----------

    // Returns camera.
    const Camera & GetCamera() const { return mCamera; }

    // Returns triangle at 'index'.
    const Triangle & GetTriangle(int index) const { return mTriangles[index]; }

    // Returns attributes of triangle at 'index'.
    const TriangleAttrib & GetTriangleAttrib(int index) const { return mTriangleAttribList[index]; }

    // Returns sphere at 'index'.
    const Sphere & GetSphere(int index) const { return mSpheres[index]; }

    // Returns attributes of sphere at 'index'.
    const SphereAttrib & GetSphereAttrib(int index) const { return mSphereAttribList[index]; }

    // Returns background color (when there is no intersection for a given pixel).
    const glm::vec4 GetBackgroundColor() const { return mBackgroundColor; }

    // Sets background color.
    void SetBackgroundColor(const glm::vec4 & color) { mBackgroundColor = color; }

    void SetUseKdTree(bool on) { mUseKdTree = on; }

private:
    // Internal methods.
    void BuildUpSpatialTree();

private:
    // Optimization.
    bool mUseKdTree;

    // Camera.
    Camera mCamera;

    // Relaxed 3D-tree for spatially storing triangle indices.
    KdTree* mSpatialTree;

    // List of all triangles 
    // and list of all corresponding attributes.
    std::vector<Triangle> mTriangles;
    std::vector<TriangleAttrib> mTriangleAttribList;

    // List of all spheres and list of all corresponding attributes.
    std::vector<Sphere> mSpheres;
    std::vector<SphereAttrib> mSphereAttribList;

    // List of all light sources.
    std::vector<Light> mLights;
    glm::vec3 mAmbLight;
    glm::vec4 mBackgroundColor;
};


}  // namespace renderer.