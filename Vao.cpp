#include "Vao.hpp"

Vao::Vao(GLfloat* vert, GLuint size)
	: vao_index(0), vbo_index(0), vertices(vert)
{
	glGenBuffers(1, &vbo_index);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_index);
	glBufferData(GL_ARRAY_BUFFER, size, vert, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao_index);
	glBindVertexArray(vao_index);
}

Vao::~Vao()
{
	glDeleteBuffers(1, &vbo_index);
	glDeleteVertexArrays(1, &vao_index);
}

void Vao::addAttribute(GLuint index, GLuint size, GLuint stride, GLuint offset)
{
	bind();

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride*sizeof(GLfloat), (void*)(offset*sizeof(GLfloat)));
}

void Vao::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_index);
	glBindVertexArray(vao_index);
}

GLuint Vao::index() const
{
	return vao_index;
}