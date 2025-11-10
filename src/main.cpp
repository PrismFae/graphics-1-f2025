#include "Window.h"
#include "Shader.h"
#include "Buffer.h"

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

int main()
{
    // CCW winding order
    Vector3 plane_vertex_positions[] =
    {
        { -0.5f, -0.5f, 0.0f }, // bottom-left
        {  0.5f, -0.5f, 0.0f }, // bottom-right
        {  0.5f,  0.5f, 0.0f }, // top-right
        {  -0.5f, 0.5f, 0.0f }  // top-left
    };

    Vector2 plane_vertex_tcoords[] =
    {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

    Vector3 plane_vertex_normals[] =
    {
        Vector3UnitZ,
        Vector3UnitZ,
        Vector3UnitZ,
        Vector3UnitZ
    };

    int plane_vertex_indices[] =
    {
        0, 1, 2,    // Face 0 indices
        0, 2, 3     // Face 1 indices
    };

    CreateWindow(800, 800, "Graphics 1");
    
    GLuint position_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/position_color.vert");
    GLuint tcoord_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/tcoord_color.vert");
    GLuint normal_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/normal_color.vert");
    GLuint vertex_color_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/vertex_color.frag");

    GLuint position_color = CreateProgram(position_color_vert, vertex_color_frag);
    GLuint tcoord_color = CreateProgram(tcoord_color_vert, vertex_color_frag);
    GLuint normal_color = CreateProgram(normal_color_vert, vertex_color_frag);

    GLuint shaders[SHADER_TYPE_COUNT];
    shaders[SHADER_POSITION_COLOR] = position_color;
    shaders[SHADER_TCOORD_COLOR] = tcoord_color;
    shaders[SHADER_NORMAL_COLOR] = normal_color;

    GLuint vbo_plane_positions = CreateBuffer();
    BindVertexBuffer(vbo_plane_positions);
        UpdateVertexBuffer(plane_vertex_positions, sizeof(plane_vertex_positions));
    UnbindVertexBuffer(vbo_plane_positions);

    GLuint vbo_plane_tcoords = CreateBuffer();
    BindVertexBuffer(vbo_plane_tcoords);
        UpdateVertexBuffer(plane_vertex_tcoords, sizeof(plane_vertex_tcoords));
    UnbindVertexBuffer(vbo_plane_tcoords);

    GLuint vbo_plane_normals = CreateBuffer();
    BindVertexBuffer(vbo_plane_normals);
        UpdateVertexBuffer(plane_vertex_normals, sizeof(plane_vertex_normals));
    UnbindVertexBuffer(vbo_plane_normals);

    GLuint ebo_plane = CreateBuffer();
    BindElementBuffer(ebo_plane);
        UpdateElementBuffer(plane_vertex_indices, sizeof(plane_vertex_indices));
    UnbindElementBuffer(ebo_plane);

    GLuint vao_plane = CreateVertexArray();
    BindVertexArray(vao_plane);

        BindElementBuffer(ebo_plane);

        EnableVertexAttribute(0);
        EnableVertexAttribute(1);
        EnableVertexAttribute(2);

        BindVertexBuffer(vbo_plane_positions);
            SetVertexAttribute(0, 3, GL_FLOAT, sizeof(Vector3));
        UnbindVertexBuffer(vbo_plane_positions);

        BindVertexBuffer(vbo_plane_tcoords);
            SetVertexAttribute(1, 2, GL_FLOAT, sizeof(Vector2));
        UnbindVertexBuffer(vbo_plane_tcoords);

        BindVertexBuffer(vbo_plane_normals);
            SetVertexAttribute(2, 3, GL_FLOAT, sizeof(Vector3));
        UnbindVertexBuffer(vbo_plane_normals);

    UnbindVertexArray(vao_plane);

    int shader_index = 0;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        if (IsKeyPressed(KEY_GRAVE_ACCENT))
            ++shader_index %= SHADER_TYPE_COUNT;

        Matrix proj = MatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
        Matrix view = MatrixLookAt({ 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, Vector3UnitY);
        Matrix world = MatrixIdentity();
        Matrix mvp = world * view * proj;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        BindVertexArray(vao_plane);
            BeginShader(shaders[shader_index]);
                SendMat4(mvp, "u_mvp");
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);  // <-- samples 4 vertices using 6 indices
                //glDrawArrays(GL_TRIANGLES, 0, 6);                         // <-- requires 6 vertices (not optimal)
            EndShader();
        UnbindVertexArray(vao_plane);

        BeginGui();
        //ImGui::ShowDemoWindow(nullptr);
        EndGui();

        Loop();
    }
    
    DestroyVertexArray(&vao_plane);
    DestroyBuffer(&ebo_plane);
    DestroyBuffer(&vbo_plane_positions);

    DestroyShader(&position_color_vert);
    DestroyShader(&tcoord_color_vert);
    DestroyShader(&normal_color_vert);
    DestroyShader(&vertex_color_frag);

    DestroyProgram(&position_color);
    DestroyProgram(&tcoord_color);
    DestroyProgram(&normal_color);

    DestroyWindow();
    return 0;
}
