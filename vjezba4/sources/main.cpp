// Local Headers
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Shader.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Standard Headers
#include <iostream>
#include <cstdlib>
#include <tuple>

const int mWidth = 1280;
const int mHeight = 800;

Shader *loadShader(char *path, char *naziv)
{
    std::string sPath(path);
    std::string pathVert;
    std::string pathFrag;


    pathVert.append(path, sPath.find_last_of("\\/") + 1);
    pathFrag.append(path, sPath.find_last_of("\\/") + 1);
    if (pathFrag[pathFrag.size() - 1] == '/')
    {
        pathVert.append("shaders/");
        pathFrag.append("shaders/");
    }
    else if (pathFrag[pathFrag.size() - 1] == '\\')
    {
        pathVert.append("shaders\\");
        pathFrag.append("shaders\\");
    }
    else
    {
        std::cerr << "nepoznat format pozicije shadera";
        exit(1);
    }

    pathVert.append(naziv);
    pathVert.append(".vert");
    pathFrag.append(naziv);
    pathFrag.append(".frag");

    return new Shader(pathVert.c_str(), pathFrag.c_str());
}

int main(int argc, char *argv[])
{
    Assimp::Importer importer;

    std::string path(argv[0]);
    std::string dirPath(path, 0, path.find_last_of("\\/"));
    std::string resPath(dirPath);
    resPath.append("/resources"); //za linux pretvoriti u forwardslash
    std::string objPath(resPath);
    objPath.append("/teapot/teapot.obj"); //za linux pretvoriti u forwardslash

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

        //popis svih tocaka u modelu s x, y, z koordinatama
        std::vector<std::tuple<float, float, float>> v = std::vector<std::tuple<float, float, float>>();
        for (int i = 0; i < mesh->mNumVertices; i++)
        {
            std::tuple<float, float, float> t = std::tuple<float, float, float>(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            v.push_back(t);
        }

        //svaki poligon se sastoji od 3 ili vise tocki.
        std::vector<float> indeces = std::vector<float>();
        for (int i = 0; i < mesh->mNumFaces; i++)
        {
            for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
                indeces.push_back(mesh->mFaces[i].mIndices[j]);
        }

        Shader *shader = loadShader(argv[0], "shader");
        GLFWwindow *window;
        glfwInit();
        window = glfwCreateWindow(mWidth, mHeight, "Vjezba 4", nullptr, nullptr);

        if (window == nullptr)
        {
            fprintf(stderr, "Failed to Create OpenGL Context");
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window);

        gladLoadGL();
        fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

        //generiranje buffera
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(v), &v[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), (void *)(&indeces[0]), GL_STATIC_DRAW);
        glBindVertexArray(0);

        while (glfwWindowShouldClose(window) == false)
        {
            glClear(GL_COLOR_BUFFER_BIT);

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            glUseProgram(shader->ID);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, sizeof(v), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }
}