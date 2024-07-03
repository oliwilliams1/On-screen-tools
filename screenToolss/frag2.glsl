#version 330 core

in vec2 UV;
out vec4 fragColour;

uniform float time;
uniform vec2 mousePos;
uniform sampler2D texture_sampler;

void main()
{
	vec2 correctedUV = vec2(1 - UV.y, 1 - UV.x); // Flipped diagonally + both axis
	fragColour = vec4(texture2D(texture_sampler, correctedUV).rgb, 1.0);
}