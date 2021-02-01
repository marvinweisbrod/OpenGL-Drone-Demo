#version 330 core

layout (location = 0) in vec2 dir;

uniform int numChars;
uniform float aspect;
uniform float textSize;
uniform vec2 textPos;
uniform int centered;

out vec2 vsUV;

void main(){
	// NOTE: For our calculations we are already in screenspace

	vsUV = dir; // use dir as UVs, since they mark the corners of the rectangle (0,0) (1,1)

	vec2 pos = dir;

	// center incoming coordinates if text has to be centered
	if(centered == 1){
		if(pos.x == 0.0) 
			pos.x = -0.5;
		else 
			pos.x = 0.5;

		if(pos.y == 0.0) 
			pos.y = -0.5;
		else 
			pos.y = 0.5;
	}

	pos.x = pos.x/aspect/2; // divided by 2 because a glyphs width is half its height
	pos.x = pos.x*numChars;
	pos = pos* textSize;
	pos = pos + textPos;
	gl_Position = vec4(pos, 0.0, 1.0);
};