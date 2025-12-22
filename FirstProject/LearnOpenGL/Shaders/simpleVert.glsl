// to pass a variable from vertex to fragment, declare as out in the vertex and declare as in in the fragment with same name and type
// uniform variables are global across the shader program. Can be accessed from anyy shader at any stage of the program. IF IT ISNT USED ANYWHERE IN GLSL CODE, thevariable is siletnly removed from teh compiled version
// OpenGL requires a vec4 output in the fragment shader to draw a color for that fragment

// The lesson was using .vs and .fs for file extensions for these, but .fs is some F# thing that VS fails to compile on.

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 interpTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   gl_Position =  proj * view * model * vec4(aPos, 1.0);
   ourColor = aColor;
   interpTexCoord = texCoord;
};