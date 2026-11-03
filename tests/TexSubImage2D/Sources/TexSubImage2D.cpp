#include <cstdio>
#include <cstdlib>
#include <string>

#define SRGL_MOCK_GL
#include "srgl.h"

#include "stb_image.h"
#include "SnapshotTestFramework.h"
#include "Resources/TexSubImage2DResources.h"

using namespace BakedResources;

GLuint CompileShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    return shader;
}

GLuint CreateProgram(void)
{
    std::string vertexShaderSource(
        reinterpret_cast<const char *>(TexSubImage2DResources::vertex_shader_glsl.data),
        TexSubImage2DResources::vertex_shader_glsl.size);
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());

    std::string fragmentShaderSource(
        reinterpret_cast<const char *>(TexSubImage2DResources::fragment_shader_glsl.data),
        TexSubImage2DResources::fragment_shader_glsl.size);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

int main(int argc, char **argv)
{
    configureTest(argc, argv, "TexSubImage2D", TEST_SOURCE_DIR, 800, 600);

    const float vertices[] = {
        0.0f, 0.6f, 0.0f, 0.5f, 1.0f,
        -0.6f, -0.6f, 0.0f, 0.0f, 0.0f,
        0.6f, -0.6f, 0.0f, 1.0f, 0.0f,
    };
    GLuint shaderProgram = CreateProgram();
    GLuint vao;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int width;
    int height;
    int channels;
    stbi_uc *pixels = stbi_load_from_memory(
        TexSubImage2DResources::texture_jpg.data,
        TexSubImage2DResources::texture_jpg.size,
        &width,
        &height,
        &channels,
        STBI_rgb);
    if (!pixels)
    {
        std::fprintf(stderr, "Failed to load texture: %s\n", stbi_failure_reason());
        return 1;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
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

    const int squareWidth = width / 4;
    const int squareHeight = height / 4;
    const int squareSize = squareWidth * squareHeight;
    std::vector<unsigned char> redSquare(static_cast<size_t>(squareSize) * 3, 0);
    for (size_t index = 0; index < redSquare.size(); index += 3)
    {
        redSquare[index] = 255;
    }
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        (width - squareWidth) / 2,
        (height - squareHeight) / 2,
        squareWidth,
        squareHeight,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        redSquare.data());

    stbi_image_free(pixels);

    glViewport(0, 0, 800, 600);
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    requireMatchingSnapshot([]() -> std::vector<uint8_t>
    {
        std::vector<uint8_t> framebuffer(800 * 600 * 3);
        glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.data());
        return framebuffer;
    });

    return 0;
}