#version 430

in vec3 passColor;
in vec2 passTexcoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D texKitten;
uniform sampler2D texPuppy;

void main()
{
	vec4 texColor = mix(texture(texKitten, passTexcoord),
                     texture(texPuppy, passTexcoord), 0.5);
	outColor = vec4(passColor, 1.0) * texColor;
}