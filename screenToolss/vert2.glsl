#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;
out vec2 UV;

uniform vec2 mousePos;
uniform float aspectRatio;

void main()
{
    gl_Position = vec4( mousePos.x + position.x,
                       -mousePos.y + (position.y * aspectRatio) + 1, 
                       0.0, 1.0);
    UV = uv;
}