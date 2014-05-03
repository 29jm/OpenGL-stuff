#include "Shader.hpp"

using namespace std;

Shader::Shader()
	: is_compiled(false), vert(0), frag(0), program(0)
{

}

Shader::Shader(string vertex, string fragment)
	: is_compiled(false), vert(0), frag(0), program(0)
{
	vert = glCreateShader(GL_VERTEX_SHADER);
	frag = glCreateShader(GL_FRAGMENT_SHADER);

	ifstream vert_file(vertex);
	string vert_text((std::istreambuf_iterator<char>(vert_file)),
					 std::istreambuf_iterator<char>());

	ifstream frag_file(fragment);
	string frag_text((std::istreambuf_iterator<char>(frag_file)),
					 std::istreambuf_iterator<char>());

	const char* vert_char = vert_text.c_str();
	const char* frag_char = frag_text.c_str();

	glShaderSource(vert, 1, &vert_char, nullptr);
	glShaderSource(frag, 1, &frag_char, nullptr);

	compile();
}

Shader::~Shader()
{
	// Detach + delete
	glDeleteShader(vert);
	glDeleteShader(frag);
}

bool Shader::loadFromMemory(const GLchar* vertex, const GLchar* fragment)
{
	vert = glCreateShader(GL_VERTEX_SHADER);
	frag = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vert, 1, &vertex, nullptr);
	glShaderSource(frag, 1, &fragment, nullptr);

	return compile();
}

bool Shader::compile()
{
	program = glCreateProgram();

	// Compile shader
	glCompileShader(vert);
	glCompileShader(frag);

	is_compiled = true;

	// Checks for syntax errors
	if (!getCompileStatus(ShaderType::Vertex))
	{
		output += getCompileLog(ShaderType::Vertex);
		is_compiled = false;
	}

	if (!getCompileStatus(ShaderType::Fragment))
	{
		output += getCompileLog(ShaderType::Fragment);
		is_compiled = false;
	}

	// Attach + link
	glAttachShader(program, vert);
	glAttachShader(program, frag);
		glLinkProgram(program);
	glDetachShader(program, vert);
	glDetachShader(program, frag);

	// Checks for links error
	if (!getProgramStatus())
	{
		output += getProgramLog();
		is_compiled = false;
	}

	return is_compiled;
}

void Shader::bind()
{
	if (is_compiled)
	{
		glUseProgram(program);
	}
	else
	{
		std::cout << "ERROR: Shader not compiled" << std::endl;
	}
}

string Shader::getShaderLog()
{
	return output;
}

bool Shader::isCompiled()
{
	return is_compiled;
}

GLint Shader::uniform(string uni)
{
	bind();
	return glGetUniformLocation(program, uni.c_str());
}

GLuint Shader::index() const
{
	return program;
}

bool Shader::getCompileStatus(ShaderType type)
{
	GLint success;
	GLuint shader = (type == ShaderType::Vertex ? vert :
												  frag);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		return false;
	}

	return true;
}

string Shader::getCompileLog(ShaderType type)
{
	GLuint shader = (type == ShaderType::Vertex ? vert :
												  frag);
	const int max = 1024;
	int length = 0;
	char out[max];
	glGetShaderInfoLog(shader, max, &length, out);

	return string(out);
}

bool Shader::getProgramStatus()
{
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		return false;
	}

	return true;
}

string Shader::getProgramLog()
{
	const int max = 1024;
	int length = 0;
	char out[max];
	glGetProgramInfoLog(program, max, &length, out);

	return string(out);
}