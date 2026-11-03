#include <stdio.h>

#include <string>

#define SRGL_MOCK_GL
#include "srgl.h"

#include "SnapshotTestFramework.h"

#include "Resources/BufferSubDataTestResources.h"

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
        reinterpret_cast<const char *>(BufferSubDataTestResources::vertex_shader_glsl.data),
        BufferSubDataTestResources::vertex_shader_glsl.size);
    GLuint vs = CompileShader(
        GL_VERTEX_SHADER,
        vertexShaderSource.c_str());

    std::string fragmentShaderSource(
        reinterpret_cast<const char *>(BufferSubDataTestResources::fragment_shader_glsl.data),
        BufferSubDataTestResources::fragment_shader_glsl.size);
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
// Two side-by-side triangles, both initially red/green/blue via glBufferData.
float vertices[] = {
    // Left triangle (left untouched, stays red/green/blue)
    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.1f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,

    // Right triangle (colors overwritten via glBufferSubData below)
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.1f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.9f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
};

// Replacement colors for the right triangle: yellow, magenta, cyan.
float rightTriangleColors[] = {
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
};

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

    // Overwrite only the right triangle's color components (per-vertex offset
    // past the 3 position floats) via glBufferSubData, leaving both the
    // positions and the left triangle's data untouched. This proves the
    // partial update only affects the targeted byte range of the buffer.
    constexpr GLintptr floatSize = sizeof(float);
    constexpr GLintptr vertexStride = 6 * floatSize;
    constexpr GLintptr colorOffsetInVertex = 3 * floatSize;
    constexpr GLintptr rightTriangleOffset = 3 * vertexStride;

    for (int vertexIndex = 0; vertexIndex < 3; ++vertexIndex)
    {
        glBufferSubData(
            GL_ARRAY_BUFFER,
            rightTriangleOffset + vertexIndex * vertexStride + colorOffsetInVertex,
            3 * floatSize,
            &rightTriangleColors[vertexIndex * 3]);
    }

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
    configureTest(argc, argv, "BufferSubDataTest", TEST_SOURCE_DIR, 800, 600);

    InitTriangles();

    glViewport(0, 0, 800, 600);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    requireMatchingSnapshot([]() -> std::vector<uint8_t>
    {
        std::vector<uint8_t> framebuffer(800 * 600 * 3);
        glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.data());
        return framebuffer;
    });

    return 0;
}
