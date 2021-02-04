#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 modelMat;
uniform mat4 modelMatInvT;
uniform mat4 viewMat;
uniform mat4 viewMatInv;
uniform mat4 projMat;
uniform float texUVScale;

out vec3 vsNormalN;
out vec3 vsPos;
out vec2 vsUV;

void main()
{
	vec4 modelPos = modelMat * vec4(pos, 1.0);
	gl_Position = projMat * viewMat * modelPos;
	vsPos = modelPos.xyz / modelPos.w;
	vsUV = uv * texUVScale;
	vsNormalN = normalize((modelMatInvT * vec4(normal, 0.0)).xyz);
}
