#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

uniform float color;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 colorVS;

vec3 hsv2rgb(vec3 c)
{
	// https://stackoverflow.com/questions/15095909/from-rgb-to-hsv-in-opengl-glsl
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 colorFromFloat(float number){
	return hsv2rgb(vec3(number, 1.0, 1.0));
}

void main(){
    //colorVS = colorFromFloat(color* clamp(((pos.x + pos.y + pos.z + 3)/6), 0.0, 1.0));
	colorVS = colorFromFloat(clamp(((pos.x + pos.y + pos.z + 3)/6), 0.0, 1.0));
    gl_Position = projMat * viewMat * modelMat * vec4(pos.x, pos.y, pos.z, 1.0);

}
