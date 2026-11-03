#include <stdio.h>
#include <cmath>
#include <algorithm>

#include <string>

#define SRGL_MOCK_GL
#include "srgl.h"

#include "SnapshotTestFramework.h"

#include "Resources/UniformsTestResources.h"

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
        reinterpret_cast<const char *>(UniformsTestResources::vertex_shader_glsl.data), 
        UniformsTestResources::vertex_shader_glsl.size);
    GLuint vs = CompileShader(
        GL_VERTEX_SHADER,
        vertexShaderSource.c_str());
        
    std::string fragmentShaderSource(
        reinterpret_cast<const char *>(UniformsTestResources::fragment_shader_glsl.data), 
        UniformsTestResources::fragment_shader_glsl.size);
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

// out = a * b, all matrices column-major (OpenGL convention).
void multiplyMat4(const float *a, const float *b, float *out)
{
    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 4; ++row)
        {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k)
            {
                sum += a[k * 4 + row] * b[col * 4 + k];
            }
            out[col * 4 + row] = sum;
        }
    }
}

void rotationYMat4(float angleRadians, float *out)
{
    const float c = std::cos(angleRadians);
    const float s = std::sin(angleRadians);
    const float rotation[16] = {
        c, 0.0f, -s, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        s, 0.0f, c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};
    std::copy(rotation, rotation + 16, out);
}

void translationMat4(float x, float y, float z, float *out)
{
    const float translation[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        x, y, z, 1.0f};
    std::copy(translation, translation + 16, out);
}

void perspectiveMat4(float fovYRadians, float aspect, float nearZ, float farZ, float *out)
{
    const float f = 1.0f / std::tan(fovYRadians / 2.0f);
    const float rangeInv = 1.0f / (nearZ - farZ);
    const float projection[16] = {
        f / aspect, 0.0f, 0.0f, 0.0f,
        0.0f, f, 0.0f, 0.0f,
        0.0f, 0.0f, (farZ + nearZ) * rangeInv, -1.0f,
        0.0f, 0.0f, 2.0f * farZ * nearZ * rangeInv, 0.0f};
    std::copy(projection, projection + 16, out);
}

GLuint vao;
GLuint vbo;
GLuint shaderProgram;

// x, y, z,    r, g, b
float vertices[] = {
    0.0f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f, // RED

    -0.6f, -0.6f, 0.0f, 0.0f, 1.0f, 0.0f, // GREEN

    0.6f, -0.6f, 0.0f, 0.0f, 0.0f, 1.0f // BLUE
};

void InitTriangle(void)
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
    configureTest(argc, argv, "UniformsTest", TEST_SOURCE_DIR, 800, 600);

    InitTriangle();

    glViewport(0, 0, 800, 600);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Model-view-projection matrix with a small rotation around Y.
    const float rotationAngle = static_cast<float>(M_PI) / 4.0f; // ~25.71 degrees
    float model[16];
    rotationYMat4(rotationAngle, model);

    float view[16];
    translationMat4(0.0f, 0.0f, -3.0f, view);

    float projection[16];
    perspectiveMat4(static_cast<float>(M_PI) / 4.0f, 800.0f / 600.0f, 0.1f, 100.0f, projection);

    float viewModel[16];
    multiplyMat4(view, model, viewModel);

    float transform[16];
    multiplyMat4(projection, viewModel, transform);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uTransform"), 1, GL_FALSE, transform);

    // Tint the vertex colors with a warm orange tone.
    float tintColor[3] = {1.0f, 0.6f, 0.2f};
    glUniform3fv(glGetUniformLocation(shaderProgram, "uTintColor"), 1, tintColor);

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    requireMatchingSnapshot([]() -> std::vector<uint8_t>
    {
        std::vector<uint8_t> framebuffer(800 * 600 * 3);
        glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.data());
        return framebuffer;
    });

    return 0;
}
