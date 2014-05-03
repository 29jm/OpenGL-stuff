#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 colorModifier;

out vec3 passColor;
out vec2 passTexcoord;

void main()
{
	passColor = color * colorModifier;
	passTexcoord = texcoord;
    gl_Position = proj*view*model*vec4(position, 1.0);
}