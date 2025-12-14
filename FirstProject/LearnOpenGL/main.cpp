#include <glad/glad.h> // This includes the required OpenGL headers under the hood
#include <glfw3.h>
#include <iostream>
#include <__msvc_ostream.hpp>
#include "helpers.h"

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void drawTriangle();
GLuint getTriangleVAO();
GLuint getRectangleVAO();
GLuint getTwoTrianglesVAO();
GLuint getTriangleTwoVAO();
GLuint getRainbowTriangleVAO();

// Just storing the shader source as a string for now
// to pass a variable from vertex to fragment, declare as out in the vertex and declare as in in the fragment with same name and type
// uniform variables are global across the shader program. Can be accessed from anyy shader at any stage of the program. IF IT ISNT USED ANYWHERE IN GLSL CODE, thevariable is siletnly removed from teh compiled version
// OpenGL requires a vec4 output in the fragment shader to draw a color for that fragment
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(ourColor, 1.0);\n"
"}\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback); // Will resize the viewport as the window resizes

	printNumberOfVertexAttributes();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	int success = 0;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "Shader compilation failed:\n" << infoLog << "\n";
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	success = 0;
	char infoLog2[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog2);
		std::cout << "Shader compilation failed:\n" << infoLog2 << "\n";
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	success = 0;
	char infoLog3[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog3);
		std::cout << "Shader linking failed:\n" << infoLog3 << '\n';
	}
	glDeleteShader(vertexShader); // We don't need these after we've finished creating the program
	glDeleteShader(fragmentShader);

	GLuint rainbowTriangleVAO = getRainbowTriangleVAO();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); Use this for wireframe

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_STENCIL_BUFFER_BIT);
		// draw triangle
		glUseProgram(shaderProgram); // Every shader and rendering call after this will use the program with our linked vertex/frag shader

		// Draw two triangles via separate VBOs and VAOs.
		glBindVertexArray(rainbowTriangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window); // Swaps color buffer for window and shows it as output to the screen. Front buffer is the output image, back buffer is where commands go.



		glfwPollEvents(); // Checks if inputs events are triggered and updates window state
	}

	glfwTerminate();
	return 0;
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Usually when you have multiple objects, you first generatte/configure all the VAOs (attribute pointers + VBOs) then store for later use.
// When using a VBO like this, we call glDrawArrays
GLuint getTriangleVAO()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// This is a general pattern in OpenGL. Has a concept of Objects. Object is a collection of options that repesent a subset of OpenGL's state.
	// Can visualize them as a C-like struct.
	// Ask OpenGL to create the object and give you an ID you can use to access it
	// Bind the object to part of the GL context's state.
	// Set options on the object
	// I'm thinking these objects remain allocated under the hood. Unbinding them just removes them from context state. Looks like they may have to be destroyed explicitly to be relasesd
	GLuint VBO; // vertex buffer object, used to manage which vertices we've sent to the GPU
	glGenBuffers(1, &VBO); // first argument is the number of buffers to generate, second argument is an array of ids. Buffers are instatiated behind the scenes

	// VAO - Vertex Array Object. Benefit is that when configuring attribute pointers, we can store that info in the VAO. Then just
// rebind the VAO when we need to draw the object again, instead of repeating the above every time for different objects. Makes it easier to switch
// between different vertex data and attribute configurations.
// OpenGL Core REQUIRES a VAO
// only needs to be done once
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Can bind several buffers at once if they have separate types. This is the buffer type of a vertex buffer

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Allocates memory and stores data in the buffer bound to the target

	/* usage arg tells the graphics card how we want it to manage the given data. Sounds like it may determine what kind of memory the buffer goes into
	 *GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
GL_STATIC_DRAW: the data is set only once and used many times.
GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
*/
	// We need to specify which part of input data goes to which vertex attribute.
	// Remember that in the shader, we spcifed location of position attribute at 0.
	// So this essentially says "the shader position input has a size of 3 (3 values), the positions are floats, they aren't normalized between 0 and 1,
	// each position is 12 bytes apart in the input data. (Since the values are tightly packed, we technically could pass 0 for the stride and openGL would figure it out)
	// Each vertex attribute takes memory from a VBO. The VBO used is what we've currently bound to GL_ARRAY_BUFFER
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// This enabled the vertex attribute we just described to OpenGL in the previous line. Attributes are disabled by default.
	glEnableVertexAttribArray(0);

	// This is safe to do. glVertexAttribPoiunter registered the VBO as the bound buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// We typically don't call this, because modifying other VAOs required binding to them.
	// NOTE: Worth remembering that I hadn't bound the VAO in here before, which led to a bug where the triangle would be drawn.
	// I'm guessing this is because OpenGL didn't link the buffer or vertex attribute pointer to the VAO, since it wasn't "active".
	glBindVertexArray(0);

	return VAO;
}

// WHen we draw with an element buffer,we call glDrawElements
GLuint getRectangleVAO()
{
	float vertices[] = {
		-0.5, -0.5, 0, // bot left
		-0.5, 0.5, 0, // up left
		0.5, 0.5, 0, // up right
		0.5, -0.5, 0 //bot right 
	};

	unsigned int indices[]
	{
		0, 1, 2,
		0, 2, 3
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Think of EBO as an index buffer, so we don't have redundant vertices in this example of a rectangle
	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	// VERY IMPORTANT. YOU NEED TO UNBIND VAO FIRST. The vao records the bind buffer calls. Which means if we unbind the others first, they end up unbound in the VAO
	// Last element buffer bound while the VAO is bound will be rebound when the VAO is rebound
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return VAO;
}

// Exercise called for VAO, so that's why we're doing it.
GLuint getTwoTrianglesVAO()
{
	float vertices[] = {
		-0.5, -0.5, 0, // bot left
		-0.5, 0.5, 0, // up left
		0.5, 0.5, 0, // up right
		-0.5, -0.5, 0, // bot left
		0.5, 0.5, 0, // up right
		0.5, -0.5, 0 //bot right 
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	// VERY IMPORTANT. YOU NEED TO UNBIND VAO FIRST. The vao records the bind buffer calls. Which means if we unbind the others first, they end up unbound in the VAO
	// Last element buffer bound while the VAO is bound will be rebound when the VAO is rebound
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return VAO;
}

GLuint getTriangleTwoVAO()
{
	float vertices[] = {
		-0.5, -0.5, 0,
		0.5, 0.5, 0,
		0.5, -0.5, 0
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glEnableVertexAttribArray(0);

	return VAO;
}

// This produces a rainbow effect because the output color from the vertex is interpolated between the verts based on
// fragment position.
GLuint getRainbowTriangleVAO()
{
	// First 3 of each row are positions, second 3 of each row are colors
	// The stride is now 24 for everything, 12 for each attribute.
	float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.f, 0.f, 0.f,
	 0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
	 0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);
	
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	// Size appears to be the number of components. The stride is the stride for an ENTIRE VERTEX.
	// The last argument gives the offset of the attribute within a single vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}