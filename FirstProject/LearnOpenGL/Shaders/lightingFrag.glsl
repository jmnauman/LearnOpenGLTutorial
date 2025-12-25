#version 330 core
out vec4 FragColor;

in vec3 norm;  
in vec3 worldPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientIntensity;
uniform float specIntensity;
uniform int shininess;

void main()
{
    vec3 ambientColor = ambientIntensity * lightColor;

    // Important to make sure diffuseColor doesn't have negative components, as lighting for negative colors isn't really defined
    vec3 lightDir = normalize(lightPos - worldPos);
    vec3 diffuseColor = lightColor * max(dot(norm, lightDir), 0.0);
    
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm); // remember lightDir is going from frag to light. reflect expects opposite.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specColor = spec * specIntensity * lightColor;
    
    vec3 result = (diffuseColor + ambientColor + specColor) * objectColor;
    FragColor = vec4(result, 1.0);
}