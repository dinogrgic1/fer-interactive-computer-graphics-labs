#pragma once

#include <utility>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Transform
{
    public:
    glm::vec3 position;
    glm::mat4 modelMatrix = glm::mat4(1);
    
    void setPosition(glm::vec3);
    void setOrientation(glm::vec3);

    void move(glm::vec3);
    void rotate(float, glm::vec3);
    void scale(float, float, float);

    Transform(float, float, float);

    static glm::mat4 lookAtMatrix(glm::vec3, glm::vec3, glm::vec3);
    static glm::mat4 scale3D(glm::vec3);
    static glm::mat4 translate3D(glm::vec3);
    static glm::mat4 frustum(float, float, float, float, float, float);
    static glm::vec3 getEyePosition(glm::mat4);
};