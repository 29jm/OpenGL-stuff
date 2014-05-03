#include "Texture2D.hpp"

Texture2D::Texture2D(const std::string img_name, GLuint slot)
	: texture_index(0), texture_unit(slot), type(Type::FromFile)
{
	glActiveTexture(GL_TEXTURE0+texture_unit);
	glGenTextures(1, &texture_index);
	bind();

	if (!img.loadFromFile(img_name))
	{
		std::cout << "Error loading image" << std::endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y,
			 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture2D::Texture2D(GLuint w, GLuint h, GLuint slot)
	: texture_index(0), texture_unit(slot), type(Type::ImageBuffer),
	width(w), height(h)
{
	glActiveTexture(GL_TEXTURE0+texture_unit);
	glGenTextures(1, &texture_index);
	bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h,
		0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &texture_index);
}

void Texture2D::bind()
{
	glActiveTexture(GL_TEXTURE0+texture_unit);
	glBindTexture(GL_TEXTURE_2D, texture_index);
}

void Texture2D::linkTo(Shader shader, std::string name)
{
	shader.bind();
	glUniform1i(shader.uniform(name.c_str()), texture_unit);
}

void Texture2D::setTextureUnit(GLuint tex_unit)
{
	texture_unit = tex_unit;
}

GLuint Texture2D::index() const
{
	return texture_index;
}
