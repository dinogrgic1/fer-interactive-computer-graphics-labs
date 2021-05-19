#include "Curve.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Curve::Curve(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3, glm::vec4 v4)
{
    this->controlPolygon = std::vector<glm::vec3>();
    this->controlPolygonAdjList = std::vector<int>();
    this->controlPolygon.push_back(glm::vec3(v1.x, v1.y, v1.z));
    this->controlPolygon.push_back(glm::vec3(v2.x, v2.y, v2.z));
    this->controlPolygonAdjList.push_back(0);
    this->controlPolygonAdjList.push_back(1);
    this->controlPolygon.push_back(glm::vec3(v3.x, v3.y, v3.z));
    this->controlPolygonAdjList.push_back(1);
    this->controlPolygonAdjList.push_back(2);
    this->controlPolygon.push_back(glm::vec3(v4.x, v4.y, v4.z));
    this->controlPolygonAdjList.push_back(2);
    this->controlPolygonAdjList.push_back(3);
}

std::vector<glm::vec3> Curve::getControlPolygon() 
{ 
    return this->controlPolygon; 
}

std::vector<int> Curve::getControlPolygonAdjList() 
{ 
    return this->controlPolygonAdjList; 
}

glm::vec3 drawBezier(glm::vec3 PT[], double t)
{
    glm::vec3 P;
    P.x = pow((1 - t), 3) * PT[0].x + 3 * t * pow((1 - t), 2) * PT[1].x + 3 * (1 - t) * pow(t, 2) * PT[2].x + pow(t, 3) * PT[3].x;
    P.y = pow((1 - t), 3) * PT[0].y + 3 * t * pow((1 - t), 2) * PT[1].y + 3 * (1 - t) * pow(t, 2) * PT[2].y + pow(t, 3) * PT[3].y;
    P.z = pow((1 - t), 3) * PT[0].z + 3 * t * pow((1 - t), 2) * PT[1].z + 3 * (1 - t) * pow(t, 2) * PT[2].z + pow(t, 3) * PT[3].z;
    return P;
}
