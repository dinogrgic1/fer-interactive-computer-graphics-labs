#include <tuple>
#include <vector>

class Mesh
{
    private:
        std::vector<std::tuple<float, float, float>> vertices;
        std::vector<float> indeces;
        
    public:
        Mesh(std::vector<std::tuple<float, float, float>>, std::vector<float>);
        std::tuple<float, float, float, float, float, float> getBoundingBox();
        void applyTransform();
        void addVertex(std::tuple<float, float, float>);
};