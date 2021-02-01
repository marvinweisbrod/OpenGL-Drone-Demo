#include "ShaderProgram.h"
#include "DirectionalLight.h"


ShaderProgram::ShaderProgram() :
	prog(0),
	currentTu(0)
{}

ShaderProgram::ShaderProgram(GLuint program) :
	prog(program),
	currentTu(0)
{}

ShaderProgram::ShaderProgram(ShaderProgram && other) :
	prog(other.prog),
	currentTu(0)
{
	other.prog = 0;
}

ShaderProgram & ShaderProgram::operator=(ShaderProgram && other)
{
	if (this == &other)
		return *this;

	if(prog)
		glDeleteProgram(prog);

	prog = other.prog;
	other.prog = 0;
	currentTu = other.currentTu;

	return *this;
}

ShaderProgram::~ShaderProgram()
{
	if(prog)
		glDeleteProgram(prog);
}

void ShaderProgram::use()
{
	resetTU();
	GLint current;
	glGetIntegerv(GL_CURRENT_PROGRAM, &current); GLERR
		if (current != prog && prog != 0)
			glUseProgram(prog); GLERR
}

void ShaderProgram::bind(Camera& camera)
{
	setUniform("viewMat", camera.getViewMatrix(), false);
	setUniform("viewMatInv", glm::inverse(camera.getViewMatrix()), false);
	setUniform("projMat", camera.getProjectionMatrix(), false);
}

void ShaderProgram::bind(PointLight& plight)
{
	auto pos = (plight.getTransformMatrix() * glm::vec4(0.0f,0.0f,0.0f,1.0f));
	setUniform("pointLightPos", pos);
	auto test = plight.getColor();
	setUniform("pointLightColor", test);
}

void ShaderProgram::bind(SpotLight& slight)
{
	auto pos = (slight.getTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	setUniform("spotLightPos", pos);
	auto test = slight.getColor();
	setUniform("spotLightColor", test);
	auto dir = (slight.getTransformMatrix() * glm::vec4(slight.getDirection(), 0.0f));
	setUniform("spotLightDir", dir);
	auto angles = slight.getAnglesInnerOuter();
	setUniform("spotLightAngles", angles);
	setUniform("spotLightAttenuation", slight.getAttenuation());
}

void ShaderProgram::bind(DirectionalLight& dlight)
{
	auto dir = dlight.getZAxis();
	setUniform("directionalLightDir", dir);
	setUniform("directionalLightColor", dlight.getColor());
}

GLuint ShaderProgram::getFreeTU()
{
	return currentTu++;
}

GLuint ShaderProgram::getCurrentTU()
{
	return currentTu;
}

void ShaderProgram::resetTU(GLuint tu)
{
	currentTu = tu;
}
