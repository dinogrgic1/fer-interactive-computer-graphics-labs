#include <utility>
#include <tuple>

class PolygonElement
{
private:
    std::pair<int, int> Vertex;
    std::tuple<int, int, int> Edge;
    int left = false;

public:
    PolygonElement();
    PolygonElement(int x, int y);
    ~PolygonElement();

    std::tuple<int, int, int> getEdge();
    void setEdge(std::tuple<int, int, int> edge);
    int getEdgeA();
    int getEdgeB();
    int getEdgeC();
    std::tuple<int, int, int> calculateEdge(PolygonElement);

    std::pair<int, int> getVertex();

    int getLeft();    
};