#include "Window.h"
#include "Shader.h"
#include "Buffer.h"

#include <imgui/imgui.h>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>

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

    int plane_vertex_indices[] =
    {
        0, 1, 2,    // Face 0 indices
        0, 2, 3     // Face 1 indices
    };

    CreateWindow(800, 800, "Graphics 1");
    
    GLuint position_color_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/position_color.vert");
    GLuint position_color_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/position_color.frag");
    GLuint position_color = CreateProgram(position_color_vert, position_color_frag);

    GLuint vbo_plane_positions = CreateBuffer();
    BindVertexBuffer(vbo_plane_positions);
        UpdateVertexBuffer(plane_vertex_positions, sizeof(plane_vertex_positions));
    UnbindVertexBuffer(vbo_plane_positions);

    GLuint ebo_plane = CreateBuffer();
    BindElementBuffer(ebo_plane);
        UpdateElementBuffer(plane_vertex_indices, sizeof(plane_vertex_indices));
    UnbindElementBuffer(ebo_plane);

    GLuint vao_plane = CreateVertexArray();
    BindVertexArray(vao_plane);

        BindElementBuffer(ebo_plane);

        BindVertexBuffer(vbo_plane_positions);
        EnableVertexAttribute(0);
        SetVertexAttribute(0, 3, GL_FLOAT, sizeof(Vector3));
        UnbindVertexBuffer(vbo_plane_positions);

    UnbindVertexArray(vao_plane);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        Matrix proj = MatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
        Matrix view = MatrixLookAt({ 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, Vector3UnitY);
        Matrix world = MatrixIdentity();
        Matrix mvp = world * view * proj;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        BindVertexArray(vao_plane);
            BeginShader(position_color);
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
    DestroyShader(&position_color_frag);
    DestroyProgram(&position_color);

    DestroyWindow();
    return 0;
}
