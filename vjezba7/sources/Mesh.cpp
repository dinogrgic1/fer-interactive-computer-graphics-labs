#include "Mesh.hpp"
#include "Transform.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <glm/glm.hpp>
#include <algorithm>
#include <iostream>

void Mesh::setMax(glm::vec3 vertex)
{
    this->max.x = (vertex.x > this->min.x) ? vertex.x : this->max.x;
    this->max.y = (vertex.y > this->min.y) ? vertex.y : this->max.y;
    this->max.z = (vertex.z > this->min.z) ? vertex.z : this->max.z;
}

void Mesh::setMin(glm::vec3 vertex)
{
    this->min.x = (vertex.x < this->min.x) ? vertex.x : this->min.x;
    this->min.y = (vertex.y < this->min.y) ? vertex.y : this->min.y;
    this->min.z = (vertex.z < this->min.z) ? vertex.z : this->min.z;
}

std::pair<glm::vec3, glm::vec3> Mesh::getBoundingBox()
{
    return std::make_pair(this->min, this->max);
}

Mesh::Mesh(aiMesh *mesh)
{
    glm::vec3 t = glm::vec3(mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z);
    this->min = this->max = glm::vec3(t);

    this->vertices = std::vector<glm::vec3>();
    this->vertices.push_back(t);
    for (int i = 1; i < mesh->mNumVertices; i++)
    {
        t = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        this->vertices.push_back(t);
        this->setMin(t);
        this->setMax(t);
    }

    this->indeces = std::vector<int>();
    this->normals = std::vector<glm::vec3>();
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        this->indeces.push_back(mesh->mFaces[i].mIndices[0]);
        this->indeces.push_back(mesh->mFaces[i].mIndices[1]);
        this->indeces.push_back(mesh->mFaces[i].mIndices[2]);
    }

    this->textureCords = std::vector<glm::vec2>();
    for (int i = 0; i < mesh->mVertices->Length(); i++)
        this->textureCords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));

    this->applyTransform();
}

Mesh::Mesh(std::vector<glm::vec3> vertex)
{
    if (vertex.size() == 0)
    {
        return;
    }
    glm::vec3 t = glm::vec3(vertex[0]);
    this->min = this->max = glm::vec3(t);
    this->vertices = std::vector<glm::vec3>();
    this->vertices.push_back(t);
    this->indeces = std::vector<int>();
    this->indeces.push_back(1);
    this->indeces.push_back(2);

    for (int i = 1; i < vertex.size(); i++)
    {
        t = glm::vec3(vertex[i]);
        this->vertices.push_back(t);
        this->setMin(t);
        this->setMax(t);
        this->indeces.push_back(i + 1);
        this->indeces.push_back(i + 2);
    }
    //this->applyTransform();
}

void Mesh::updateMesh(glm::vec3 v)
{
    this->vertices.push_back(v);
    this->setMax(v);
    this->setMin(v);
    if (this->vertices.size() >= 3)
        this->applyTransform();
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
    glm::mat4 scaleMatrix = Transform::scale3D(glm::vec3(M, M, M));
    glm::mat4 translationMatrix = Transform::translate3D(glm::vec3(-x_avg, -y_avg, -z_avg));

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