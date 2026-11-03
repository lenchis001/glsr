#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <string>

#define SRGL_MOCK_GL
#include "srgl.h"

#include "stb_image.h"

#include "SnapshotTestFramework.h"

#include "Resources/MultiTexturingResources.h"

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
        reinterpret_cast<const char *>(MultiTexturingResources::vertex_shader_glsl.data),
        MultiTexturingResources::vertex_shader_glsl.size);
    GLuint vs = CompileShader(
        GL_VERTEX_SHADER,
        vertexShaderSource.c_str());

    std::string fragmentShaderSource(
        reinterpret_cast<const char *>(MultiTexturingResources::fragment_shader_glsl.data),
        MultiTexturingResources::fragment_shader_glsl.size);
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
GLuint texture1;
GLuint texture2;

// x, y, z,    u, v
float vertices[] = {
    0.0f,
    0.6f,
    0.0f,
    0.5f,
    1.0f,
    -0.6f,
    -0.6f,
    0.0f,
    0.0f,
    0.0f,
    0.6f,
    -0.6f,
    0.0f,
    1.0f,
    0.0f,
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

void InitTexture(GLuint *texture, const unsigned char *fileName, int fileSize)
{
    int width, height, channels;
    stbi_uc *pixels = stbi_load_from_memory(
        fileName,
        fileSize,
        &width,
        &height,
        &channels,
        STBI_rgb);
    if (!pixels)
    {
        printf("Failed to load texture: %s\n", stbi_failure_reason());
        exit(1);
    }

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

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
    configureTest(argc, argv, "MultiTexturing", TEST_SOURCE_DIR, 800, 600);

    InitTriangle();
    InitTexture(
        &texture1,
        MultiTexturingResources::texture_jpg.data,
        MultiTexturingResources::texture_jpg.size);
    InitTexture(
        &texture2,
        MultiTexturingResources::shadows_png.data,
        MultiTexturingResources::shadows_png.size);

    glViewport(0, 0, 800, 600);

    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture1"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture2"), 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    requireMatchingSnapshot([]() -> std::vector<uint8_t>
                            {
        std::vector<uint8_t> framebuffer(800 * 600 * 3);
        glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.data());
        return framebuffer; });

    return 0;
}
