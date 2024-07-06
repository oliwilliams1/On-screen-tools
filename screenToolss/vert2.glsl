#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;
out vec2 UV;

uniform vec2 mousePos;
uniform vec2 widthHeight;

void main()
{
	vec2 pos = position * widthHeight;
    gl_Position = vec4( mousePos.x + pos.x,
                       -mousePos.y + pos.y + 1, 
                       0.0, 1.0);
    UV = uv;
}