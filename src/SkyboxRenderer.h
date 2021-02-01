#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Texture2D.h"

class SkyboxRenderer {
public:
	SkyboxRenderer(ShaderProgram* skyboxShader, const std::vector<std::string>* faces);
	void render(Camera& camera);
private:
	ShaderProgram* m_shader;
	GLuint vaoID = 0;
	GLuint vboID = 0;
	Texture2D cubemapTexture;
};