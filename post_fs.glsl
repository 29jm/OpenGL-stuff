#version 430

in vec2 passTexcoord;
out vec4 outColor;

uniform sampler2D texFramebuffer;

void main()
{
	outColor = texture(texFramebuffer, passTexcoord);
}