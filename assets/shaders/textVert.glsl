#version 330 core

layout (location = 0) in vec2 dir;

uniform int numChars;
uniform float aspect;
uniform float textSize;
uniform vec2 textPos;
uniform vec2 positioning;

out vec2 vsUV;

void main(){
	// NOTE: For our calculations we are already in screenspace

	vsUV = dir; // use dir as UVs, since they mark the corners of the rectangle (0,0) (1,1)

	vec2 pos = dir;

	if(positioning.x == 0){
		pos.x -= 0.5;
	}
	if(positioning.x == 1.0){
		pos.x -= 1.0;
	}
	if(positioning.y == 0){
		pos.y -= 0.5;
	}
	if(positioning.y == 1.0){
		pos.y -= 1.0;
	}

	pos.x = pos.x/aspect/2; // divided by 2 because a glyphs width is half its height
	pos.x = pos.x*numChars;
	pos = pos* textSize;
	pos = pos + textPos;
	gl_Position = vec4(pos, 0.0, 1.0);
};