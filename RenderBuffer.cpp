#include "RenderBuffer.hpp"

RenderBuffer::RenderBuffer(GLuint w,  GLuint h)
	: rbo_index(0), width(w), height(h)
{
	glGenRenderbuffers(1, &rbo_index);
	bind();

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

RenderBuffer::~RenderBuffer()
{
	glDeleteRenderbuffers(1, &rbo_index);
}

void RenderBuffer::bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_index);
}

GLuint RenderBuffer::index() const
{
	return rbo_index;
}