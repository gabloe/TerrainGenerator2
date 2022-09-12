#version 330 core

in vec3 camera;
in vec3 position;
in vec3 normal;
in vec4 color;

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
    fLightPosition = view * vec4(0.0,0.0,1.0,1.0);
    cameraPos = camera;
    fColor = color;
    fNormal = vec3(view * vec4(normal,0.0));
    gl_Position = projection * fPosition * model;
}
