#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<VertexAttribute>& vertexAttributes, std::vector<Index>& indices)
	: m_vertices(vertices)
	, m_vertexAttributes(vertexAttributes)
	, m_indices(indices)
{
}

void Mesh::initialize()
{
	if (initialized)
		return;

	// create buffers
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &eboID);

	// bind VAO
	glBindVertexArray(vaoID);

	// configure VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
	for (size_t i = 0; i < m_vertexAttributes.size(); ++i) {
		auto& va = m_vertexAttributes.at(i);
		glVertexAttribPointer(static_cast<GLuint>(i), va.n, va.type, GL_FALSE, va.stride, (void*)va.offset);
		glEnableVertexAttribArray(static_cast<GLuint>(i));
	}

	// configure element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	auto size = sizeof(m_indices);
	std::cout << size << "\n";
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Index)*m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

	// unbind vao
	glBindVertexArray(0);

	initialized = true;
}

void Mesh::render()
{
	if (!initialized) {
		std::cout << "INFO:\t Attempting render when mesh is not initialized.\n";
		return;
	}

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(m_indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	std::cout << "Mesh destroyed\n";
	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &eboID);
}
