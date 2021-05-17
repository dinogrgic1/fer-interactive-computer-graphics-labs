#include <utility>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
    public:
    glm::vec3 position;
    glm::mat4 modelMatrix = glm::mat4(1);
    
    void setPosition(glm::vec3);
    void setOrientation(glm::vec3);

    void move(glm::vec3);
    void rotate(float, glm::vec3);
    void scale(float, float, float);

    Transform(float, float, float);

};