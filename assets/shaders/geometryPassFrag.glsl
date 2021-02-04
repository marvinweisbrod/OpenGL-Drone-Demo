#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gEmissiveShine;

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
	gAlbedoSpec.rgb = texture(texDiffuse, vsUV).rgb;
	gAlbedoSpec.a = texture(texSpecular, vsUV).r;
	gEmissiveShine.rgb = texture(texEmissive, vsUV).rgb;
	gEmissiveShine.a = shine;
}


/*
out vec4 color;

//fragment shader output
uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;
uniform sampler2D texEmissive;
uniform float shine;

uniform vec4 ambient;
uniform vec4 pointLightPos;
uniform vec4 pointLightColor;
uniform vec2 spotLightAngles;
uniform vec4 spotLightPos;
uniform vec4 spotLightDir;
uniform vec4 spotLightColor;
uniform vec3 spotLightAttenuation;
uniform vec3 directionalLightDir;
uniform vec4 directionalLightColor;

in vec3 vsNormalN;
in vec3 vsPos;
in vec2 vsUV;
in vec3 vsViewPos;

vec4 calculatePointLight(vec4 colorDiff, vec4 colorSpec, vec3 lightPos, vec4 lightColor){
	vec3 lightDirN = normalize(lightPos-vsPos);
	vec3 cameraDirN = normalize(vsViewPos-vsPos);
	vec3 lightDirReflectN = normalize(reflect(-lightDirN, vsNormalN));
	
	float cosa = dot(vsNormalN, lightDirN);
	float cosBeta = dot(lightDirReflectN, cameraDirN);
	float cosBetak = pow(cosBeta, shine);
	
	vec4 diffuseResult = colorDiff * max(cosa, 0.0);
	vec4 specularResult = colorSpec * max(cosBetak, 0.0);
	return (diffuseResult + specularResult) * lightColor;
}

vec4 calculateSpotLight(vec4 colorDiff, vec4 colorSpec, vec3 lightPos, vec4 lightColor, vec3 spotDir, vec2 lightAngles){
	vec3 lightToPixel = lightPos - vsPos;
	float d = length(lightToPixel);
	vec3 lightDirN = lightToPixel / d;
	float degreeDiff = acos(dot(lightDirN, normalize(-spotDir)));
	
	if(degreeDiff > lightAngles.y) {return colorDiff*vec4(0.0,0.0,0.0,1.0);};
	
	vec3 cameraDirN = normalize(vsViewPos-vsPos);
	vec3 lightDirReflectN = normalize(reflect(-lightDirN, vsNormalN));
	
	float cosa = dot(vsNormalN, lightDirN);
	float cosBeta = dot(lightDirReflectN, cameraDirN);
	float cosBetak = pow(cosBeta, shine);
	
	vec4 diffuseResult = colorDiff * max(cosa, 0.0);
	vec4 specularResult = colorSpec * max(cosBetak, 0.0);
	vec4 result = diffuseResult + specularResult;

	// Calculate smooth / soft edges
	float theta = dot(lightDirN, normalize(-spotDir));
	float epsilon = cos(spotLightAngles.x) - cos(spotLightAngles.y);
	float intensity = clamp((theta - cos(spotLightAngles.y)) / epsilon, 0.0, 1.0);
	result *= intensity;

	result /= (spotLightAttenuation.x + (spotLightAttenuation.y * d)) + (spotLightAttenuation.z * (d * d));

	return result * lightColor;
}

vec4 calculateDirectionalLight(vec4 colorDiff, vec4 colorSpec, vec4 lightColor, vec3 lightDir) {
	vec3 lightDirN = normalize(-lightDir);
	vec3 cameraDirN = normalize(vsViewPos-vsPos);
	vec3 lightDirReflectN = normalize(reflect(-lightDirN, vsNormalN));
	
	float cosa = dot(vsNormalN, lightDirN);
	float cosBeta = dot(lightDirReflectN, cameraDirN);
	float cosBetak = pow(cosBeta, shine);
	
	vec4 diffuseResult = colorDiff * max(cosa, 0.0);
	vec4 specularResult = colorSpec * max(cosBetak, 0.0);
	return (diffuseResult + specularResult) * lightColor;
	
//	float NdotL = max(dot(vsNormalN, lightDir), 0.0);
//	return lightColor * colorDiff * NdotL;
}

vec4 calculateLit(vec4 colorDiff, vec4 colorSpec){
	vec4 total;
	
	total += calculatePointLight(colorDiff, colorSpec, pointLightPos.xyz, pointLightColor);
	
	total += calculateSpotLight(colorDiff, colorSpec, spotLightPos.xyz, spotLightColor, spotLightDir.xyz, spotLightAngles);

	total += calculateDirectionalLight(colorDiff, colorSpec, directionalLightColor, directionalLightDir.xyz);

	return total;
}


void main(){
	vec4 colorDiff = texture(texDiffuse, vsUV);
	vec4 colorSpec = texture(texSpecular, vsUV);
	vec4 colorEmss = texture(texEmissive, vsUV);
	
	
    vec4 sum = colorEmss + (colorDiff * ambient) + calculateLit(colorDiff, colorSpec);
	color = clamp(sum, vec4(0.0,0.0,0.0,0.0),vec4(1.0,1.0,1.0,1.0));
}
*/
