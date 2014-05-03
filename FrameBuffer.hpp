#pragma once

#include "Texture2D.hpp"
#include "RenderBuffer.hpp"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void attach(const Texture2D& texture); // TODO: add possibility to use GL_ATTACHMENT1...
	void attach(const RenderBuffer& renderbuffer);

	bool isComplete();

	void bind();
	GLuint index() const;

	static void bindDefault();

private:
	GLuint fbo_index;
};