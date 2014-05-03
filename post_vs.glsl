#version 430

in vec2 position;
in vec2 texCoord;

out vec2  passTexcoord;

void main()
{
	passTexcoord = texCoord;
	gl_Position = vec4(position, 0.0, 1.0);
}