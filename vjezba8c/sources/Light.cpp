#include "Light.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Light::Light(float x, float y, float z, float Iar, float Iag, float Iab, float Idr, float Idg, float Idb, float Irr, float Irg, float Irb) : Transform(x, y, z)
{
    float matrix[12] = 
    {
        Iar, Iag, Iab,
        Idr, Idg, Idb,
        Irr, Irg, Irb
    };
    this->lightMatrix = glm::make_mat3x3(matrix);
}

glm::mat4x4 Light::getLightMatrix() { return this->lightMatrix; }