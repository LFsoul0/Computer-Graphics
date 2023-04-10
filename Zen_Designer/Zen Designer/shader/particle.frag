#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0) * texture(tex, TexCoords).rgba;
}