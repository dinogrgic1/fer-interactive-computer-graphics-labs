#include "Transform.hpp"

Transform::Transform(float x, float y, float z)
{
    this->setPosition(glm::vec3(x, y, z));
}

void Transform::setPosition(glm::vec3 pos)
{
    this->position = pos;
    this->modelMatrix = glm::translate(this->modelMatrix, glm::vec3(pos.x, pos.y, pos.z));
}

void Transform::move(glm::vec3 pos)
{
    this->setPosition(pos);
}

void Transform::rotate(float angle, glm::vec3 direction)
{
    this->modelMatrix = glm::rotate(this->modelMatrix, angle, direction);
}

void Transform::scale(float x, float y, float z)
{
    this->modelMatrix = glm::scale(this->modelMatrix, glm::vec3(x, y, z));
}