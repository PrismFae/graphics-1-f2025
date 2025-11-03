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

    GLuint vbo_plane_positions = CreateVertexBuffer();
    BindVertexBuffer(vbo_plane_positions);
        UpdateVertexBuffer(vbo_plane_positions, (void*)plane_vertex_positions, sizeof(plane_vertex_positions));
    UnbindVertexBuffer();

    GLuint ebo_plane;
    glGenBuffers(1, &ebo_plane);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_plane);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_vertex_indices), plane_vertex_indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

    GLuint vao_plane = CreateVertexArray();
    BindVertexArray(vao_plane);

    EnableVertexAttribute(0);
    BindVertexBuffer(vbo_plane_positions);
    SetVertexAttribute(0, 3, GL_FLOAT, sizeof(Vector3));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_plane);

    UnbindVertexArray();
    UnbindVertexBuffer();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

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

        glLineWidth(5.0f);
        BindVertexArray(vao_plane);
        BeginShader(position_color);
        {
            SendMat4(mvp, "u_mvp");
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);  // <-- samples 4 vertices using 6 indices
            //glDrawArrays(GL_TRIANGLES, 0, 6);                         // <-- requires 6 vertices (not optimal)
        }
        EndShader();
        UnbindVertexArray();
        glLineWidth(1.0f);

        BeginGui();
        //ImGui::ShowDemoWindow(nullptr);
        EndGui();

        Loop();
    }
    
    // TODO -- both vbo & ebo use gen/bufferdata/delete buffer so add an argument for buffer target?
    DestroyVertexArray(&vao_plane);
    DestroyVertexBuffer(&vbo_plane_positions);
    DestroyProgram(&position_color);
    DestroyShader(&position_color_vert);
    DestroyShader(&position_color_frag);

    DestroyWindow();
    return 0;
}
