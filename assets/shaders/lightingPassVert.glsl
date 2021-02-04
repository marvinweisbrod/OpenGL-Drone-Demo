#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

uniform mat4 viewMatInv;

out vec3 vsViewPos;
out vec2 vsUV;

void main()
{
    gl_Position = vec4(pos, 0.0f, 1.0);
	vsViewPos = (viewMatInv*vec4(0.0,0.0,0.0,1.0)).xyz;
    vsUV = uv;
}
