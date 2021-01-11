#pragma once

#include "Texture2D.h"
#include "ShaderProgram.h"
#include "CommonTypes.h"
#include <libheaders.h>
#include <vector>


class Mesh {
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<VertexAttribute>& vertexAttributes, std::vector<Index>& indices, std::string texDiff, std::string texSpec, std::string texEmss, float uvScale = 1.0f, float shine = 64);
	~Mesh();
	void render(ShaderProgram& shader);
private:
	GLuint vaoID = 0;
	GLuint vboID = 0; 
	GLuint iboID = 0;
	GLuint indexCount = 0;
	float m_shine;
	float m_uvScale;
	Texture2D m_texDiff;
	Texture2D m_texSpec;
	Texture2D m_texEmss;
};