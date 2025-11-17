#include "Window.h"
#include "Shader.h"
#include "Mesh.h"

#include <imgui/imgui.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>

enum ShaderType
{
    SHADER_POSITION_COLOR,
    SHADER_TCOORD_COLOR,
    SHADER_NORMAL_COLOR,
    SHADER_TYPE_COUNT
};

enum MeshType
{
    MESH_TETRAHEDRON,
    MESH_CUBE,
    MESH_OCTAHEDRON,
    MESH_DODECAHEDRON,
    MESH_ICOSAHEDRON,
    MESH_TYPE_COUNT
};

int main()
{
    CreateWindow(800, 800, "Graphics 1");

    Mesh head, plane, sphere;
    LoadMesh(&head, "./assets/meshes/head.obj");
    LoadMeshPlane(&plane);
    LoadMeshSphere(&sphere);

    Mesh solids[MESH_TYPE_COUNT];
    LoadMeshTetrahedron(&solids[MESH_TETRAHEDRON]);
    LoadMeshCube(&solids[MESH_CUBE]);
    LoadMeshOctahedron(&solids[MESH_OCTAHEDRON]);
    LoadMeshDodecahedron(&solids[MESH_DODECAHEDRON]);
    LoadMeshIcosahedron(&solids[MESH_ICOSAHEDRON]);
    
    GLuint position_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/position_color.vert");
    GLuint tcoord_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/tcoord_color.vert");
    GLuint normal_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/normal_color.vert");
    GLuint vertex_color_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/vertex_color.frag");

    GLuint shaders[SHADER_TYPE_COUNT];
    shaders[SHADER_POSITION_COLOR] = CreateProgram(position_color_vert, vertex_color_frag);
    shaders[SHADER_TCOORD_COLOR] = CreateProgram(tcoord_color_vert, vertex_color_frag);
    shaders[SHADER_NORMAL_COLOR] = CreateProgram(normal_color_vert, vertex_color_frag);

    int shader_index = 0;
    int mesh_index = 0;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        if (IsKeyPressed(KEY_GRAVE_ACCENT))
            ++shader_index %= SHADER_TYPE_COUNT;

        if (IsKeyPressed(KEY_TAB))
            ++mesh_index %= MESH_TYPE_COUNT;

        float tt = Time();

        //Matrix proj = MatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
        Matrix proj = MatrixPerspective(75.0f * DEG2RAD, WindowWidth() / (float)WindowHeight(), 0.01f, 100.0f);
        Matrix view = MatrixLookAt({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, Vector3UnitY);
        Matrix world = MatrixRotateY(tt * 100.0f * DEG2RAD); //MatrixIdentity();
        Matrix mvp = world * view * proj;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        BeginShader(shaders[shader_index]);
            SendMat4(mvp, "u_mvp");
            DrawMesh(solids[mesh_index]);
        EndShader();

        BeginGui();
        //ImGui::ShowDemoWindow(nullptr);
        EndGui();

        Loop();
    }

    DestroyShader(&position_color_vert);
    DestroyShader(&tcoord_color_vert);
    DestroyShader(&normal_color_vert);
    DestroyShader(&vertex_color_frag);

    for (int i = 0; i < SHADER_TYPE_COUNT; i++)
        DestroyProgram(&shaders[i]);

    for (int i = 0; i < MESH_TYPE_COUNT; i++)
        UnloadMesh(&solids[i]);

    UnloadMesh(&sphere);
    UnloadMesh(&plane);
    UnloadMesh(&head);

    DestroyWindow();
    return 0;
}
