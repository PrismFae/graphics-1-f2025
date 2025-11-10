#pragma once
#include <glad/glad.h>
#include <vector>
#include "raymath.h"

// Extra practice 1:
// Transform the plane within main.cpp's position/tcoord/normal data into this Mesh structure.
// Add the corresponding vertex buffers to this Mesh structure so it has everything to store a mesh on both the CPU and GPU!

// Extra practice 2:
// Have a look at the fastObjMesh data-type.
// See if you can transform the data loaded into fastObjMesh to the data the GPU expects!
struct Mesh
{
	std::vector<Vector3> positions;
	std::vector<Vector2> tcoords;
	std::vector<Vector3> normals;
};

void LoadMesh(Mesh* mesh, const char* path);
void UnloadMesh(Mesh* mesh);