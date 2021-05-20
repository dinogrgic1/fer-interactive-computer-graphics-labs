// Local Headers
#include "Shader.h"
#include "Object.hpp"
#include "Camera.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

std::vector<Object *> objects;
Camera *camera = new Camera(0.0f, 0.0f, 2.0f);

int width = 900, height = 900;
float offset = 0.5f;
float nearPlane = 1.0f;
float farPlane = 1000.0f;

glm::mat4 projection = glm::perspective(glm::radians(60.f) , (float)(width/height), nearPlane, farPlane);
glm::mat4 view = glm::lookAt(camera->getPosition(), camera->getTarget(), glm::vec3(0.0f, 1.0f, 0.0f));

void framebuffer_size_callback(GLFWwindow *window, int Width, int Height)
{
    width = Width;
    height = Height;
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    float deltaX = (width) / 2 - xpos;
    float deltaY = (height) / 2 - ypos;

    // TODO: Camera look
    float sensitivtiy = 0.05f;
    deltaX *= sensitivtiy;
    deltaY *= sensitivtiy;
    camera->targetSet(deltaX, deltaY);

    view = Transform::lookAtMatrix(camera->getPosition(), camera->getTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
    glfwSetCursorPos(window, (width) / 2, (height) / 2);
}

void key_callback(GLFWwindow *window, int key, int scancdoe, int action, int mods)
{
    if (action == GLFW_REPEAT || action == GLFW_PRESS)
    {
        glm::vec3 tmp = glm::vec3(0.0f, 0.0f, 0.0f);
        switch (key)
        {
            // TODO: Camera strafe
            case GLFW_KEY_W:
                camera->move(glm::normalize(camera->getTarget() - camera->getPosition()) / 50.0f);
                view = glm::lookAt(camera->getPosition(), camera->getTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
                break;
            case GLFW_KEY_S:
                camera->move(glm::normalize(camera->getTarget() - camera->getPosition()) / -50.0f);
                view = glm::lookAt(camera->getPosition(), camera->getTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[])
{
    Assimp::Importer importer;

    std::string path(argv[0]);
    std::string dirPath(path, 0, path.find_last_of("\\/"));
    std::string resPath(dirPath);
    resPath.append("/resources"); //za linux pretvoriti u forwardslash
    std::string objPath(resPath);
    objPath.append("/glava/glava.obj"); //za linux pretvoriti u forwardslash

    const aiScene *scene = importer.ReadFile(objPath.c_str(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);

    if (!scene)
    {
        std::cerr << importer.GetErrorString();
        return false;
    }

    if (scene->HasMeshes())
    {
        aiMesh *mesh = scene->mMeshes[0];
        Mesh *m = new Mesh(mesh);

        objects.push_back(new Object(m, new Transform(1.0f, 0.0f, 0.0f)));
        objects.push_back(new Object(m, new Transform(-1.0f, 0.0f, 0.0f)));
        objects[0]->transform->scale(2.0f,2.0f,2.0f);
        objects[1]->transform->scale(2.0f,2.0f,2.0f);

        std::vector<glm::vec3> v = m->getVertices();
        std::vector<int> indeces = m->getIndeces();

        glfwInit();
        GLFWwindow *window = glfwCreateWindow(width, height, "Vjezba 6 -- backface culling", nullptr, nullptr);
        if (window == nullptr)
        {
            fprintf(stderr, "Failed to Create OpenGL Context");
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(window);

        gladLoadGL();
        fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);

        glClearColor(0.15, 0.1, 0.1, 1);

        GLuint VAO[2];
        GLuint VBO[2];
        GLuint EBO[2];

        glGenVertexArrays(2, VAO);
        glGenBuffers(2, VBO);
        glGenBuffers(2, EBO);

        glBindVertexArray(VAO[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, m->getVertices().size() * sizeof(glm::vec3), &(m->getVertices()[0]), GL_STATIC_DRAW);


        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->getIndeces().size() * sizeof(int), &(m->getIndeces()[0]), GL_STATIC_DRAW);

        glBindVertexArray(VAO[1]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, m->getVertices().size() * sizeof(glm::vec3), &(m->getVertices()[0]), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->getIndeces().size() * sizeof(int), &(m->getIndeces()[0]), GL_STATIC_DRAW);
        glBindVertexArray(0);

        Shader *shaderProj = Shader::loadShader(argv[0], "cullingProjcSystem");
        Shader *shaderScene = Shader::loadShader(argv[0], "cullingSceneSystem");

        GLint uniformProjCps = glGetUniformLocation(shaderProj->ID, "matProjection");
        GLint uniformViewCps = glGetUniformLocation(shaderProj->ID, "matView");
        GLint uniformModelCps = glGetUniformLocation(shaderProj->ID, "matModel");

        GLint uniformProjCss = glGetUniformLocation(shaderScene->ID, "matProjection");
        GLint uniformViewCss = glGetUniformLocation(shaderScene->ID, "matView");
        GLint uniformModelCss = glGetUniformLocation(shaderScene->ID, "matModel");
        GLint eyeViewCss = glGetUniformLocation(shaderScene->ID, "eyeView");

        glClearColor(0.80f, 0.80f, 0.80f, 0.0f);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetCursorPos(window, (width) / 2, (height) / 2);

        while (glfwWindowShouldClose(window) == false)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            glUseProgram(shaderScene->ID);
            glBindVertexArray(VAO[0]);
            glUniformMatrix4fv(uniformProjCss, 1, GL_FALSE, &projection[0][0]);
            glUniformMatrix4fv(uniformViewCss, 1, GL_FALSE, &view[0][0]);
            glUniform3fv(eyeViewCss, 1, &(camera->getPosition()[0]));
            glUniformMatrix4fv(uniformModelCss, 1, GL_FALSE, &(objects[0]->transform->modelMatrix[0][0]));
            glDrawElements(GL_TRIANGLES, indeces.size(), GL_UNSIGNED_INT, 0);

            glUseProgram(shaderProj->ID);
            glBindVertexArray(VAO[0]);
            glUniformMatrix4fv(uniformProjCps, 1, GL_FALSE, &projection[0][0]);
            glUniformMatrix4fv(uniformViewCps, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(uniformModelCps, 1, GL_FALSE, &(objects[1]->transform->modelMatrix[0][0]));
            glDrawElements(GL_TRIANGLES, indeces.size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        delete shaderScene;     delete shaderProj;
        glDeleteBuffers(2, VBO);
        glDeleteBuffers(2, EBO);
        glDeleteVertexArrays(2, VAO);

        glfwTerminate();
        return EXIT_SUCCESS;
    }
}
