#include "Mesh.hpp"
#include "Transform.hpp"

#include <assimp/scene.h>
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

    this->normals = std::vector<glm::vec3>();
    this->textureCords = std::vector<glm::vec2>();

    this->normals.emplace_back(mesh->mNormals[0].x, mesh->mNormals[0].y, mesh->mNormals[0].z);
    this->textureCords.emplace_back(mesh->mTextureCoords[0][0].x, mesh->mTextureCoords[0][0].y);

    this->vertices = std::vector<glm::vec3>();
    this->vertices.push_back(t);
    for (int i = 1; i < mesh->mNumVertices; i++)
    {
        t = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        this->normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        this->vertices.push_back(t);
        this->setMin(t);
        this->setMax(t);
        this->textureCords.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }

    this->indeces = std::vector<int>();

    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        this->indeces.push_back(mesh->mFaces[i].mIndices[0]);
        this->indeces.push_back(mesh->mFaces[i].mIndices[1]);
        this->indeces.push_back(mesh->mFaces[i].mIndices[2]);
    }

    // calculate vertex normlas if they are not defined in .obj
    if (this->normals.size() == 0)
    {
        for (int i = 1; i <= this->vertices.size(); i++)
        {
            glm::vec3 tmp = glm::vec3();
            for (int j = 0; j < this->indeces.size(); j = j + 3)
            {
                if (this->indeces[j] == i || this->indeces[j + 1] == i || this->indeces[j + 2] == i)
                {
                    glm::vec3 v_x = this->vertices[this->indeces[j] - 1];
                    glm::vec3 v_x1 = this->vertices[this->indeces[j + 1] - 1];
                    glm::vec3 v_x2 = this->vertices[this->indeces[j + 2] - 1];

                    glm::vec3 normal = glm::cross((v_x1 - v_x), (v_x2, v_x));
                    tmp += normal;
                }
            }
            this->normals.push_back(glm::normalize(tmp));
        }
    }

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

    for (int i = 0; i < this->normals.size(); i++)
    {
        glm::vec4 curr = glm::vec4(normals[i], 1.0f);
        curr = scaleMatrix * translationMatrix * curr;

        this->normals[i].x = curr.x;
        this->normals[i].y = curr.y;
        this->normals[i].z = curr.z;
    }
}

std::vector<glm::vec3> Mesh::getVertices()
{
    return this->vertices;
}

std::vector<glm::vec3> Mesh::getVertexNormals()
{
    return this->normals;
}

std::vector<int> Mesh::getIndeces()
{
    return this->indeces;
}

std::vector<glm::vec2> Mesh::getUvCords()
{
    return this->textureCords;
}