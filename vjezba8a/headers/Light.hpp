#pragma once


#include <glm/glm.hpp>
#include "Transform.hpp"

class Light : public Transform
{
private:
    glm::mat3x3 lightMatrix;

public:
    Light(float, float, float, float, float, float, float, float, float, float, float, float);
    glm::mat4x4 getLightMatrix();
};