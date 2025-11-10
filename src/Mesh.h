#pragma once
#include <glad/glad.h>
#include <vector>
#include "raymath.h"

struct Mesh
{
	std::vector<Vector3> positions;
	std::vector<Vector2> tcoords;
	std::vector<Vector3> normals;
};

void LoadMesh(Mesh* mesh, const char* path);
void UnloadMesh(Mesh* mesh);