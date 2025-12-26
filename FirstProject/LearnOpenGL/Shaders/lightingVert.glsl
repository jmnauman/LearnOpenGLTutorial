#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUv;

out vec3 norm;
out vec3 pos;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normal;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    pos = vec3(view * model * vec4(aPos, 1.0));
    norm = normalize(normal * aNorm);
    uv = aUv;
} 