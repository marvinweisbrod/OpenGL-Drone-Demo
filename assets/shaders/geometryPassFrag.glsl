#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gEmissive;
layout (location = 4) out vec4 gSpecShine;

uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;
uniform sampler2D texEmissive;
uniform float shine;

in vec3 vsNormalN;
in vec3 vsPos;
in vec2 vsUV;

void main()
{
	gPosition = vec4(vsPos, 1.0f);
	gNormal = vec4(normalize(vsNormalN), 1.0f);
	gAlbedo = texture(texDiffuse, vsUV);
	gEmissive = texture(texEmissive, vsUV);
	gSpecShine.x = texture(texSpecular, vsUV).r;
	gSpecShine.y = shine / 100.0f;
	gSpecShine.a = 1.0f;
}
