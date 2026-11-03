#version 330 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

void main()
{
    vec4 color1 = texture(uTexture1, vTexCoord);
    vec4 color2 = texture(uTexture2, vTexCoord);

    FragColor = color1 * 0.5 + color2 * 0.5;
}