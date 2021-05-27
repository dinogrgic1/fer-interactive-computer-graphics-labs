#pragma once

#include <utility>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

class Mesh
{
    private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textureCords;
        std::vector<int> indeces;
        glm::vec3 min;
        glm::vec3 max;

        void setMin(glm::vec3);
        void setMax(glm::vec3);
        void applyTransform();
        
    public:
        Mesh(aiMesh*);
        Mesh(std::vector<glm::vec3>);
        std::pair<glm::vec3, glm::vec3> getBoundingBox();
        std::vector<glm::vec3> getVertices();
        std::vector<int> getIndeces();
        void updateMesh(glm::vec3);
};