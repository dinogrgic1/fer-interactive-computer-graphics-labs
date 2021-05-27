#include "Transform.hpp"
#include "Mesh.hpp"

class Object
{   
public:
    Object(Mesh*, Transform*);
    Transform *transform;
    Mesh *mesh;
};