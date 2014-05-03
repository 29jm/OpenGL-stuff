#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();
	Shader(std::string vertex, std::string fragment);
	~Shader();

	bool loadFromMemory(const GLchar* vertex, const GLchar* fragment);

	bool compile();
	void bind();
	std::string getShaderLog();
	bool isCompiled();
	GLint uniform(std::string uni);

	GLuint index() const;

private:
	enum class ShaderType {
		Vertex, Fragment
	};

	bool getCompileStatus(ShaderType type);
	std::string getCompileLog(ShaderType type);

	bool getProgramStatus();
	std::string getProgramLog();

	std::string output;
	bool is_compiled;

	GLuint vert;
	GLuint frag;
	GLuint program;
};