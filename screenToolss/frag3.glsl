#version 330 core

in vec2 UV;
out vec4 fragColour;

uniform float time;
uniform vec2 mousePos;
uniform sampler2D texture_sampler;

void main()
{
	fragColour = vec4(vec3(1.0f), 1.0);
}