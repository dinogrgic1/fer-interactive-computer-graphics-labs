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
        std::vector<int> indeces;
        glm::vec3 min;
        glm::vec3 max;

        void setMin(glm::vec3);
        void setMax(glm::vec3);
        void applyTransform();
        
    public:
        Mesh(aiMesh*);
        
        std::pair<glm::vec3, glm::vec3> getBoundingBox();
        std::vector<glm::vec3> getVertices();
        std::vector<int> getIndeces();
};