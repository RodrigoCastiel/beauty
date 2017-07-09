#include "Camera.h"

#include <glm/gtx/transform.hpp>

namespace renderer
{

glm::vec3 Camera::CastRay(float x_v, float y_v, float w, float h) const
{
    // Compute boundaries of near clip plane.
    float x_max = 1.0;
    float y_max = 1.0;

    // Convert viewport coordinates to projection plane coordinates.
    y_v = (h - y_v);
    float xp = ((x_v / w) * (2 * x_max)) - x_max;
    float yp = ((y_v / h) * (2 * y_max)) - y_max;
    float aspect = static_cast<float>(w)/h;

    const glm::dmat4 P = glm::perspective(mFovy, aspect, mNearZ, mFarZ);

    const glm::dmat4 V =  glm::translate(-mPos)
                        * glm::rotate(-mRot[2], glm::vec3(0, 0, 1))
                        * glm::rotate(-mRot[0], glm::vec3(1, 0, 0))
                        * glm::rotate(-mRot[1], glm::vec3(0, 1, 0))
                        * glm::scale(mScale);

    // const glm::dmat4 V = glm::lookAt(glm::vec3(mPos), glm::vec3(0), glm::vec3(0, 1, 0));

    // 'Unproject' point to  xp, yp, z = 1, w = 1. 
    glm::vec4 ray = glm::inverse(P*V) * glm::dvec4(xp, yp, 1.0, 1.0);

    return glm::normalize(glm::vec3(ray[0], ray[1], ray[2]));
}

}  // namespace renderer.