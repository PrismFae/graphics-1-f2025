#include "Buffer.h"

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
