#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientIntensity;
uniform float specIntensity;
uniform int shininess;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    vec3 pos = vec3(view * model * vec4(aPos, 1.0));
    vec3 norm = normalize(normal * aNorm);

    vec3 ambientColor = ambientIntensity * lightColor;

    // Important to make sure diffuseColor doesn't have negative components, as lighting for negative colors isn't really defined
    vec3 lightDir = normalize(lightPos - pos);
    vec3 diffuseColor = lightColor * max(dot(norm, lightDir), 0.0);
    
    vec3 viewDir = normalize(-pos);
    vec3 reflectDir = reflect(-lightDir, norm); // remember lightDir is going from frag to light. reflect expects opposite.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specColor = spec * specIntensity * lightColor;
    
    color = (diffuseColor + ambientColor + specColor) * objectColor;
} 