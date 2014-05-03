#pragma once

#include <GL/glew.h>

class RenderBuffer
{
public:
	RenderBuffer(GLuint w,  GLuint h);
	~RenderBuffer();

	void bind();
	GLuint index() const;

private:
	GLuint rbo_index;
	GLuint width;
	GLuint height;
};