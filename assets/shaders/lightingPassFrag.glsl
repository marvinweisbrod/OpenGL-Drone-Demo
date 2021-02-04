#version 330 core

out vec4 color;

//fragment shader output
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gEmissive;
uniform sampler2D gSpecShine;

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

in vec3 vsViewPos;
in vec2 vsUV;

vec4 calculatePointLight(vec4 colorDiff, vec4 colorSpec, vec3 lightPos, vec4 lightColor, vec3 vsPos, vec3 vsNormalN, float shine){
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

vec4 calculateSpotLight(vec4 colorDiff, vec4 colorSpec, vec3 lightPos, vec4 lightColor, vec3 spotDir, vec2 lightAngles, vec3 vsPos, vec3 vsNormalN, float shine){
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

vec4 calculateDirectionalLight(vec4 colorDiff, vec4 colorSpec, vec4 lightColor, vec3 lightDir, vec3 vsPos, vec3 vsNormalN, float shine) {
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

vec4 calculateLit(vec4 colorDiff, vec4 colorSpec, vec3 vsPos, vec3 vsNormalN, float shine){
	vec4 total;
	
	total += calculatePointLight(colorDiff, colorSpec, pointLightPos.xyz, pointLightColor, vsPos, vsNormalN, shine);
	
	total += calculateSpotLight(colorDiff, colorSpec, spotLightPos.xyz, spotLightColor, spotLightDir.xyz, spotLightAngles, vsPos, vsNormalN, shine);

	total += calculateDirectionalLight(colorDiff, colorSpec, directionalLightColor, directionalLightDir.xyz, vsPos, vsNormalN, shine);

	return total;
}

void main(){
	vec3 pos = texture(gPosition, vsUV).xyz;
	vec3 normal = texture(gNormal, vsUV).xyz;
	vec4 albedo = texture(gAlbedo, vsUV);
	vec4 emissive = texture(gEmissive, vsUV);
	vec4 specShine = texture(gSpecShine, vsUV);
	float spec = specShine.x;
	float shine = specShine.y * 100;

	vec4 colorDiff = albedo;
	vec4 colorSpec = vec4(spec, spec, spec, 1.0);
	vec4 colorEmss = emissive;
	
    vec4 sum = colorEmss + (colorDiff * ambient) + calculateLit(colorDiff, colorSpec, pos, normal, shine);
	color = clamp(sum, vec4(0.0,0.0,0.0,0.0),vec4(1.0,1.0,1.0,1.0));
}
