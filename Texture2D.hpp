#pragma once

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include "Shader.hpp"

class Texture2D
{
public:
	Texture2D(const std::string img_name, GLuint slot);
	Texture2D(const GLuint w, const GLuint h, GLuint slot);
	~Texture2D();

	void bind();
	void linkTo(Shader shader_program, std::string);
	void setTextureUnit(GLuint tex_unit);
	GLuint index() const;

private:
	enum class Type {
		FromFile, ImageBuffer
	};

	GLuint texture_index;
	GLuint texture_unit;
	sf::Image img;
	Type type;

	GLuint width;
	GLuint height;
};