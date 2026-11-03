#version 330 core

precision lowp float;

layout(location = 0) in vec3 vertexPosition;

out vec3 TexCoords;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertexPosition, 1);

    TexCoords = vertexPosition;
}
