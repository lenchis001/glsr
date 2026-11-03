#include <stdio.h>

#include <string>

#define SRGL_MOCK_GL
#include "srgl.h"

#include "SnapshotTestFramework.h"

#include "Resources/DrawElementsTestResources.h"

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
        reinterpret_cast<const char *>(DrawElementsTestResources::vertex_shader_glsl.data), 
        DrawElementsTestResources::vertex_shader_glsl.size);
    GLuint vs = CompileShader(
        GL_VERTEX_SHADER,
        vertexShaderSource.c_str());
        
    std::string fragmentShaderSource(
        reinterpret_cast<const char *>(DrawElementsTestResources::fragment_shader_glsl.data), 
        DrawElementsTestResources::fragment_shader_glsl.size);
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
GLuint ebo;
GLuint shaderProgram;

// x, y, z,    r, g, b
float vertices[] = {
    -0.6f,  0.6f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left,     RED
     0.6f,  0.6f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right,    GREEN
     0.6f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right, BLUE
    -0.6f, -0.6f, 0.0f, 1.0f, 1.0f, 0.0f  // bottom-left,  YELLOW
};

// Square made up of two triangles
unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

void InitSquare(void)
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

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
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
    configureTest(argc, argv, "DrawElementsTest", TEST_SOURCE_DIR, 800, 600);

    InitSquare();

    glViewport(0, 0, 800, 600);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    requireMatchingSnapshot([]() -> std::vector<uint8_t>
    {
        std::vector<uint8_t> framebuffer(800 * 600 * 3);
        glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.data());
        return framebuffer;
    });

    return 0;
}
