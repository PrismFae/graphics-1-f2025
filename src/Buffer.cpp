#include "Buffer.h"

GLuint GenVertexArray()
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	return vao;
}

GLuint GenVertexBuffer()
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	return vbo;
}

void BindVertexBuffer(GLuint vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void UnbindVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

void BindVertexArray(GLuint vao)
{
	glBindVertexArray(vao);
}

void UnbindVertexArray()
{
	glBindVertexArray(GL_NONE);
}

void EnableVertexAttribute(GLuint index)
{
	glEnableVertexAttribArray(index);
}

void DisableVertexAttribute(GLuint index)
{
	glDisableVertexAttribArray(index);
}

void SetVertexAttribute(GLuint index, GLint compSize, GLenum type, GLsizei stride/*, int offset*/)
{
	// No interleaved vertex data in order to simplify our interface (offset is always nullptr)
	glVertexAttribPointer(index, compSize, type, GL_FALSE, stride, nullptr);
}
