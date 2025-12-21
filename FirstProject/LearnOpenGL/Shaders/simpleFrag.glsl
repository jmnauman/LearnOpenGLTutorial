#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec3 vertPos;
in vec2 interpTexCoord;

uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, interpTexCoord) * vec4(ourColor, 1.0);
};