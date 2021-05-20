#include "Material.hpp"
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Material::Material(const aiScene *scene)
{
    if (scene->HasMaterials())
    {
        int i = scene->mMeshes[0]->mMaterialIndex;
        glm::vec3 ambientColor;
        aiColor3D ambientK, diffuseK, reflectiveK;
        
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientK);
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK);
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, reflectiveK);

         float mat[12] {
            ambientK.r, ambientK.g, ambientK.b,
            diffuseK.r, diffuseK.g, diffuseK.b,
            reflectiveK.r, reflectiveK.g, reflectiveK.b,
        };

        this->materialProps = glm::make_mat3x3(mat);
    }
}

glm::mat3x3 Material::getMaterialProps()
{
    return this->materialProps;
}

void Material::print()
{
    std::cout << "Loaded material-----------" << std::endl;
    std::cout << "Ambient: " << this->materialProps[0][0] << " " << this->materialProps[0][1] << " " << this->materialProps[0][2] << std::endl;
    std::cout << "Diffuse: " << this->materialProps[1][0] << " " << this->materialProps[1][1] << " " << this->materialProps[1][2] << std::endl;
    std::cout << "Reflective: " << this->materialProps[2][0] << " " << this->materialProps[2][1] << " " << this->materialProps[2][2] << std::endl;
}