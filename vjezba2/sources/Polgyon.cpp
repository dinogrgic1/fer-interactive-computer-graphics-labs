#include "Polygon.h"
#include "Lines.h"

#include <iostream>
#include <utility>
#include <stdexcept>
#include <cmath>


Polygon::Polygon() 
{ 
    this->Elements = std::vector<PolygonElement>();
}

Polygon::~Polygon() { }

std::vector<PolygonElement> Polygon::getElements()
{
    return this->Elements;
}

void Polygon::addVertex(int x, int y)
{
    PolygonElement tmp = PolygonElement(x, y);

    if(this->Elements.size() >= 1)
    {
        PolygonElement last = this->Elements.back();
        PolygonElement first = this->Elements.front();

        last.calculateEdge(tmp);
        tmp.calculateEdge(first);

        this->Elements.pop_back();
        this->Elements.push_back(last);
    }

    this->Elements.push_back(tmp);
}

bool Polygon::isConvex()
{
    int i0, r;
    int over, under, on;

    over = under = on = 0;

    int n = this->Elements.size(); 

    if(n <= 2)
    {
        return true;
    }

    i0 = n - 2;
    for(int i = 0; i < n; i++)
    {      
        if(i0 >= n)
            i0 = 0;

        std::pair<int, int> vertex = this->Elements[i].getVertex();
        std::tuple<int, int, int> edge = this->Elements[i0].getEdge();

        r = std::get<0>(edge) * vertex.first +
            std::get<1>(edge) * vertex.second +
            std::get<2>(edge);

        if(r == 0)
        {
            on++;
        } 
        else if(r > 0) 
        {
            over++;
        }
        else 
        {
            under++;
        }
        i0++;
    }

    //std::cout << under << " " << on << " " << over << std::endl;
    if(under == 0)
    {
        return true;
    }
    else if(over == 0)
    {
        return true;
    }
    return false;
}

void Polygon::popVertex()
{
    this->Elements.pop_back();
    PolygonElement pe = this->Elements.back();
    this->Elements.pop_back();
    this->addVertex(pe.getVertex().first, pe.getVertex().second);
}

int Polygon::getVerteciesNumber()
{
    return this->Elements.size();
}

void Polygon::draw(std::vector<std::pair<int, int>> &pravac)
{
    int i0, y;
    int xmin, xmax, ymin, ymax;

    int n = this->getVerteciesNumber();

    std::tuple<int, int, int, int> minMax = this->getMinMax();

    xmin = std::get<0>(minMax); xmax = std::get<1>(minMax);
    ymin = std::get<2>(minMax); ymax = std::get<3>(minMax);

    //std::cout << xmin << " " << xmax << std::endl;
    //std::cout << ymin << " " << ymax << std::endl;

    double L,D,x;
    for(y = ymin; y <= ymax; y++)
    {
        L = xmin, D = xmax;
        i0 = n - 1;
        for(int i = 0; i < n; i0 = i++)
        {
            PolygonElement start = this->Elements[i0];
            PolygonElement end = this->Elements[i];

            if(start.getEdgeA() == 0)
            {
                if(start.getVertex().second == y)
                {
                    if(start.getVertex().first < end.getVertex().first)
                    {
                        L = start.getVertex().first;
                        D = end.getVertex().first;
                    }
                    else
                    {
                        L = end.getVertex().first;
                        D = start.getVertex().first;
                    }   
                    break;
                }
            }
            else
            {
                x = (-(start.getEdgeB() * y) - start.getEdgeC()) / (float)start.getEdgeA();

                //std::cout << "clockwise: " << this->clockwise << std::endl;
                bool left = 0;
                if(this->clockwise != 0)
                {
                    left = !left;
                }
                    
                if(start.getLeft() == left)
                    {
                        if(x >= L)
                        {
                            L = x;
                        }
                    }
                        
                    else
                    {
                        if(x <= D)
                        {
                            D = x;
                        }
                    }
                }
                
            }
        //std::cout << round(L) << " " << x  << " " << round(D) << std::endl;
        //std::cout << "line draw (" << (int) (L + 0.5) << ", " << y << ") to (" << (int)  (D + 0.5) << ", " << y << std::endl;
        Lines().drawLine(pravac, (int) (L + 0.5), y, int (D + 0.5), y);
    }
}

std::tuple<int, int, int, int> Polygon::getMinMax()
{
    if(this->Elements.size() < 1)
        throw "[Error] Polygon has no vertecies";

    int xmin, xmax, ymin, ymax;
    xmin = xmax = this->Elements[0].getVertex().first;
    ymin = ymax = this->Elements[0].getVertex().second;

    for(int i = 1; i < this->Elements.size(); i++)
    {
        std::pair<int, int> pe = this->Elements[i].getVertex();
        if(xmin > pe.first)
            xmin = pe.first;
        if(xmax < pe.first)
            xmax = pe.first;
        if(ymin > pe.second)
            ymin = pe.second;
        if(ymax < pe.second)
            ymax = pe.second;
    }
    return std::make_tuple(xmin, xmax, ymin, ymax);
}

bool Polygon::dotIsInside(int x, int y)
{
    for(int i = 0; i < this->Elements.size(); i++)
    {
        PolygonElement pe = this->Elements[i];
        //std::cout << (x * pe.getEdgeA()) + (y * pe.getEdgeB()) + pe.getEdgeC() << std::endl;
        if(this->clockwise)
        {
            if(x * pe.getEdgeA() + y * pe.getEdgeB() + pe.getEdgeC() < 0)
            {
               return false;
            }
        }
        else
        {
            if(x * pe.getEdgeA() + y * pe.getEdgeB() + pe.getEdgeC() > 0)
            {
                return false;
            }
        }
        
    }
    return true;
}

void Polygon::setClockwise()
{
    int tmp = 0;
    for(int i = 1; i < this->Elements.size(); i++)
    {
        PolygonElement p1 = this->Elements[i-1];
        PolygonElement p2 = this->Elements[i];
        tmp += (p1.getVertex().first * p2.getVertex().second) -
                (p1.getVertex().second * p2.getVertex().first);
    }
    tmp += (this->Elements.back().getVertex().first * this->Elements.front().getVertex().second) -
                (this->Elements.back().getVertex().second * this->Elements.front().getVertex().first);
    
    this->clockwise = (tmp > 0);
}