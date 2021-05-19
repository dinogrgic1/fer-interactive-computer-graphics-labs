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
        int i = 0;
        glm::vec3 ambientColor;
        aiColor3D ambientK, diffuseK, specularK, reflectiveK, emissiveK;
        
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientK);
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseK);
        scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularK);

         float mat[12] {
            ambientK.r, ambientK.g, ambientK.b,
            diffuseK.r, diffuseK.g, diffuseK.b,
            specularK.r, specularK.g, specularK.b,
        };

        this->materialProps = glm::make_mat3x3(mat);
    }
}

glm::mat3x3 Material::getMaterialProps()
{
    return this->materialProps;
}