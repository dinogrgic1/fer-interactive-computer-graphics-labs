// Local Headers
#include "Shader.h"
#include "Object.hpp"
#include "Curve.hpp"
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

int width = 900, height = 900;
float offset = 0.5f;
float nearPlane = 1.0f;
float farPlane = 100.0f;
char lastKey;

glm::mat4 projection = glm::frustum(-offset, offset, -offset, offset, nearPlane, farPlane);

Curve *c = nullptr;
Camera *camera = new Camera(0.0f, 1.5f, 3.0f);
glm::vec4 lightPos = glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
std::vector<glm::vec3> crrr = std::vector<glm::vec3>();

glm::mat4 view = glm::lookAt(camera->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

std::vector<glm::vec3> interpolated = std::vector<glm::vec3>();
std::vector<glm::vec3> bezier = std::vector<glm::vec3>();
std::vector<glm::vec3> control = std::vector<glm::vec3>();


bool animationStart = false;
int lastInterpolated = 0;

Shader *loadShader(char *path, char *naziv)
{
	std::string sPath(path);
	std::string pathVert;
	std::string pathFrag;
	std::string pathGeom;

	//malo je nespretno napravljeno jer ne koristimo biblioteku iz c++17, a treba podrzati i windows i linux
	pathVert.append(path, sPath.find_last_of("\\/") + 1);
	pathFrag.append(path, sPath.find_last_of("\\/") + 1);
	pathGeom.append(path, sPath.find_last_of("\\/") + 1);
	if (pathFrag[pathFrag.size() - 1] == '/')
	{
		pathVert.append("shaders/");
		pathFrag.append("shaders/");
		pathGeom.append("shaders/");
	}
	else if (pathFrag[pathFrag.size() - 1] == '\\')
	{
		pathVert.append("shaders\\");
		pathFrag.append("shaders\\");
		pathGeom.append("shaders\\");
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
	pathGeom.append(naziv);
	pathGeom.append(".geom");

	return new Shader(pathVert.c_str(), pathFrag.c_str(), pathGeom.c_str());
}

Shader *loadShaderNoGeom(char *path, char *naziv)
{
    std::string sPath(path);
    std::string pathVert;
    std::string pathFrag;

    //malo je nespretno napravljeno jer ne koristimo biblioteku iz c++17, a treba podrzati i windows i linux
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

    return new Shader(pathVert.c_str(), pathFrag.c_str(), 0);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	float deltaX = (width) / 2 - xpos;
	float deltaY = (height) / 2 - ypos;

	//camera += glm::vec3(0.001f * deltaX, 0.0f, 0.0f);
	//camera += glm::vec3(0.0f, 0.001f * deltaY, 0.0f);
	//view = Transform::lookAtMatrix(camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glfwSetCursorPos(window, (width) / 2, (height) / 2);
}

void key_callback(GLFWwindow *window, int key, int scancdoe, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
        glm::vec3 tmp = glm::vec3(0.0f, 0.0f, 0.0f);
	    if(action != GLFW_REPEAT && key == GLFW_KEY_K)
        {
            crrr.emplace_back(camera->position);
//            glm::vec4 v4 = camera->modelMatrix * glm::vec4(camera->position, 1.0f);
//            v4 = v4 / v4.w;
//            glm::vec3 v3 = glm::vec4(v4);
//            crrr.emplace_back(v3);
//            lastKey = key;
//            std::cout << crrr.size() << std::endl;
        }
	    if(key == GLFW_KEY_SPACE)
        {
	        animationStart = true;
        }

		switch (key)
		{
		case GLFW_KEY_W:
			tmp.z = -0.1f;
			break;
		case GLFW_KEY_S:
			tmp.z = +0.1f;
			break;
		case GLFW_KEY_A:
			tmp.x = +0.1f;
			break;
		case GLFW_KEY_D:
			tmp.x = -0.1f;
			break;
		case GLFW_KEY_E:
			tmp.y = +0.1f;
			break;
		case GLFW_KEY_Q:
			tmp.y = -0.1f;
			break;
		}
	    camera->setPosition(camera->position + tmp);
        view = glm::lookAt(camera->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
	objPath.append("/kocka/glava.obj"); //za linux pretvoriti u forwardslash

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

		objects.push_back(new Object(m, new Transform(-0.25f, 0.0f, 0.0f)));
		objects[0]->transform->scale(2.0f, 2.0f, 2.0f);

		std::vector<glm::vec3> v = m->getVertices();
		std::vector<int> indeces = m->getIndeces();

        crrr.emplace_back(0.0f, 0.0f, 0.0f);
        crrr.emplace_back(-1.5f, -0.5f, 0.0f);
        crrr.emplace_back(-1.5f, -0.5f, 1.5f);
        crrr.emplace_back(0.0f, 0.5f, 1.5f);

        c = new Curve(crrr);
        if(c != nullptr)
        {
            interpolated = c->getInterpolated();
            bezier = c->getBezier();
            control = c->getControl();
        }
        else
        {
            interpolated = std::vector<glm::vec3>();
            bezier = std::vector<glm::vec3>();
            control = std::vector<glm::vec3>();
        }

		GLFWwindow *window;
		glfwInit();

		window = glfwCreateWindow(width, height, "Vjezba 7 -- Bezier", nullptr, nullptr);

		// Check for Valid Context
		if (window == nullptr)
		{
			fprintf(stderr, "Failed to Create OpenGL Context");
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window);
		gladLoadGL();
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

		glClearColor(0.15, 0.1, 0.1, 1);

		//generiranje buffera
		GLuint VAO[4];
		GLuint VBO[4];
		GLuint EBO[2];
		glGenVertexArrays(4, VAO);
		glGenBuffers(4, VBO);
		glGenBuffers(2, EBO);

		glBindVertexArray(VAO[0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(glm::vec3), &v[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeces.size() * sizeof(int), &indeces[0], GL_STATIC_DRAW);

            glBindVertexArray(VAO[1]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, control.size() * sizeof(glm::vec3), &control[0], GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindVertexArray(VAO[2]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
            glBufferData(GL_ARRAY_BUFFER, bezier.size() * sizeof(glm::vec3), &bezier[0], GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindVertexArray(VAO[3]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
            glBufferData(GL_ARRAY_BUFFER, interpolated.size() * sizeof(glm::vec3), &interpolated[0], GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		Shader *shaderProjcSystem = loadShader(argv[0], "cullingProjcSystem");
		Shader *shaderLine = loadShaderNoGeom(argv[0], "shader");

		GLint uniformProjProjc = glGetUniformLocation(shaderProjcSystem->ID, "matProjection");
		GLint uniformViewProjc = glGetUniformLocation(shaderProjcSystem->ID, "matView");
        GLint uniformModelProjc = glGetUniformLocation(shaderProjcSystem->ID, "matModel");
		GLint eyeView = glGetUniformLocation(shaderProjcSystem->ID, "eyeView");

        GLint uniformProj = glGetUniformLocation(shaderLine->ID, "matProjection");
        GLint uniformView = glGetUniformLocation(shaderLine->ID, "matView");
        GLint uniformModel = glGetUniformLocation(shaderLine->ID, "matModel");
        GLint uniformColor = glGetUniformLocation(shaderLine->ID, "color");

        glClearColor(0.80f, 0.80f, 0.80f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetCursorPos(window, (width) / 2, (height) / 2);

		while (glfwWindowShouldClose(window) == false)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

            if(c == nullptr && crrr.size() == 4)
            {
                c = new Curve(crrr);
                interpolated = c->getInterpolated();
                bezier = c->getBezier();
                control = c->getControl();
            }

            if(animationStart)
            {
                if(lastInterpolated < interpolated.size())
                {
                    camera->setPosition(interpolated[lastInterpolated++]);
                }
                else {
                    animationStart = false;
                    lastInterpolated = 0;
                    camera->setPosition(glm::vec3(0.0f, 1.5f, 3.0f));
                }
                view = glm::lookAt(camera->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }

			glUseProgram(shaderProjcSystem->ID);
			glUniformMatrix4fv(uniformProjProjc, 1, GL_FALSE, &projection[0][0]);
			glUniformMatrix4fv(uniformViewProjc, 1, GL_FALSE, &view[0][0]);
			glUniform3fv(eyeView, 1, &camera->position[0]);

			glBindVertexArray(VAO[0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			for (int i = 0; i < objects.size(); i++)
            {
				glUniformMatrix4fv(uniformModelProjc, 1, GL_FALSE, &(objects[i]->transform->modelMatrix[0][0]));
				glDrawElements(GL_TRIANGLES, indeces.size(), GL_UNSIGNED_INT, 0);
			}

            if(!animationStart && c != nullptr)
            {
                glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
                glUseProgram(shaderLine->ID);
                glBindVertexArray(VAO[1]);
                glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
                glUniform3fv(uniformColor, 1, &color[0]);
                glBufferData(GL_ARRAY_BUFFER, control.size() * sizeof(glm::vec3), &control[0], GL_DYNAMIC_DRAW);
                glUniformMatrix4fv(uniformProj, 1, GL_FALSE, &projection[0][0]);
                glUniformMatrix4fv(uniformView, 1, GL_FALSE, &view[0][0]);
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &(c->controlPolygon->transform->modelMatrix[0][0]));
                glDrawArrays(GL_LINE_STRIP, 0, control.size());

                glBindVertexArray(VAO[2]);
                color = glm::vec3(0.60f, 0.0f, 0.0f);
                glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
                glUniform3fv(uniformColor, 1, &color[0]);
                glBufferData(GL_ARRAY_BUFFER, bezier.size() * sizeof(glm::vec3), &bezier[0], GL_DYNAMIC_DRAW);
                glUniformMatrix4fv(uniformProj, 1, GL_FALSE, &projection[0][0]);
                glUniformMatrix4fv(uniformView, 1, GL_FALSE, &view[0][0]);
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &(c->controlPolygon->transform->modelMatrix[0][0]));
                glDrawArrays(GL_LINE_STRIP, 0, bezier.size());

                glBindVertexArray(VAO[3]);
                color = glm::vec3(0.0f, 0.0f, 0.60f);
                glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
                glUniform3fv(uniformColor, 1, &color[0]);
                glBufferData(GL_ARRAY_BUFFER, interpolated.size() * sizeof(glm::vec3), &(interpolated[0]), GL_DYNAMIC_DRAW);
                glUniformMatrix4fv(uniformProj, 1, GL_FALSE, &projection[0][0]);
                glUniformMatrix4fv(uniformView, 1, GL_FALSE, &view[0][0]);
                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &(c->controlPolygon->transform->modelMatrix[0][0]));
                glDrawArrays(GL_LINE_STRIP, 0, interpolated.size());
            }

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

        delete m;
		delete c;
		delete shaderProjcSystem;
		delete shaderLine;
		glDeleteBuffers(4, VBO);
		glDeleteBuffers(2, EBO);
		glDeleteVertexArrays(4, VAO);
		glfwTerminate();

		return EXIT_SUCCESS;
	}
}