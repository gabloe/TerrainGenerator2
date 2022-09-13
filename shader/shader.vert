#version 330 core

in vec3 position;
in vec3 normal;
in vec4 color;

uniform vec3 camera;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 fPosition;
out vec4 fColor;
out vec4 fLightPosition;
out vec3 fNormal;
out vec3 cameraPos;

void main(void)
{
    fPosition = view * vec4(position,1.0);
    fLightPosition = view * vec4(0.0,0.0,1.0,0.0);
    fNormal = vec3(view * vec4(normal,0.0));

    cameraPos = camera;
    fColor = color;
    
    gl_Position = projection * fPosition * model;
}
