// Headers
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "Vao.hpp"
#include "RenderBuffer.hpp"
#include "FrameBuffer.hpp"

using namespace std;

string getErrorString(GLenum err_code);

// Shader sources
const GLchar* sceneVertexSource =
	"#version 150 core\n"
	"in vec3 position;"
	"in vec3 color;"
	"in vec2 texcoord;"
	"out vec3 Color;"
	"out vec2 Texcoord;"
	"uniform mat4 model;"
	"uniform mat4 view;"
	"uniform mat4 proj;"
	"uniform vec3 overrideColor;"
	"void main() {"
	"   Color = overrideColor * color;"
	"   Texcoord = texcoord;"
	"   gl_Position = proj * view * model * vec4(position, 1.0);"
	"}";
const GLchar* sceneFragmentSource =
	"#version 150 core\n"
	"in vec3 Color;"
	"in vec2 Texcoord;"
	"out vec4 outColor;"
	"uniform sampler2D texKitten;"
	"uniform sampler2D texPuppy;"
	"void main() {"
	"   outColor = vec4(Color, 1.0) * mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), 0.5);"
	"}";

const GLchar* screenVertexSource =
	"#version 150 core\n"
	"in vec2 position;"
	"in vec2 texcoord;"
	"out vec2 Texcoord;"
	"void main() {"
	"   Texcoord = texcoord;"
	"   gl_Position = vec4(position, 0.0, 1.0);"
	"}";
const GLchar* screenFragmentSource =
	"#version 150 core\n"
	"in vec2 Texcoord;"
	"out vec4 outColor;"
	"uniform sampler2D texFramebuffer;"
	"void main() {"
	"   outColor = texture(texFramebuffer, Texcoord);"
	"}";

// Cube vertices
GLfloat cubeVertices[] = {
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

	-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	 1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

// Quad vertices
GLfloat quadVertices[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

int main()
{
	sf::Window window(sf::VideoMode(800, 600, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Create VAOs
	Vao vaoCube(cubeVertices, sizeof(cubeVertices));
	Vao vaoQuad(quadVertices, sizeof(quadVertices));

	// Create shader programs
	Shader sceneShaderProgram;
	sceneShaderProgram.loadFromMemory(sceneVertexSource, sceneFragmentSource);

	Shader screenShaderProgram;
	screenShaderProgram.loadFromMemory(screenVertexSource, screenFragmentSource);

	// Specify the layout of the vertex data
	vaoCube.addAttribute(0, 3, 8, 0);
	vaoCube.addAttribute(1, 3, 8, 3);
	vaoCube.addAttribute(2, 2, 8, 6);

	vaoQuad.addAttribute(0, 2, 4, 0);
	vaoQuad.addAttribute(1, 2, 4, 2);

	// Load textures
	Texture2D texKitten("sample.png", 0);
	Texture2D texPuppy("sample2.png", 1);

	texKitten.linkTo(sceneShaderProgram, "texKitten");
	texPuppy.linkTo(sceneShaderProgram, "texPuppy");

	glUniform1i(screenShaderProgram.uniform("texFramebuffer"), 0);

	GLint uniModel = sceneShaderProgram.uniform("model");

	// Create frame buffer
	FrameBuffer framebuffer;
	Texture2D texColorBuffer(800, 600, 0);
	RenderBuffer rboDepthStencil(800, 600);

	framebuffer.attach(texColorBuffer);
	framebuffer.attach(rboDepthStencil);
	
	sceneShaderProgram.bind();

	// Set up projection
	glm::mat4 view = glm::lookAt(
		glm::vec3(2.5f, 2.5f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	GLint uniView = sceneShaderProgram.uniform("view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);
	GLint uniProj = sceneShaderProgram.uniform("proj");;
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	GLint uniColor = sceneShaderProgram.uniform("overrideColor");;

	while (window.isOpen())
	{
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (windowEvent.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			default:
				break;
			}
		}

		// Bind our framebuffer and draw 3D scene (spinning cube)
		glEnable(GL_DEPTH_TEST);
		framebuffer.bind();
		vaoCube.bind();
		sceneShaderProgram.bind();

		texKitten.bind();
		texPuppy.bind();

		// Clear the screen to white
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate transformation
		glm::mat4 model;
		model = glm::rotate(
			model,
			(GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC * 180.0f,
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		// Draw cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glEnable(GL_STENCIL_TEST);
			// Draw floor
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF);
			glDepthMask(GL_FALSE);
			glClear(GL_STENCIL_BUFFER_BIT);
			
			glDrawArrays(GL_TRIANGLES, 36, 6);

			// Draw cube reflection
			glStencilFunc(GL_EQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDepthMask(GL_TRUE);

			model = glm::scale(glm::translate(model, glm::vec3(0, 0, -1)), glm::vec3(1, 1, -1));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

			glUniform3f(uniColor, 0.3f, 0.3f, 0.3f);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);
		glDisable(GL_STENCIL_TEST);

		// Bind default framebuffer and draw contents of our framebuffer
		glDisable(GL_DEPTH_TEST);
		FrameBuffer::bindDefault();
		vaoQuad.bind();
		screenShaderProgram.bind();
		texColorBuffer.bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Swap buffers
		window.display();
	}
}

string getErrorString(GLenum err_code)
{
	switch (err_code)
	{
	case 0:
		return "GL_NO_ERROR";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_TABLE_TOO_LARGE:
		return "GL_TABLE_TOO_LARGE";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	}

	return "OTHER";
}