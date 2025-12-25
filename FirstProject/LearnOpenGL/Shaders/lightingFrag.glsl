#version 330 core
out vec4 FragColor;

in vec3 norm;  
in vec3 pos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main()
{
    vec3 ambientColor = light.ambient * material.ambient;

    // Important to make sure diffuseColor doesn't have negative components, as lighting for negative colors isn't really defined
    vec3 lightDir = normalize(light.position - pos);
    vec3 diffuseColor = material.diffuse * light.diffuse * max(dot(norm, lightDir), 0.0);
    
    vec3 viewDir = normalize(-pos);
    vec3 reflectDir = reflect(-lightDir, norm); // remember lightDir is going from frag to light. reflect expects opposite.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specColor = spec * material.specular * light.specular;
    
    vec3 result = diffuseColor + ambientColor + specColor;
    FragColor = vec4(result, 1.0);
}