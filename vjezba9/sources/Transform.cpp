#include "Transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

glm::mat4 Transform::translate3D(glm::vec3 tv)
{
    const float translation[16] =
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            tv.x, tv.y, tv.z, 1.0f};
    return glm::make_mat4(translation);
}

glm::mat4 Transform::scale3D(glm::vec3 tv)
{
    const float scale[16] =
        {
            tv.x, 0.0f, 0.0f, 0.0f,
            0.0f, tv.y, 0.0f, 0.0f,
            0.0f, 0.0f, tv.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};
    return glm::make_mat4(scale);
}

glm::mat4 Transform::lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 viewUp)
{
    glm::vec3 f = glm::normalize(center - eye);
    glm::vec3 u = glm::normalize(viewUp);
    glm::vec3 s = glm::normalize(cross(f, u));
    u = glm::cross(s, f);
    const float lookAt[16] =
        {
            s.x, u.x, -f.x, 0.0f,
            s.y, u.y, -f.y, 0.0f,
            s.z, u.z, -f.z, 0.0f,
            -glm::dot(s, eye), -glm::dot(u, eye), glm::dot(f, eye), 1.0f};
    return glm::make_mat4(lookAt);
}

glm::mat4 Transform::frustum(float l, float r, float b, float t, float n, float f)
{
    const float frustumMatrix[16] =
        {
            2 * n / (r - l), 0.0f, 0.0f, 0.0f,
            0.0f, 2 * n / (t - b), 0.0f, 0.0f,
            (r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1.0f,
            0.0f, 0.0f, -(2 * f * n) / (f - n), 1.0f};
    return glm::make_mat4(frustumMatrix);
}

Transform::Transform(float x, float y, float z)
{
    this->setPosition(glm::vec3(x, y, z));
}

void Transform::setPosition(glm::vec3 pos)
{
    this->position = pos;
    this->modelMatrix = this->modelMatrix * Transform::translate3D(pos);
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
    this->modelMatrix = this->modelMatrix * Transform::scale3D(glm::vec3(x, y, z));
}

glm::vec3 Transform::getEyePosition(glm::mat4 view)
{
    return glm::vec3(view[3][0], view[3][1], view[3][2]);
}