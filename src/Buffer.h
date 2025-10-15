#pragma once
#include <glad/glad.h>
#include "raymath.h"

void BindVertexBuffer(GLuint vbo);
void UnbindVertexBuffer();

void BindVertexArray(GLuint vao);
void UnbindVertexArray();

void EnableVertexAttribute(GLuint index);
void DisableVertexAttribute(GLuint index);

void SetVertexAttribute(GLuint index, GLint compSize, GLenum type, GLsizei stride);