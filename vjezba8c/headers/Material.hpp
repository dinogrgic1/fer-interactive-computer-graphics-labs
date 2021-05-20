#include <utility>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

class Material
{
    public:
        glm::mat3x3 materialProps;
    public:
        Material(const aiScene*);
        glm::mat3x3 getMaterialProps();
        void print();
};

