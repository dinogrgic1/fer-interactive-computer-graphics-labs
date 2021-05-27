#ifndef IRGLAB_CAMERA_HPP
#define IRGLAB_CAMERA_HPP
#include "Transform.hpp"

class Camera : public Transform
{
public:
    float yaw = 0.0f;
    float pitch = 0.0f;
    glm::vec3 target;

    Camera(float, float, float);

    glm::vec3 getTarget();
    void targetSet(float, float);
};

#endif //IRGLAB_CAMERA_HPP
