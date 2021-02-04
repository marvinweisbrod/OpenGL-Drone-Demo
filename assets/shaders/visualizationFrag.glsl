#version 330 core

out vec4 color;

uniform mat4 channels;
uniform vec4 additive;
uniform sampler2D gTexture;

in vec2 vsUV;

void main(){
	vec4 sampled = texture(gTexture, vsUV);
	color = channels * sampled + additive;
}
