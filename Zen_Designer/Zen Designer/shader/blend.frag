#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform vec4 mask;

void main()
{             
    vec4 sceneColor = texture(scene, TexCoords);      
    FragColor = mask + (1 - mask.a) * sceneColor; 
}