#include "Mesh.h"
#include "Buffer.h"

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
    DestroyVertexArray(&mesh->vao);
    DestroyBuffer(&mesh->pbo);
    DestroyBuffer(&mesh->tbo);
    DestroyBuffer(&mesh->nbo);
    DestroyBuffer(&mesh->ibo);

    mesh->positions.resize(0);
    mesh->tcoords.resize(0);
    mesh->normals.resize(0);
    mesh->indices.resize(0);

    mesh->vertex_count = -1;
}

void LoadMeshPlane(Mesh* mesh)
{
    mesh->vertex_count = 6;

    mesh->positions.resize(4);
    mesh->tcoords.resize(4);
    mesh->normals.resize(4);
    mesh->indices.resize(6);

    mesh->positions[0] = { -0.5f, -0.5f, 0.0f };
    mesh->positions[1] = {  0.5f, -0.5f, 0.0f };
    mesh->positions[2] = {  0.5f,  0.5f, 0.0f };
    mesh->positions[3] = {  -0.5f, 0.5f, 0.0f };

    mesh->tcoords[0] = { 0.0f, 0.0f };
    mesh->tcoords[1] = { 1.0f, 0.0f };
    mesh->tcoords[2] = { 1.0f, 1.0f };
    mesh->tcoords[3] = { 0.0f, 1.0f };

    mesh->normals[0] = Vector3UnitZ;
    mesh->normals[1] = Vector3UnitZ;
    mesh->normals[2] = Vector3UnitZ;
    mesh->normals[3] = Vector3UnitZ;

    mesh->indices[0] = 0;
    mesh->indices[1] = 1;
    mesh->indices[2] = 2;
    mesh->indices[3] = 0;
    mesh->indices[4] = 2;
    mesh->indices[5] = 3;

    mesh->pbo = CreateBuffer();
    BindVertexBuffer(mesh->pbo);
        UpdateVertexBuffer(mesh->positions.data(), mesh->positions.size() * sizeof(Vector3));
    UnbindVertexBuffer(mesh->pbo);

    mesh->tbo = CreateBuffer();
    BindVertexBuffer(mesh->tbo);
        UpdateVertexBuffer(mesh->tcoords.data(), mesh->tcoords.size() * sizeof(Vector2));
    UnbindVertexBuffer(mesh->tbo);

    mesh->nbo = CreateBuffer();
    BindVertexBuffer(mesh->nbo);
        UpdateVertexBuffer(mesh->normals.data(), mesh->normals.size() * sizeof(Vector3));
    UnbindVertexBuffer(mesh->nbo);

    mesh->ibo = CreateBuffer();
    BindElementBuffer(mesh->ibo);
        UpdateElementBuffer(mesh->indices.data(), mesh->indices.size() * sizeof(unsigned int));
    UnbindElementBuffer(mesh->ibo);

    mesh->vao = CreateVertexArray();
    BindVertexArray(mesh->vao);

    BindElementBuffer(mesh->ibo);

    EnableVertexAttribute(0);
    EnableVertexAttribute(1);
    EnableVertexAttribute(2);

    BindVertexBuffer(mesh->pbo);
        SetVertexAttribute(0, 3, GL_FLOAT, sizeof(Vector3));
    UnbindVertexBuffer(mesh->pbo);

    BindVertexBuffer(mesh->tbo);
        SetVertexAttribute(1, 2, GL_FLOAT, sizeof(Vector2));
    UnbindVertexBuffer(mesh->tbo);

    BindVertexBuffer(mesh->nbo);
        SetVertexAttribute(2, 3, GL_FLOAT, sizeof(Vector3));
    UnbindVertexBuffer(mesh->nbo);

    UnbindVertexArray(mesh->vao);
}

void DrawMesh(const Mesh& mesh)
{
    BindVertexArray(mesh.vao);
    if (mesh.ibo != GL_NONE)
        glDrawElements(GL_TRIANGLES, mesh.vertex_count, GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count);
    UnbindVertexArray(mesh.vao);
}
