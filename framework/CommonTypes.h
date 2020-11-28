#ifndef _COMMON_TYPES_H_
#define _COMMON_TYPES_H_
#include <libheaders.h>

struct VertexAttribute
{
	VertexAttribute() {};
	VertexAttribute(GLint n_, GLenum type_, GLsizei stride_, GLintptr offset_)
	: n(n_), type(type_), stride(stride_), offset(offset_){};
	GLint n;
	GLenum type;
	GLsizei stride;
	GLintptr offset;
};

struct Vertex
{
	Vertex() {};
	Vertex(glm::vec3 pos) : position(pos) {};
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec3 tangent;
};

typedef GLuint Index;

#endif