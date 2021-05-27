#include <vector>

#include "Transform.hpp"
#include "Object.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Curve
{
    public:
    Object *controlPolygon;
    int size1 = 0;
    int size2 = 0;

    std::vector<glm::vec3> getInterpolated();
    std::vector<glm::vec3> getBezier();
    std::vector<glm::vec3> getControl();

    Curve(std::vector<glm::vec3>);
    Curve();
    glm::mat4x3 drawInterpolatedBezier(std::vector<glm::vec3> controlPolygon);

    ~Curve();
};
