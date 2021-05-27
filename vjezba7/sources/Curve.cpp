#include "Curve.hpp"
#include "Transform.hpp"
#include "Object.hpp"
#include "Mesh.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Curve::Curve(std::vector<glm::vec3> ctrlPolygon)
{
    std::vector<glm::vec3> b = std::vector<glm::vec3>();
    glm::vec4 P1 = glm::vec4(ctrlPolygon[0], 1.0f);
    glm::vec4 P2 = glm::vec4(ctrlPolygon[1], 1.0f);
    glm::vec4 P3 = glm::vec4(ctrlPolygon[2], 1.0f);
    glm::vec4 P4 = glm::vec4(ctrlPolygon[3], 1.0f);

    b.emplace_back(ctrlPolygon[0]);
    b.emplace_back(ctrlPolygon[1]);
    b.emplace_back(ctrlPolygon[2]);
    b.emplace_back(ctrlPolygon[3]);

    glm::mat4x3 points = Curve::drawInterpolatedBezier(b);

    glm::vec4 P1x = glm::vec4(points[0][0], points[0][1], points[0][2], 1.0f);
    glm::vec4 P2x = glm::vec4(points[1][0], points[1][1], points[1][2], 1.0f);
    glm::vec4 P3x = glm::vec4(points[2][0], points[2][1], points[2][2], 1.0f);
    glm::vec4 P4x = glm::vec4(points[3][0], points[3][1], points[3][2], 1.0f);


    for(float f = 0.0f; f <= 1.00f; f += 0.001f)
    {
        glm::vec4 P = (float) pow((1-f), 3) * P1 +
                (float) (3.0f * pow((1-f), 2) * f) * P2 +
                (float) (3.0f * (1 - f) * pow(f, 2)) * P3 +
                (float) pow(f, 3) * P4;
        P = P / P.w;
        b.emplace_back(glm::vec3(P.x, P.y, P.z));
    }
    this->size1 = b.size();

    for(float f = 0.0f; f <= 1.00f; f += 0.001f)
    {
        glm::vec4 P = (float) pow((1-f), 3) * P1x +
                      (float) (3.0f * pow((1-f), 2) * f) * P2x +
                      (float) (3.0f * (1 - f) * pow(f, 2)) * P3x +
                      (float) pow(f, 3) * P4x;
        P = P / P.w;
        b.emplace_back(glm::vec3(P.x, P.y, P.z));
    }
    this->size2 = b.size();
    this->controlPolygon = new Object(new Mesh(b), new Transform(0.0f, 0.0f, 0.0f));
}

Curve::Curve()
{
    this->controlPolygon = new Object(new Mesh(std::vector<glm::vec3>()), new Transform(0.0f, 0.0f, 0.0f));
}

Curve::~Curve()
{
    delete this->controlPolygon;
}

glm::mat4x3 Curve::drawInterpolatedBezier(std::vector<glm::vec3> controlPolygon)
{
    float T_f[16] = {
            1.0f,       0.0f,       0.0f,       0.0f,
            8.0f/27.0f,    4/9.0f,     2/9.0f,     1.0f/27.0f,
            1.0f/27.0f,    2/9.0f,     4/9.0f,     8.0f/27.0f,
            0.0f,       0.0f,       0.0f,       1.0f
    };
    glm::mat4x4 T = glm::make_mat4x4(T_f);
    T = glm::inverse(T);

    glm::vec3 p0 = controlPolygon[0];
    glm::vec3 p1 = controlPolygon[1];
    glm::vec3 p2 = controlPolygon[2];
    glm::vec3 p3 = controlPolygon[3];

    float pMat[12] = {
            p0.x, p0.y, p0.z,
            p1.x, p1.y, p1.z,
            p2.x, p2.y, p2.z,
            p3.x, p3.y, p3.z
    };
    glm::mat4x3 P = glm::make_mat4x3(pMat);
    glm::mat4x3 points = P * T;
    return points;
}

std::vector<glm::vec3> Curve::getInterpolated()
{
    std::vector<glm::vec3> all = this->controlPolygon->mesh->getVertices();
    auto first = all.begin() + size1 - 3;
    auto last = all.begin() + size2;
    std::vector<glm::vec3> newVec = {first, last};
    return newVec;
}

std::vector<glm::vec3> Curve::getBezier()
{
    std::vector<glm::vec3> all = this->controlPolygon->mesh->getVertices();
    std::vector<glm::vec3> newVec = {all.begin() + 4, all.begin() + size1};
    return newVec;
}

std::vector<glm::vec3> Curve::getControl()
{
    std::vector<glm::vec3> all = this->controlPolygon->mesh->getVertices();
    std::vector<glm::vec3> newVec = {all.begin(), all.begin() + 4};
    return newVec;
}

