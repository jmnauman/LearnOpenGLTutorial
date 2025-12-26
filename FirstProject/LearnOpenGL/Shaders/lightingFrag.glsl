#version 330 core
out vec4 FragColor;

in vec3 norm;  
in vec3 pos;
in vec2 uv;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
    vec3 diffuse = vec3(texture(material.diffuse, uv));

    vec3 ambientColor = light.ambient * diffuse;

    // Important to make sure diffuseColor doesn't have negative components, as lighting for negative colors isn't really defined
    vec3 lightDir = normalize(light.position - pos);
    vec3 diffuseColor = diffuse * light.diffuse * max(dot(norm, lightDir), 0.0);
    
    vec3 viewDir = normalize(-pos);
    vec3 reflectDir = reflect(-lightDir, norm); // remember lightDir is going from frag to light. reflect expects opposite.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specColor = spec * vec3(texture(material.specular, uv)) * light.specular;
    
    vec3 result = diffuseColor + ambientColor + specColor;
    FragColor = vec4(result, 1.0);
}