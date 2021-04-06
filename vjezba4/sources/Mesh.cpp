#include "Mesh.hpp"

std::tuple<float, float, float, float, float, float> Mesh::getBoundingBox()
{
    float minx, miny, minz;
    float maxx, maxy, maxz;

    minx = maxx = std::get<0>(this->vertices[0]);
    miny = maxy = std::get<1>(this->vertices[0]);
    minz = maxz = std::get<2>(this->vertices[0]);

    for(int i = 1; i < this->vertices.size(); i++)
    {
        float x, y, z;
        x = std::get<0>(this->vertices[i]);
        y = std::get<1>(this->vertices[i]);
        z = std::get<2>(this->vertices[i]);

        if(x < minx)
        {
            minx = x;
        }

        if(x > maxx)
        {
            maxx = x;
        }

        if(y < miny)
        {
            miny = y;
        }

        if(y > maxy)
        {
            maxy = y;
        }

        if(z < minz)
        {
            minz = z;
        }

        if(x > maxx)
        {
            maxz = z;
        }
    }

    return std::tuple<float, float, float, float, float, float>(minx, maxx, miny, maxy, minz, maxz);
}

Mesh::Mesh(std::vector<std::tuple<float, float, float>> vertices, std::vector<float> index)
{
    this->vertices = vertices;
    this->indeces = index;
}

void Mesh::addVertex(std::tuple<float, float, float> t)
{
    this->vertices.push_back(t);
}