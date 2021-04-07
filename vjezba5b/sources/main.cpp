// Local Headers
#include "Mesh.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <iostream>

int width = 900, height = 600;

Shader *loadShader(char *path, char *naziv)
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

	return new Shader(pathVert.c_str(), pathFrag.c_str());
}

void framebuffer_size_callback(GLFWwindow *window, int Width, int Height)
{
	width = Width;
	height = Height;
}

int main(int argc, char *argv[])
{
	Assimp::Importer importer;

	std::string path(argv[0]);
	std::string dirPath(path, 0, path.find_last_of("\\/"));
	std::string resPath(dirPath);
	resPath.append("/resources"); //za linux pretvoriti u forwardslash
	std::string objPath(resPath);
	objPath.append("/kocka/kocka.obj"); //za linux pretvoriti u forwardslash

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
		std::vector<glm::vec3> v = std::vector<glm::vec3>();
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			glm::vec3 t = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			v.push_back(t);
		}

		//svaki poligon se sastoji od 3 ili vise tocki.
		std::vector<int> indeces = std::vector<int>();
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				indeces.push_back(mesh->mFaces[i].mIndices[j]);
		}

		Mesh *m = new Mesh(v, indeces);
		
		v = m->getVertices();
        indeces = m->getIndeces();

		GLFWwindow *window;

		glfwInit();

		window = glfwCreateWindow(width, height, "Vjezba 5a", nullptr, nullptr);

		// Check for Valid Context
		if (window == nullptr)
		{
			fprintf(stderr, "Failed to Create OpenGL Context");
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window);

		gladLoadGL();

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //funkcija koja se poziva prilikom mijenjanja velicine prozora

		fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

		glClearColor(0.15, 0.1, 0.1, 1);

		//generiranje buffera
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(glm::vec3), &v[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeces.size() * sizeof(int), &indeces[0], GL_STATIC_DRAW);

		/*********************************************************************************************/
		//primjer 4a
		//instanciranje istog "objekta" preko uniforme varijable, u odnosu na primjer 3 promijenio se samo shader i uvela dodatna uniform varijabla
		Shader *shader = loadShader(argv[0], "shader2");

		GLint lokacijaUniformVarijable2 = glGetUniformLocation(shader->ID, "tMatrica");

		//izrada polja transformacija koji  postavljaju objekte u mrezu 4x4
		glm::mat4 jedinicna = glm::mat4(1);
		glm::mat4 skaliranje = glm::scale(jedinicna, glm::vec3(1, 1, 1));
		glm::mat4 poljeTransformacija[16];
		int brojac = 0;
		for (float i = -1; i < 1; i += 0.5)
		{
			for (float j = -1; j < 1; j += 0.5)
			{
				poljeTransformacija[brojac++] = glm::translate(jedinicna, glm::vec3(j + 0.5, i + 0.5, 0)) * skaliranje;
			}
		}

		glBindVertexArray(0);
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

		while (glfwWindowShouldClose(window) == false)
		{

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);

			/********************************************************/
			//primjer 4a
			//za svaku instancu objekta saljemo naredbu za iscrtavanje. podaci o modelu ostaju na grafickoj, mijenja se samo uniform varijabla.
			glUseProgram(shader->ID);
			glBindVertexArray(VAO);

			for (int i = 0; i < 16; i++)
			{
				glUniformMatrix4fv(lokacijaUniformVarijable2, 1, GL_FALSE, &poljeTransformacija[i][0][0]);
				glDrawElements(GL_LINE_STRIP, indeces.size(), GL_UNSIGNED_INT, 0);
			}

			glBindVertexArray(0);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete shader;

		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);

		glfwTerminate();

		return EXIT_SUCCESS;
	}
}
