#include <utility>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

class Material
{
    private:
        glm::mat3x3 materialProps;
        int width, height, nrChannels;
        unsigned char *data;
public:
        Material(const aiScene*, char **);
        glm::mat3x3 getMaterialProps();
        void print();

        int getWidth();
        int getHeight();
        int getChannels();
        unsigned char *getData();
};

