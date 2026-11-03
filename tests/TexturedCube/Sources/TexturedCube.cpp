#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <algorithm>

#include <string>

#define SRGL_MOCK_GL
#include "srgl.h"

#include "stb_image.h"
#include "SnapshotTestFramework.h"

#include "Resources/TexturedCubeResources.h"

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
        reinterpret_cast<const char *>(TexturedCubeResources::vertex_shader_glsl.data),
        TexturedCubeResources::vertex_shader_glsl.size);
    GLuint vs = CompileShader(
        GL_VERTEX_SHADER,
        vertexShaderSource.c_str());

    std::string fragmentShaderSource(
        reinterpret_cast<const char *>(TexturedCubeResources::fragment_shader_glsl.data),
        TexturedCubeResources::fragment_shader_glsl.size);
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

void rotationXMat4(float angleRadians, float *out)
{
    const float c = std::cos(angleRadians);
    const float s = std::sin(angleRadians);
    const float rotation[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, c, s, 0.0f,
        0.0f, -s, c, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};
    std::copy(rotation, rotation + 16, out);
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

void rotationZMat4(float angleRadians, float *out)
{
    const float c = std::cos(angleRadians);
    const float s = std::sin(angleRadians);
    const float rotation[16] = {
        c, s, 0.0f, 0.0f,
        -s, c, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
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
GLuint ebo;
GLuint shaderProgram;
GLuint texture;

// x, y, z,    u, v
// Four vertices per face (with their own texture coordinates) so each face
// of the cube is fully textured, rather than sharing 8 corner vertices.
float vertices[] = {
    // Front face (+z)
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

    // Back face (-z)
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

    // Left face (-x)
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

    // Right face (+x)
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

    // Top face (+y)
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

    // Bottom face (-y)
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
};

// Two triangles per face, wound counter-clockwise when viewed from outside.
unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,         // Front
    4, 5, 6, 6, 7, 4,         // Back
    8, 9, 10, 10, 11, 8,      // Left
    12, 13, 14, 14, 15, 12,   // Right
    16, 17, 18, 18, 19, 16,   // Top
    20, 21, 22, 22, 23, 20,   // Bottom
};

void InitCube(void)
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
        5 * sizeof(float),
        (void *)0);

    glEnableVertexAttribArray(0);

    // Texture coordinate
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void InitTexture(void)
{
    int width, height, channels;
    stbi_uc *pixels = stbi_load_from_memory(
        TexturedCubeResources::texture_jpg.data,
        TexturedCubeResources::texture_jpg.size,
        &width,
        &height,
        &channels,
        STBI_rgb);
    if (!pixels)
    {
        printf("Failed to load texture: %s\n", stbi_failure_reason());
        exit(1);
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels);

    stbi_image_free(pixels);
}

int main(int argc, char **argv)
{
    configureTest(argc, argv, "TexturedCube", TEST_SOURCE_DIR, 800, 600);

    InitCube();
    InitTexture();

    glViewport(0, 0, 800, 600);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Model matrix: rotate the cube around all three axes.
    float rotationX[16];
    rotationXMat4(static_cast<float>(M_PI) / 6.0f, rotationX); // 30 degrees

    float rotationY[16];
    rotationYMat4(static_cast<float>(M_PI) / 4.0f, rotationY); // 45 degrees

    float rotationZ[16];
    rotationZMat4(static_cast<float>(M_PI) / 8.0f, rotationZ); // 22.5 degrees

    float rotationXY[16];
    multiplyMat4(rotationY, rotationX, rotationXY);

    float model[16];
    multiplyMat4(rotationZ, rotationXY, model);

    float view[16];
    translationMat4(0.0f, 0.0f, -3.0f, view);

    float projection[16];
    perspectiveMat4(static_cast<float>(M_PI) / 4.0f, 800.0f / 600.0f, 0.1f, 100.0f, projection);

    float viewModel[16];
    multiplyMat4(view, model, viewModel);

    // MVP = projection * view * model
    float transform[16];
    multiplyMat4(projection, viewModel, transform);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, transform);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    requireMatchingSnapshot([]() -> std::vector<uint8_t>
    {
        std::vector<uint8_t> framebuffer(800 * 600 * 3);
        glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.data());
        return framebuffer;
    });

    return 0;
}
