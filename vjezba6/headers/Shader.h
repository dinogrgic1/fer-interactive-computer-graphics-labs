#pragma once

#include <glad/glad.h>

#include <string>

#include <fstream>
#include <sstream>

class Shader
{
private:
	static void checkCompilerErrors(unsigned int shader, const std::string& type);
	Shader(const char* vertexPath, const char* fragmentPath, const char *geometryPath);

public:
	unsigned int ID;

	static Shader * loadShader(char *path, char *naziv);
	~Shader();

	void use() const;
	void setUniform(const std::string &name, bool value)const;
	void setUniform(const std::string &name, int value)const;
	void setUniform(const std::string &name, float value)const;


};

