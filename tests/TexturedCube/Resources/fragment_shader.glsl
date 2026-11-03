#version 330 core

precision lowp float;

in vec2 UV;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, UV);
}