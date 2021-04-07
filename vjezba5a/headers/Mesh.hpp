#include <utility>
#include <vector>

#include <glm/glm.hpp>

class Mesh
{
    private:
        std::vector<glm::vec3> vertices;
        std::vector<int> indeces;
        
    public:
        Mesh(std::vector<glm::vec3>, std::vector<int>);
        std::pair<glm::vec3, glm::vec3> getBoundingBox();
        void applyTransform();
        void addVertex(glm::vec3);

        std::vector<glm::vec3> getVertices();
        std::vector<int> getIndeces();
};