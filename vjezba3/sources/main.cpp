// Local Headers

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "FPSManager.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <iostream>

glm::vec3 color_picked = glm::vec3(0.6f, 0.4f, 0.0f);
int color_chosen = 0;
int to3 = 0;
int triangles = 0;
int width = 500, height = 500;

std::vector<glm::vec3> points;
std::vector<double> colors;
std::vector<int> index;

//malo je nespretno napravljeno jer ne koristimo c++17, a treba podrzati i windows i linux,
//slobodno pozivajte new Shader(...); direktno
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

//funkcija koja se poziva prilikom mijenjanja velicine prozora, moramo ju poveyati pomocu glfwSetFramebufferSizeCallback
void framebuffer_size_callback(GLFWwindow *window, int Width, int Height)
{
	width = Width;
	height = Height;

	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		xpos = xpos / (width / 2) - 1.0;
		ypos = -1 * (ypos / (height / 2) - 1.0);

		points.push_back(glm::vec3(xpos, ypos, 0.0f));
		index.push_back(triangles);
		to3++;

		if (to3 == 3)
		{
			to3 = 1;
			index.push_back(triangles);
		}

		triangles++;
	}
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
	xpos = xpos / (width / 2) - 1.0;
	ypos = -1 * (ypos / (height / 2) - 1.0);
	glm::vec3 v = {xpos, ypos, 0.0f};

	if (points.size() == 1)
	{
		points.push_back(v);
	}
	else if (points.size() > 1)
	{
		points.pop_back();
		points.push_back(v);
	}
}

void key_callback(GLFWwindow *window, int key, int scancdoe, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		float value = 0.00f;
		if (key == 82)
		{
			std::cout << "==========Red color component chosen==========" << std::endl;
			color_chosen = 0;
		}
		else if (key == 71)
		{
			std::cout << "==========Green color component chosen==========" << std::endl;
			color_chosen = 1;
		}
		else if (key == 66)
		{
			std::cout << "==========Blue color component chosen==========" << std::endl;
			color_chosen = 2;
		}
		else if (key == 61)
		{
			value = 0.05f;
		}
		else if (key == 45)
		{
			value = -0.05f;
		}

		switch (color_chosen)
		{
		case 0:
			if (color_picked.x + value <= 1.01f && color_picked.x + value >= -0.01f)
			{
				color_picked.x += value;
			}
			break;
		case 1:
			if (color_picked.y + value <= 1.01f && color_picked.y + value >= -0.01f)
			{
				color_picked.y += value;
			}
			break;
		case 2:
			if (color_picked.z + value <= 1.01f && color_picked.z + value >= -0.01f)
			{
				color_picked.z += value;
			}
			break;
		}
		std::cout << "Color state: (R: " << color_picked.x << ", G: " << color_picked.y << ", B: " << color_picked.z << ")" << std::endl;
	}
}

int main(int argc, char *argv[])
{
	std::cout << argv[0] << std::endl;
	GLFWwindow *window;

	glfwInit();
	gladLoadGL();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	window = glfwCreateWindow(width, height, "Zadatak 3", nullptr, nullptr);
	if (window == nullptr)
	{
		fprintf(stderr, "Failed to Create OpenGL Context");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD");
		exit(-1);
	}
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(0.15, 0.1, 0.1, 1);

	glfwSwapInterval(0);

	FPSManager FPSManagerObject(window, 60, 1.0, "Vjezba 3");

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //funkcija koja se poziva prilikom mijenjanja velicine prozora
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetKeyCallback(window, key_callback);

	Shader *sjencar = loadShader(argv[0], "shader");
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	GLint lokacijaUniformVarijable = glGetUniformLocation(sjencar->ID, "u_color");

	glBindVertexArray(VAO);
	//buffer za koordinate i povezi s nultim mjestom u sjencaru -- layout (location = 0)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), &index[0], GL_DYNAMIC_DRAW);
	glBindVertexArray(0);

	/*********************************************************************************************/
	//"instanciranje objekata" svaka matrica reprezentira novu instancu objekta. Izrada polja transformacija koji  postavljaju objekte u mrezu 4x4

	glm::mat4 jedinicna = glm::mat4(1);
	glm::mat4 poljeTransformacija[50];

	/*********************************************************************************************/
	//glavna petlja za prikaz
	while (glfwWindowShouldClose(window) == false)
	{

		float deltaTime = (float)FPSManagerObject.enforceFPS(false);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(sjencar->ID);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), &points[0], GL_DYNAMIC_DRAW);
		glUniform3f(lokacijaUniformVarijable, color_picked.x, color_picked.y, color_picked.z);
		glDrawArrays(GL_LINE_STRIP, 0, points.size());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), &index[0], GL_DYNAMIC_DRAW);
		glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	delete sjencar;
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();

	return EXIT_SUCCESS;
}