#version 330 core
in vec2 UV;
out vec4 fragColour;

void main() 
{
	fragColour = vec4(UV.xy, 0.0f, 1.0f);
}