#version 330 core
out vec4 FragColor;

in vec3 pos;
in vec3 norm;
in vec2 uv;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_diffuse4;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
    sampler2D texture_specular4;
    float shininess;
};


struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCone;
    float cutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

float Attenuate(float dist, float kc, float kl, float kq);
vec3 CalculateSpec(vec3 viewPos, vec3 pos, vec3 lightDir, float shininess, vec3 matSpec, vec3 lightSpec, vec3 norm);
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 matDiffuse, vec3 matSpec, vec3 norm, vec3 viewDir, float shininess, vec3 fragPos);
vec3 CalculateSpotLight(SpotLight light, vec3 matDiffuse, vec3 matSpec, vec3 norm, vec3 viewDir, float shininess, vec3 fragPos);
vec3 CalculatePointLight(PointLight light, vec3 matDiffuse, vec3 matSpec, vec3 norm, vec3 viewDir, float shininess, vec3 fragPos);

void main()
{
    vec3 result = vec3(0);
    vec3 matDiffuse = vec3(texture(material.texture_diffuse1, uv));
    vec3 matSpec = vec3(texture(material.texture_specular1, uv));
    vec3 viewDir = normalize(viewPos - pos);
    vec3 normal = normalize(norm);
    result += CalculateDirectionalLight(dirLight, matDiffuse, matSpec, normal, viewDir, material.shininess, pos);
    result += CalculateSpotLight(spotLight, matDiffuse, matSpec, normal, viewDir, material.shininess, pos);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalculatePointLight(pointLights[i], matDiffuse, matSpec, normal, viewDir, material.shininess, pos);
    }

    FragColor = vec4(result, 1.0);
}

float Attenuate(float dist, float kc, float kl, float kq)
{
    return 1.0 / (kc + kl * dist + kq * dist * dist);
}

vec3 CalculateSpec(vec3 viewDir, vec3 fragPos, vec3 lightDir, float shininess, vec3 matSpec, vec3 lightSpec, vec3 norm)
{
    vec3 reflectDir = reflect(-lightDir, norm); // remember lightDir is going from frag to light. reflect expects opposite.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specColor = spec * matSpec * lightSpec;
    return specColor;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 matDiffuse, vec3 matSpec, vec3 norm, vec3 viewDir, float shininess, vec3 fragPos)
{
    vec3 ambientColor = light.ambient * matDiffuse;

    vec3 lightDir = normalize(-light.direction);
    vec3 diffuseColor = matDiffuse * light.diffuse * max(dot(norm, lightDir), 0.0);
    vec3 specColor = CalculateSpec(viewDir, fragPos, lightDir, shininess, matSpec, light.specular, norm);

    return clamp(ambientColor + diffuseColor + specColor, 0, 1);
}

vec3 CalculateSpotLight(SpotLight light, vec3 matDiffuse, vec3 matSpec, vec3 norm, vec3 viewDir, float shininess, vec3 fragPos)
{
    vec3 ambientColor = light.ambient * matDiffuse;

    vec3 lightDir = normalize(-light.direction);
    vec3 diffuseColor = matDiffuse * light.diffuse * max(dot(norm, lightDir), 0.0);

    vec3 specColor = CalculateSpec(viewDir, fragPos, light.direction, shininess, matSpec, light.specular, norm);

    float lightDist = length(light.position - fragPos);

    vec3 toLight = normalize(light.position - fragPos);
    float theta = dot(toLight, lightDir);
    float intensity = clamp((theta - light.cutoff) / (light.innerCone - light.cutoff), 0.0, 1.0);
    float attenuation = Attenuate(lightDist, light.constant, light.linear, light.quadratic);

    return clamp(attenuation * intensity * (diffuseColor + specColor), 0, 1);
}

vec3 CalculatePointLight(PointLight light, vec3 matDiffuse, vec3 matSpec, vec3 norm, vec3 viewDir, float shininess, vec3 fragPos)
{
    vec3 ambientColor = light.ambient * matDiffuse;

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 diffuseColor = matDiffuse * light.diffuse * max(dot(norm, lightDir), 0.0);

    vec3 specColor = CalculateSpec(viewDir, fragPos, lightDir, shininess, matSpec, light.specular, norm);

    float lightDist = length(light.position - fragPos);

    float attenuation = Attenuate(lightDist, light.constant, light.linear, light.quadratic);

    return clamp(attenuation * (diffuseColor + specColor + ambientColor), 0, 1);
}