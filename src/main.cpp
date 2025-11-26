#include "Window.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

#include <imgui/imgui.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>

enum ShaderType
{
    SHADER_SAMPLE_TEXTURE,
    SHADER_POSITION_COLOR,
    SHADER_TCOORD_COLOR,
    SHADER_NORMAL_COLOR,
    SHADER_TYPE_COUNT
};

enum MeshType
{
    // Platonic solids
    MESH_TETRAHEDRON,
    MESH_CUBE,
    MESH_OCTAHEDRON,
    MESH_DODECAHEDRON,
    MESH_ICOSAHEDRON,

    // Parametric surfaces
    MESH_PLANE,
    MESH_SPHERE,
    MESH_HEMISPHERE,

    // Obj files
    MESH_HEAD,

    MESH_TYPE_COUNT
};

enum TextureType
{
    TEXTURE_GRADIENT_WARM,
    TEXTURE_GRADIENT_COOL,
    TEXTURE_TYPE_COUNT
};

void LoadTextures(Texture textures[TEXTURE_TYPE_COUNT])
{
    Image warm, cool;
    LoadImage(&warm, 512, 512);
    LoadImage(&cool, 512, 512);
    LoadImageGradient(&warm, Vector3Zeros, Vector3UnitX, Vector3UnitY, Vector3UnitX + Vector3UnitY);
    LoadImageGradient(&cool, Vector3UnitZ, Vector3UnitZ + Vector3UnitX, Vector3UnitY + Vector3UnitZ, Vector3Ones);
    
    LoadTexture(&textures[TEXTURE_GRADIENT_WARM], warm);
    LoadTexture(&textures[TEXTURE_GRADIENT_COOL], cool);
    // (No need to call UnloadImage because warm & cool's memory gets cleaned up by stl vector destructor after this function returns)!
}

int main()
{
    CreateWindow(800, 800, "Graphics 1");

    Mesh meshes[MESH_TYPE_COUNT];

    LoadMeshTetrahedron(&meshes[MESH_TETRAHEDRON]);
    LoadMeshCube(&meshes[MESH_CUBE]);
    LoadMeshOctahedron(&meshes[MESH_OCTAHEDRON]);
    LoadMeshDodecahedron(&meshes[MESH_DODECAHEDRON]);
    LoadMeshIcosahedron(&meshes[MESH_ICOSAHEDRON]);

    LoadMeshPlane(&meshes[MESH_PLANE]);
    LoadMeshSphere(&meshes[MESH_SPHERE]);
    LoadMeshHemisphere(&meshes[MESH_HEMISPHERE]);

    LoadMeshObj(&meshes[MESH_HEAD], "./assets/meshes/head.obj");
    
    GLuint position_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/position_color.vert");
    GLuint tcoord_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/tcoord_color.vert");
    GLuint normal_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/normal_color.vert");
    GLuint vertex_color_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/vertex_color.frag");
    GLuint a4_texture_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/a4_texture.vert");
    GLuint a4_texture_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/a4_texture.frag");

    GLuint shaders[SHADER_TYPE_COUNT];
    shaders[SHADER_SAMPLE_TEXTURE] = CreateProgram(a4_texture_vert, a4_texture_frag);
    shaders[SHADER_POSITION_COLOR] = CreateProgram(position_color_vert, vertex_color_frag);
    shaders[SHADER_TCOORD_COLOR] = CreateProgram(tcoord_color_vert, vertex_color_frag);
    shaders[SHADER_NORMAL_COLOR] = CreateProgram(normal_color_vert, vertex_color_frag);

    Texture textures[TEXTURE_TYPE_COUNT];
    LoadTextures(textures);

    int shader_index = SHADER_SAMPLE_TEXTURE;
    int mesh_index = MESH_PLANE;
    int texture_index = TEXTURE_GRADIENT_WARM;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        if (IsKeyPressed(KEY_GRAVE_ACCENT))
            ++shader_index %= SHADER_TYPE_COUNT;

        if (IsKeyPressed(KEY_TAB))
            ++mesh_index %= MESH_TYPE_COUNT;

        if (IsKeyPressed(KEY_T))
            ++texture_index %= TEXTURE_TYPE_COUNT;

        float tt = Time();

        //Matrix proj = MatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
        Matrix proj = MatrixPerspective(75.0f * DEG2RAD, WindowWidth() / (float)WindowHeight(), 0.01f, 100.0f);
        Matrix view = MatrixLookAt({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 0.0f }, Vector3UnitY);
        //Matrix world = MatrixRotateY(tt * 100.0f * DEG2RAD);
        Matrix world = MatrixIdentity();
        Matrix mvp = world * view * proj;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        BeginShader(shaders[shader_index]);
        BeginTexture(textures[texture_index]);
            SendMat4(mvp, "u_mvp");
            DrawMesh(meshes[mesh_index]);
        EndTexture();
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

    for (int i = 0; i < TEXTURE_TYPE_COUNT; i++)
        UnloadTexture(&textures[i]);

    for (int i = 0; i < SHADER_TYPE_COUNT; i++)
        DestroyProgram(&shaders[i]);

    for (int i = 0; i < MESH_TYPE_COUNT; i++)
        UnloadMesh(&meshes[i]);

    DestroyWindow();
    return 0;
}
