#pragma once
#include <glad/glad.h>
#include "raymath.h"

GLuint CreateVertexArray();
GLuint CreateVertexBuffer();

void DestroyVertexArray(GLuint* vao);
void DestroyVertexBuffer(GLuint* vbo);

void BindVertexBuffer(GLuint vbo);
void UnbindVertexBuffer();

void BindVertexArray(GLuint vao);
void UnbindVertexArray();

void EnableVertexAttribute(GLuint index);
void DisableVertexAttribute(GLuint index);

void SetVertexAttribute(GLuint index, GLint compSize, GLenum type, GLsizei stride);
void UpdateVertexBuffer(GLuint vbo, void* data, int data_size);