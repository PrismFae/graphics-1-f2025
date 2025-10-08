#pragma once
#include <glad/glad.h>
#include "raymath.h"

GLuint CreateShader(GLint type, const char* path);
GLuint CreateProgram(GLuint vs, GLuint fs);

void BeginShader(GLuint shader);
void EndShader();

void SendVec3(Vector3 value, const char* name);
