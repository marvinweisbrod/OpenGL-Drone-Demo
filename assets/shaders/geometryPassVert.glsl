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

/*
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

uniform mat4 modelMat;
uniform mat4 modelMatInvT;
uniform mat4 viewMat;
uniform mat4 viewMatInv;
uniform mat4 projMat;
uniform float texUVScale;

out vec3 vsNormalN;
out vec3 vsPos;
out vec2 vsUV;
out vec3 vsViewPos;

void main(){
	vec4 modelPos = modelMat * vec4(pos.x, pos.y, pos.z, 1.0);
    gl_Position = projMat * viewMat * modelPos;
	vsPos = modelPos.xyz;
	vsUV = uv*texUVScale;
	vsViewPos = (viewMatInv*vec4(0.0,0.0,0.0,1.0)).xyz;
	vsNormalN = normalize((modelMatInvT * vec4(normal,1.0)).xyz);
}
*/
