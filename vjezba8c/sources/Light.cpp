#include "Light.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Light::Light(glm::vec3 pos, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 reflective) : Transform(pos.x, pos.y, pos.z)
{
    float matrix[12] =
            {
                    ambient.x, ambient.y, ambient.z,
                    diffuse.x, diffuse.y, diffuse.z,
                    reflective.x, reflective.y, reflective.z
            };
    this->lightMatrix = glm::make_mat3x3(matrix);
}

Light::Light(glm::vec3 pos) : Transform(pos.x, pos.y, pos.z)
{
    float matrix[12] = {
            0.3f, 0.3f, 0.3f,
            1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f
    };
    this->lightMatrix = glm::make_mat3x3(matrix);
}

glm::mat4x4 Light::getLightMatrix() { return this->lightMatrix; }
