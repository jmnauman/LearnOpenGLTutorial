#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec3 vertPos;
in vec2 interpTexCoord;

uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
	vec2 mirrored = vec2(-interpTexCoord.x, interpTexCoord.y);
	FragColor = mix(texture(tex, interpTexCoord), texture(tex2, mirrored), 0.2); // mix linerarly interpolates between the two arguments based on the third. In this case, 80% of the color is taken from first texture.
};