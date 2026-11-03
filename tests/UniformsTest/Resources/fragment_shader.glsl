#version 330 core

in vec3 vColor;

out vec4 FragColor;

uniform vec3 uTintColor;

void main()
{
    FragColor = vec4(vColor * uTintColor, 1.0);
}
