#include "PolgyonEelement.h"

#include <iostream>

PolygonElement::PolygonElement() 
{
    this->Vertex = std::pair<int, int>();
    this->Edge = std::tuple<int, int, int>();
    this->left = 0;
}

PolygonElement::PolygonElement(int x, int y)
{
    this->Vertex = std::make_pair(x, y);
    this->left = 0;
}

PolygonElement::~PolygonElement() { }

std::tuple<int, int, int> PolygonElement::calculateEdge(PolygonElement curr)
{
    int a = this->Vertex.second - curr.Vertex.second;
    int b = -(this->Vertex.first - curr.Vertex.first);
    int c = (this->Vertex.first * curr.Vertex.second) - (this->Vertex.second * curr.Vertex.first);
    this->left = (this->Vertex.second >= curr.Vertex.second);
    this->setEdge(std::make_tuple(a ,b, c));
}

std::tuple<int, int, int> PolygonElement::getEdge()
{
    return this->Edge;
}

std::pair<int, int> PolygonElement::getVertex()
{
    return this->Vertex;
}

int PolygonElement::getEdgeA() { return std::get<0>(this->Edge); }
int PolygonElement::getEdgeB() { return std::get<1>(this->Edge); }
int PolygonElement::getEdgeC() { return std::get<2>(this->Edge); }

void PolygonElement::setEdge(std::tuple<int, int, int> edge)
{
    this->Edge = edge;
}

int PolygonElement::getLeft() { return this->left; }