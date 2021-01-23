#version 330 core

out vec4 color_out;

uniform sampler2D fontTex;
uniform int text[64];
uniform vec4 color;
uniform int numChars;

in vec2 vsUV;

void main(){
	if(numChars < 1){ // catching potential problems
		color_out = vec4(0.0);
		return;
	}

	
	
	// which position in the text am I?
	int myPosition = int(floor(vsUV.x*numChars));

	// special case right edge of the rectangle
	if(myPosition == numChars) myPosition = myPosition - 1;
	
	int myChar = text[myPosition]-32; // first character on the texture is SPACE (ascii 32)
	int myRow = int(floor(float(myChar)/32.0));
	int myColumn = myChar - myRow*32;

	float step = 1.0/numChars;

	// Spread UVs back to [0,1]
	float myU = (vsUV.x - (step*myPosition)) / step;
	vec2 myUV = vec2(myU, -vsUV.y+1.0); // invert y so we can pretend the top left of the texture is (0,0). makes it easier

	float glyphSizeOnTexX = 1.0/32;
	float glyphSizeOnTexY = 1.0/16;

	float trueU = myColumn*glyphSizeOnTexX + myUV.x*glyphSizeOnTexX;
	float trueV = myRow*glyphSizeOnTexY + myUV.y*glyphSizeOnTexY;

	vec2 trueUVs = vec2(trueU, (trueV*-1)+1);




	vec4 colorFromTex = texture(fontTex, trueUVs);
	color_out = colorFromTex*color;
}