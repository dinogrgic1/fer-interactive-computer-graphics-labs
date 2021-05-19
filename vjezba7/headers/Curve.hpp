#include <vector>

#include "Transform.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Curve
{
    private:
    std::vector<glm::vec3> controlPolygon;
    std::vector<int> controlPolygonAdjList;

    public:
    Curve(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3, glm::vec4 v4);
    std::vector<glm::vec3> getControlPolygon();
    std::vector<int> getControlPolygonAdjList();
};