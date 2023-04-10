#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float alpha;

void main()
{             
    vec4 sceneColor = texture(scene, TexCoords);      
    vec4 bloomColor = texture(bloomBlur, TexCoords);
    if (bloom)
        FragColor = alpha * bloomColor + (1 - alpha) * sceneColor; 
    else 
        FragColor = sceneColor;
}