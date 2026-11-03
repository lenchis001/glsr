#include <stdio.h>

#include <string>

#define SRGL_MOCK_GL
#include "srgl.h"

#include "SnapshotTestFramework.h"

#include "Resources/DepthTestResources.h"

using namespace BakedResources;

GLuint CompileShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        printf("Shader compile error:\n%s\n", log);
    }

    return shader;
}

GLuint CreateProgram(void)
{
    std::string vertexShaderSource(
        reinterpret_cast<const char *>(DepthTestResources::vertex_shader_glsl.data),
        DepthTestResources::vertex_shader_glsl.size);
    GLuint vs = CompileShader(
        GL_VERTEX_SHADER,
        vertexShaderSource.c_str());

    std::string fragmentShaderSource(
        reinterpret_cast<const char *>(DepthTestResources::fragment_shader_glsl.data),
        DepthTestResources::fragment_shader_glsl.size);
    GLuint fs = CompileShader(
        GL_FRAGMENT_SHADER,
        fragmentShaderSource.c_str());

    GLuint program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        printf("Program link error:\n%s\n", log);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

GLuint vao;
GLuint vbo;
GLuint shaderProgram;

// x, y, z,    r, g, b
// Two overlapping triangles at different depths: the back (red) triangle
// points right and the front (blue) triangle points left, so they overlap
// in the middle of the viewport. The red triangle is drawn first, followed
// by the blue triangle; depth testing should keep the blue triangle visible
// in the overlapping region since it has a smaller (nearer) z.
float vertices[] = {
    // Back triangle (red), farther from the camera (larger z)
    -0.7f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    -0.7f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.3f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,

    // Front triangle (blue), closer to the camera (smaller z)
    0.7f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    0.7f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    -0.3f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f};

void InitTriangles(void)
{
    shaderProgram = CreateProgram();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void *)0);

    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

int main(int argc, char **argv)
{
    configureTest(argc, argv, "DepthTest", TEST_SOURCE_DIR, 800, 600);

    InitTriangles();

    glViewport(0, 0, 800, 600);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindVertexArray(vao);

    // Draw the back (red) triangle first, then the front (blue) triangle;
    // depth testing (not draw order) should decide the overlap result.
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 3, 3);

    glBindVertexArray(0);

    requireMatchingSnapshot([]() -> std::vector<uint8_t>
    {
        std::vector<uint8_t> framebuffer(800 * 600 * 3);
        glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.data());
        return framebuffer;
    });

    return 0;
}
