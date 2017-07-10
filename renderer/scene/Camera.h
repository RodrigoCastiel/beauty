//  +-----------------------------------------------------+
//  |   File: Camera.h                                    |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: July 08, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include <cmath>
#include <glm/glm.hpp>

namespace renderer
{

class Camera
{
public:
    Camera() { }

    // Computes the line which goes from camera origin to mouse coords at projection plane.
    glm::vec3 CastRay(float x_v, float y_v, float w, float h) const;
    glm::vec3 GetCenterCoordinates() const { return mPos; }

    // Setters and getters.
    inline void SetPosition(const glm::vec3 & pos) { mPos = pos; }
    inline void SetRotation(const glm::vec3 & rot) { mRot = rot; }
    inline void SetScale(float scale) { mScale = glm::vec3(scale); }

    inline void SetFOVy(float fovy) { mFovy = fovy; }
    inline float GetFOVy() { return mFovy; }

private:
    // View parameters.
    glm::vec3 mPos{ 0.0, 0.0, 0.0 };  // Center coordinates.
    glm::vec3 mRot{ 0.0, 0.0, 0.0 };  // Rotation angles.
    glm::vec3 mScale{ 1.0, 1.0, 1.0 };  // Scales.

    // Projection parameters
    float mFovy{ 3.141592f / 3.0f };
    float mFarZ{ 10000.0f };
    float mNearZ{ 0.01f };
};

}  // namespace renderer.