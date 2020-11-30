#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<VertexAttribute>& vertexAttributes, std::vector<Index>& indices)
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
	auto size = sizeof(indices);
	std::cout << size << "\n";
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Index) * indices.size(), indices.data(), GL_STATIC_DRAW);
	indexCount = static_cast<GLuint>(indices.size());
	// unbind vao
	glBindVertexArray(0);
}

void Mesh::render()
{
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
