#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer()
	: fbo_index(0)
{
	glGenFramebuffers(1, &fbo_index);
	bind();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &fbo_index);
}

void FrameBuffer::attach(const Texture2D& texture)
{
	bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, texture.index(), 0);
}

void FrameBuffer::attach(const RenderBuffer& renderbuffer)
{
	bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER, renderbuffer.index());
}

bool FrameBuffer::isComplete()
{
	bind();
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_index);
}

GLuint FrameBuffer::index() const
{
	return fbo_index;
}

void FrameBuffer::bindDefault()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}