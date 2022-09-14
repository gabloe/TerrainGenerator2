#version 330 core

in vec4 fPosition;
in vec4 fColor;
in vec4 fLightPosition;
in vec3 fNormal;

uniform vec3 camera;

// output
out vec4 color;

// white light
vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main(void)
{   
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(fNormal);
    vec3 lightDir = -normalize(fLightPosition.xyz + fPosition.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(camera - fPosition.xyz);

    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;     

    // Result
    vec4 result = vec4(ambient + diffuse + specular, 1.0) * fColor;

    color = result;
}
