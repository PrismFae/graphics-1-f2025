#include "Mesh.h"

#define FAST_OBJ_IMPLEMENTATION
#include <fast_obj/fast_obj.h>

void LoadMesh(Mesh* mesh, const char* path)
{
	fastObjMesh* obj = fast_obj_read(path);

	// LoadMesh transforms an internal fastObjMesh into our renderer's unified Mesh type
	fast_obj_destroy(obj);
}

void UnloadMesh(Mesh* mesh)
{
	// TODO -- Destroy CPU and GPU resources here
}
