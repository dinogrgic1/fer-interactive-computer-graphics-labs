#include "Material.hpp"
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Material::Material(const aiScene *scene, char *argv[])
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

        std::string path(argv[0]);
        std::string dirPath(path, 0, path.find_last_of("\\/"));
        std::string resPath(dirPath);
        resPath.append("//resources"); //za linux pretvoriti u forwardslash

        aiString texturePosition;
        if (AI_SUCCESS == scene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePosition)) {
            dirPath.append("/resources/glava/textures/_Diffuse.png");
            this->data = stbi_load(dirPath.c_str(), &this->width, &this->height, &this->nrChannels, 0);
            std::cout << dirPath << std::endl;
        }

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