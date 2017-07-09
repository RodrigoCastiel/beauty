//  +-----------------------------------------------------+
//  |   File: Light.h                                     |
//  |   Author: Rodrigo Castiel                           |
//  |   Date: July 08, 2017.                              |
//  +-----------------------------------------------------+
#pragma once

#include <glm/glm.hpp>

namespace renderer
{

struct Light
{
    glm::vec3 pos;
    glm::vec3 col;
};

}  // namespace renderer.