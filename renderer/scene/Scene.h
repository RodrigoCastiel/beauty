//  +-----------------------------------------------------+
//  |   File: Scene.h                                     |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: July 06, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include <string>
#include <vector>

#include "Camera.h"
#include "../geometries/Primitives.h"
#include "../geometries/MathFunctions.h"

// class Scene stores all scene, including triangles, spheres, light
// sources and camera.
//
// In this version, there is no spatial data structure optimization.

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
    Scene() 
    {
        mCamera = new Camera();
    }

    // Loads scene from a file.
    bool Load(const std::string & filePath);

    // Loads .obj from.
    //bool LoadObj(const std::string & objFilePath);

    // TraceRay - returns a RGB vec3 containing the color of intersection.
    glm::vec3 TraceRay(const glm::vec3 & r, const glm::vec3 & O, int depth = 3) const;

    // Returns the index of the nearest triangle reachable from input ray.
    // If there is a valid triangle, it also outputs barycentric coordinates,
    // depth and intersection point.
    // If there is no triangle which intersects with input ray, return value
    // will be 0.
    int NearestTriangle(const glm::vec3 & r, const glm::vec3 & O,
        glm::vec3 & intersection, glm::vec3 & baryCoord, float & t) const;

    int NearestSphere(const glm::vec3 & r, const glm::vec3 & O,
        glm::vec3 & intersection, glm::vec3 & n, float & t) const;

    //
    glm::vec3 ComputePhongIllumination(const glm::vec3 & f_pos, const glm::vec3 & n, 
        const glm::vec3 & Kd, const glm::vec3 & Ks, float alpha) const;

    // Prints a log containing general Scene data.
    void Log(std::ostream & stream);

    // Returns camera.
    Camera* GetCamera() const { return mCamera; }

private:
    // Camera.
    Camera* mCamera { nullptr };

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
    glm::vec3 mBackgroundColor { 0.0f, 0.0f, 0.0f };
};


}  // namespace renderer.