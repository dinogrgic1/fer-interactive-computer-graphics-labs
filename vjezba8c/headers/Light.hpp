#pragma once


#include <glm/glm.hpp>
#include "Transform.hpp"

class Light : public Transform
{
private:
    glm::mat3x3 lightMatrix;

public:
    Light(glm::vec3);
    Light(glm::vec3, glm::vec3, glm::vec3, glm::vec3);
    glm::mat4x4 getLightMatrix();
};