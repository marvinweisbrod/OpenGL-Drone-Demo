#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<VertexAttribute>& vertexAttributes, std::vector<Index>& indices, std::string texDiff, std::string texSpec, std::string texEmss, const Bounds& bounds, float uvScale , float shine)
	: bounds(bounds)
	, m_texDiff(texDiff, true)
	, m_texSpec(texSpec, true)
	, m_texEmss(texEmss, true)
	, m_shine(shine)
	, m_uvScale(uvScale)
{
	// create buffers
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &iboID);

	// bind VAO
	glBindVertexArray(vaoID);

	// configure VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	for (size_t i = 0; i < vertexAttributes.size(); ++i) {
		auto& va = vertexAttributes.at(i);
		glVertexAttribPointer(static_cast<GLuint>(i), va.n, va.type, GL_FALSE, va.stride, (void*)va.offset);
		glEnableVertexAttribArray(static_cast<GLuint>(i));
	}

	// configure element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Index) * indices.size(), indices.data(), GL_STATIC_DRAW);
	indexCount = static_cast<GLuint>(indices.size());
	// unbind vao
	glBindVertexArray(0);
}

void Mesh::render(ShaderProgram& shader)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texDiff.getTexId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texSpec.getTexId());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_texEmss.getTexId());

	shader.setUniform("shine", m_shine);
	shader.setUniform("texUVScale", m_uvScale);
	shader.setUniform("texDiffuse", 0);
	shader.setUniform("texSpecular", 1);
	shader.setUniform("texEmissive", 2);

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	std::cout << "Mesh destroyed\n";
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &iboID);
}