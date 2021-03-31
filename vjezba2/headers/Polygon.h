#include "PolgyonEelement.h"

#include <vector>

class Polygon
{
private:
    std::vector<PolygonElement> Elements;
    bool clockwise = false;

    std::tuple<int, int, int, int> getMinMax();

public:
    Polygon();
    ~Polygon();

    void addVertex(int, int);
    void popVertex();
    bool isConvex();
    bool dotIsInside(int, int);

    void setClockwise();
 
    std::vector<PolygonElement> getElements();
    void draw(std::vector<std::pair<int, int>>&);
    int getVerteciesNumber();
};