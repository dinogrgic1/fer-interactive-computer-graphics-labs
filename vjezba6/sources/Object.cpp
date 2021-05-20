#include "Object.hpp"

Object::Object(Mesh* m, Transform *t)
{
    this->mesh = m;
    this->transform = t;
}

Object::~Object()
{
    delete this->mesh;
    delete this->transform;
}
