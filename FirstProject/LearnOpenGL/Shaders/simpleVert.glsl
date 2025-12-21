// to pass a variable from vertex to fragment, declare as out in the vertex and declare as in in the fragment with same name and type
// uniform variables are global across the shader program. Can be accessed from anyy shader at any stage of the program. IF IT ISNT USED ANYWHERE IN GLSL CODE, thevariable is siletnly removed from teh compiled version
// OpenGL requires a vec4 output in the fragment shader to draw a color for that fragment

// The lesson was using .vs and .fs for file extensions for these, but .fs is some F# thing that VS fails to compile on.

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec3 vertPos;
out vec2 interpTexCoord;

uniform float offset;

void main()
{
   vec3 position = aPos + vec3(offset, 0.0, 0.0);
   gl_Position = vec4(position.x, position.y, position.z, 1.0);
   ourColor = aColor;
   vertPos = position;
   interpTexCoord = texCoord;
};