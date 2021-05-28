// Local Headers
#include "Shader.h"
#include "Object.hpp"
#include "Light.hpp"
#include "Material.hpp"

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

Mesh *m;
glm::mat4 projection = glm::frustum(-offset, offset, -offset, offset, nearPlane, farPlane);

glm::vec3 camera = glm::vec3(0.0f, 0.5f, 2.0f);
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 1.0f);
Light light = Light(lightPos);

glm::mat4 view = glm::lookAt(camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	float deltaX = (width) / 2 - xpos;
	float deltaY = (height) / 2 - ypos;

	camera += glm::vec3(0.001f * deltaX, 0.0f, 0.0f);
	camera += glm::vec3(0.0f, 0.001f * deltaY, 0.0f);
	view = Transform::lookAtMatrix(camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glfwSetCursorPos(window, (width) / 2, (height) / 2);
}

void key_callback(GLFWwindow *window, int key, int scancdoe, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		glm::vec3 tmp = glm::vec3(0.0f, 0.0f, 0.0f);
		switch (key)
		{
		case GLFW_KEY_W:
			tmp.z = -0.1f;
			break;
		case GLFW_KEY_S:
			tmp.z = 0.1f;
			break;
		case GLFW_KEY_A:
			tmp.x = -0.1f;
			break;
		case GLFW_KEY_D:
			tmp.x = 0.1f;
			break;
		case GLFW_KEY_E:
			tmp.y = -0.1f;
			break;
		case GLFW_KEY_Q:
			tmp.y = 0.1f;
			break;
		}
		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->transform->setPosition(tmp);
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

		m = new Mesh(mesh);
		Material objMaterial = Material(scene, argv);
		objMaterial.print();

		objects.push_back(new Object(m, new Transform(-0.25f, 0.0f, 0.0f)));
		objects[0]->transform->scale(2.0f, 2.0f, 2.0f);

		std::vector<glm::vec3> v = m->getVertices();
		std::vector<glm::vec3> vecNorm = m->getVertexNormals();
		std::vector<int> indeces = m->getIndeces();
        std::vector<glm::vec2> uvCords = m->getUvCords();

        GLFWwindow *window;
		glfwInit();

		window = glfwCreateWindow(width, height, "Vjezba 8c -- Phongovo sjenacnje", nullptr, nullptr);

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
		GLuint VAO;
		GLuint VBO[3];
		GLuint EBO[2];
        GLuint texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, objMaterial.getWidth(), objMaterial.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, objMaterial.getData());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

        glGenVertexArrays(1, &VAO);
		glGenBuffers(3, VBO);
		glGenBuffers(1, EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(glm::vec3), &v[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, vecNorm.size() * sizeof(glm::vec3), &vecNorm[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, uvCords.size() * sizeof(glm::vec2), &uvCords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeces.size() * sizeof(int), &indeces[0], GL_STATIC_DRAW);

        glBindVertexArray(0);

        Shader *shader = loadShader(argv[0], "shader");
		GLint uniformProj = glGetUniformLocation(shader->ID, "matProjection");
		GLint uniformView = glGetUniformLocation(shader->ID, "matView");
		GLint uniformModel = glGetUniformLocation(shader->ID, "matModel");

		GLint eyeView = glGetUniformLocation(shader->ID, "eyeView");
		GLint lightPosUniform = glGetUniformLocation(shader->ID, "lightPos");
		GLint lightUniform = glGetUniformLocation(shader->ID, "light");
		GLint materialPropsUniform = glGetUniformLocation(shader->ID, "materialProps");

		glm::mat3x3 mater = objMaterial.getMaterialProps();
		glm::mat3x3 lightt = light.getLightMatrix();

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

			glUseProgram(shader->ID);
            glBindVertexArray(VAO);

			glUniformMatrix3fv(lightUniform, 1, GL_FALSE, &(lightt[0][0]));
			glUniformMatrix3fv(materialPropsUniform, 1, GL_FALSE, &(mater[0][0]));
			glUniform3fv(lightPosUniform, 1, &lightPos[0]);

			glUniformMatrix4fv(uniformProj, 1, GL_FALSE, &projection[0][0]);
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, &view[0][0]);
			glUniform3fv(eyeView, 1, &camera[0]);

			for (int i = 0; i < objects.size(); i++)
			{
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, &(objects[i]->transform->modelMatrix[0][0]));
				glDrawElements(GL_TRIANGLES, indeces.size(), GL_UNSIGNED_INT, 0);
			}
			glBindVertexArray(0);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete shader;
        glDeleteTextures(1, &texture);
		glDeleteBuffers(2, VBO);
		glDeleteBuffers(2, EBO);
		glDeleteVertexArrays(1, &VAO);
		glfwTerminate();

		return EXIT_SUCCESS;
	}
}