#include "Transform.hpp"
#include "Mesh.hpp"

class Object
{   
public:
    Object(Mesh*, Transform*);
    ~Object();

    Transform *transform;
    Mesh *mesh;
};