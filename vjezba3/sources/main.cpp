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

int width = 500, height = 500;
bool follow = 0;

typedef struct
{
	float x, y, z;
} Vertex;

std::vector<Vertex> points;
std::vector<double> colors;
std::vector<double> index;

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
		follow = !follow;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		xpos = xpos / (width / 2) - 1.0;
		ypos = -1 * (ypos / (height / 2) - 1.0);
		Vertex v = {xpos, ypos, 0.0f};
		points.push_back(v);
	}
}

void cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
	xpos = xpos / (width / 2) - 1.0;
	ypos = -1 * (ypos / (height / 2) - 1.0);
	Vertex v = {xpos, ypos, 0.0f};

	if (points.size() == 1)
	{	
		points.push_back(v);
	}
	else if(points.size() > 1)
	{
		points.pop_back();
		points.push_back(v);
	}
}

int main(int argc, char *argv[])
{
	std::cout << argv[0] << std::endl;
	/*********************************************************************************************/
	//postavljanje OpenGL konteksta, dohvacanje dostupnih OpenGL naredbi
	GLFWwindow *window;

	glfwInit();
	gladLoadGL();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	window = glfwCreateWindow(width, height, "Zadatak 3", nullptr, nullptr);
	// provjeri je li se uspio napraviti prozor
	if (window == nullptr)
	{
		fprintf(stderr, "Failed to Create OpenGL Context");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// dohvati sve dostupne OpenGL funkcije
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "Failed to initialize GLAD");
		exit(-1);
	}
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glEnable(GL_CULL_FACE); //ukljuci uklanjanje straznjih poligona -- za ovaj primjer je iskljuceno
	//glCullFace(GL_BACK);

	glClearColor(0.15, 0.1, 0.1, 1);

	glfwSwapInterval(0);

	FPSManager FPSManagerObject(window, 60, 1.0, "Zadatak X");

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //funkcija koja se poziva prilikom mijenjanja velicine prozora
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_callback);

	/*********************************************************************************************/
	Shader *sjencar = loadShader(argv[0], "shader");

	/*********************************************************************************************/
	//prenosenje podataka i objasnjavanje u kojem formatu su ti podaci
	//generiranje buffera

	GLuint VAO;
	GLuint VBO[2];
	GLuint EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	GLint lokacijaUniformVarijable = glGetUniformLocation(sjencar->ID, "u_color");

	glBindVertexArray(VAO);
	//buffer za koordinate i povezi s nultim mjestom u sjencaru -- layout (location = 0)
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

	//buffer za boje i povezi s prvim mjestom u sjencaru -- layout (location = 1)
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//buffer za indekse, moze biti samo jedan GL_ELEMENT_ARRAY_BUFFER po VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index.data(), GL_STATIC_DRAW);
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
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * points.size(), &points[0], GL_DYNAMIC_DRAW);
		glUniform3f(lokacijaUniformVarijable, 0.5, 1.0, 1.0);
		glDrawArrays(GL_LINE_STRIP, 0, points.size());
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	delete sjencar;
	glDeleteBuffers(2, VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();

	return EXIT_SUCCESS;
}