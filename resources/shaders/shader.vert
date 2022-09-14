#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 fPosition;
out vec4 fColor;
out vec4 fLightPosition;
out vec3 fNormal;

void main(void)
{
    fPosition = view * vec4(position,1.0);
    fLightPosition = view * vec4(0.0,0.0,1.0,0.0);
    fNormal = vec3(view * vec4(normal,0.0));

    fColor = vec4(color, 0.0);
    
    gl_Position = projection * fPosition * model;
}
