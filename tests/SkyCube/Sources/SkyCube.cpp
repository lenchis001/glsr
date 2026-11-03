#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <algorithm>

#include <string>

#define SRGL_MOCK_GL
#include "srgl.h"

#include "stb_image.h"
#include "SnapshotTestFramework.h"

#include "Resources/SkyCubeResources.h"

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
        reinterpret_cast<const char *>(SkyCubeResources::vertex_shader_glsl.data),
        SkyCubeResources::vertex_shader_glsl.size);
    GLuint vs = CompileShader(
        GL_VERTEX_SHADER,
        vertexShaderSource.c_str());

    std::string fragmentShaderSource(
        reinterpret_cast<const char *>(SkyCubeResources::fragment_shader_glsl.data),
        SkyCubeResources::fragment_shader_glsl.size);
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
GLuint texture;

// Positions only; they double as the cubemap sampling direction.
float vertices[] = {
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
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

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void *)0);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void LoadCubeMapFace(GLenum face, const uint8_t *data, size_t size)
{
    int width, height, channels;
    stbi_uc *pixels = stbi_load_from_memory(
        data,
        static_cast<int>(size),
        &width,
        &height,
        &channels,
        STBI_rgb);
    if (!pixels)
    {
        printf("Failed to load cubemap face: %s\n", stbi_failure_reason());
        exit(1);
    }

    glTexImage2D(
        face,
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

void InitTexture(void)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    LoadCubeMapFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X, SkyCubeResources::skybox_right_jpg.data, SkyCubeResources::skybox_right_jpg.size);
    LoadCubeMapFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, SkyCubeResources::skybox_left_jpg.data, SkyCubeResources::skybox_left_jpg.size);
    LoadCubeMapFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, SkyCubeResources::skybox_top_jpg.data, SkyCubeResources::skybox_top_jpg.size);
    LoadCubeMapFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, SkyCubeResources::skybox_bottom_jpg.data, SkyCubeResources::skybox_bottom_jpg.size);
    LoadCubeMapFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, SkyCubeResources::skybox_front_jpg.data, SkyCubeResources::skybox_front_jpg.size);
    LoadCubeMapFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, SkyCubeResources::skybox_back_jpg.data, SkyCubeResources::skybox_back_jpg.size);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

int main(int argc, char **argv)
{
    configureTest(argc, argv, "SkyCube", TEST_SOURCE_DIR, 800, 600);

    InitCube();
    InitTexture();

    glViewport(0, 0, 800, 600);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Camera stays at the origin: only rotation is applied, as translation
    // would break the illusion of an infinitely distant skybox.
    float rotationX[16];
    rotationXMat4(static_cast<float>(M_PI) / 10.0f, rotationX); // 18 degrees

    float rotationY[16];
    rotationYMat4(static_cast<float>(M_PI) / 6.0f, rotationY); // 30 degrees

    float view[16];
    multiplyMat4(rotationX, rotationY, view);

    float projection[16];
    perspectiveMat4(static_cast<float>(M_PI) / 4.0f, 800.0f / 600.0f, 0.1f, 100.0f, projection);

    float transform[16];
    multiplyMat4(projection, view, transform);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, transform);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    requireMatchingSnapshot([]() -> std::vector<uint8_t>
    {
        std::vector<uint8_t> framebuffer(800 * 600 * 3);
        glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.data());
        return framebuffer;
    });

    return 0;
}
