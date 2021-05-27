#include <iostream>
#include "Camera.hpp"

Camera::Camera(float x, float y, float z) : Transform(x, y, z)
{
    this->target = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 Camera::getTarget() { return this->target; }

void Camera::targetSet(float xOffset, float yOffset)
{
    target = glm::rotate(glm::mat4(1.0f), (float) glm::radians(10.0), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(target, 1.0f);
    target = glm::rotate(glm::mat4(1.0f), (float) glm::radians(10.0), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(target, 1.0f);
}
