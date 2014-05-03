#pragma once

#include <string>
#include <iostream>
#include <GL/glew.h>

class Vao
{
public:
	Vao(GLfloat* vert, GLuint size);
	~Vao();

	void addAttribute(GLuint index, GLuint size, GLuint stride, GLuint offset);
	void bind();
	GLuint index() const;

private:
	GLuint vao_index;
	GLuint vbo_index;

	const GLfloat* vertices;
};