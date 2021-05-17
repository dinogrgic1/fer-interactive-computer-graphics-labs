#include "Mesh.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <glm/glm.hpp>
#include <algorithm>
#include <iostream>

std::pair<glm::vec3, glm::vec3> Mesh::getBoundingBox()
{
    glm::vec3 min = glm::vec3(this->vertices[0].x, this->vertices[0].y, this->vertices[0].z);
    glm::vec3 max = glm::vec3(this->vertices[0].x, this->vertices[0].y, this->vertices[0].z);

    for (int i = 1; i < this->vertices.size(); i++)
    {
        float x, y, z;
        x = this->vertices[i].x;
        y = this->vertices[i].y;
        z = this->vertices[i].z;

        if (x < min.x)
        {
            min.x = x;
        }

        if (x > max.x)
        {
            max.x = x;
        }

        if (y < min.y)
        {
            min.y = y;
        }

        if (y > max.y)
        {
            max.y = y;
        }

        if (z < min.z)
        {
            min.z = z;
        }

        if (x > max.x)
        {
            max.z = z;
        }
    }

    return std::pair<glm::vec3, glm::vec3>(min, max);
}

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<int> index)
{
    this->vertices = vertices;
    this->indeces = index;
    this->applyTransform();
}

void Mesh::addVertex(glm::vec3 t)
{
    this->vertices.push_back(t);
}

void Mesh::applyTransform()
{
    std::pair<glm::vec3, glm::vec3> minMax = this->getBoundingBox();
    glm::vec3 min = minMax.first;
    glm::vec3 max = minMax.second;

    float x_avg = (min.x + max.x) / 2;
    float y_avg = (min.y + max.y) / 2;
    float z_avg = (min.z + max.z) / 2;

    float M = std::max({max.x - min.x, max.y - min.y, max.z - min.z});
    float scale[16] =
    {
        2.0f / M,   0.0f,       0.0f,       0.0f,
        0.0f,       2.0f / M,   0.0f,       0.0f,
        0.0f,       0.0f,       2.0f / M,   0.0f,
        0.0f,       0.0f,       0.0f,       1.0f
    };
    glm::mat4 scaleMatrix = glm::make_mat4(scale);

    float translation[16] =
    {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        -x_avg,  -y_avg,  -z_avg, 1.0f
    };
    glm::mat4 translationMatrix = glm::make_mat4(translation);

    for (int i = 0; i < this->vertices.size(); i++)
    {
        glm::vec4 curr = glm::vec4(vertices[i], 1.0f);
        curr = scaleMatrix * translationMatrix * curr;

        this->vertices[i].x = curr.x;
        this->vertices[i].y = curr.y;
        this->vertices[i].z = curr.z;
    }
}

std::vector<glm::vec3> Mesh::getVertices()
{
    return this->vertices;
}
std::vector<int> Mesh::getIndeces()
{
    return this->indeces;
}