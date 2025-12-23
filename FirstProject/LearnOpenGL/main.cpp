#include <glad/glad.h> // This includes the required OpenGL headers under the hood
#include <glfw3.h>
#include <iostream>
#include <__msvc_ostream.hpp>
#include "helpers.h"
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "FlyCamera.h"
#include "vaoLibrary.h"

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float& mixStrength, const float deltaTime);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

GLuint createTex(const char* texPath, int sWrap = GL_REPEAT, int tWrap = GL_REPEAT, int magFilter = GL_LINEAR);

float deltaTime = 0.f;
float lastFrame = 0.f;
float lastX = 400;
float lastY = 300;
bool firstMouse = true;
FlyCamera camera(800.f / 600.f);

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
	// Capture mouse movement, force it to remain inside the window, and don't show the pointer.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback); // Will resize the viewport as the window resizes

	printNumberOfVertexAttributes();

	Shader simpleShader("./Shaders/simpleVert.glsl", "./Shaders/simpleFrag.glsl");

	GLuint tex0 = createTex("./Resources/container.jpg", GL_CLAMP, GL_CLAMP);
	GLuint tex1 = createTex("./Resources/awesomeface.png", GL_REPEAT, GL_REPEAT);
	GLuint VAO = getBoxVAO();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); Use this for wireframe
	glEnable(GL_DEPTH_TEST);

	std::vector<glm::vec3> cubePositions = tenRandomCubePositions();

	float mixStrength = 0.5;
	while (!glfwWindowShouldClose(window))
	{
		float time = (float)glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		processInput(window, mixStrength, deltaTime);
		glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_STENCIL_BUFFER_BIT);

		simpleShader.use(); // Every shader and rendering call after this will use the program with our linked vertex/frag shader
		simpleShader.setInt("tex", 0); // I think this is saying "the sampler called tex will sample from texture unit (or location 0)". We then bind our texture to that location below.
		simpleShader.setInt("tex2", 1);
		simpleShader.setFloat("mixStrength", mixStrength);
		simpleShader.setMatrix4("view", camera.getView());
		simpleShader.setMatrix4("proj", camera.getProj());

		glActiveTexture(GL_TEXTURE0); // This activates "texture unit 0". The next line will bind the texture to that unit. tex unit is the location from which a sampler will sample. This is how we can get multiple textures.
		glBindTexture(GL_TEXTURE_2D, tex0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex1);
		glBindVertexArray(VAO);

		for (auto i = 0; i < 10; i++)
		{
			float rotate = i % 3 == 0 ? i + 1 : 0;
			glm::mat4 model = tr(cubePositions[i], glm::vec3(0.5f, 1.0f, 0.f), (float)glfwGetTime() * rotate * glm::radians(-55.0f));
			simpleShader.setMatrix4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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

void processInput(GLFWwindow* window, float& mixStrength, const float deltaTime)
{
	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		mixStrength = mixStrength + 0.01f > 1.f ? 1.f : mixStrength + 0.01f;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		mixStrength = mixStrength - 0.01f < 0.f ? 0.f : mixStrength - 0.01f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveForward(deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveBackward(deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveLeft(deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(deltaTime);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float dx = xPos - lastX;
	float dy = yPos - lastY;
	lastX = xPos;
	lastY = yPos;

	camera.adjustLook(dx, dy);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.zoom(-yOffset);
}


GLuint createTex(const char* texPath, int sWrap, int tWrap, int magFilter)
{
	stbi_set_flip_vertically_on_load(true); // For STBI, y == 0 is at the top. For OpenGL, y == 0 is at the bottom.

	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap); // Repeat horizontally if s (think UVs) is less than zero or greater than one. Could also clamp here.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // When the texture takes up a small enough portion of the screen, (i.e. minification) use mipmaps. Linear filtering between mipmap levels and linear filtering between texels.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter); // When the texture is scaled larger, use linear filtering on texels.

	int width, height, numChannels;
	unsigned char* data = stbi_load(texPath, &width, &height, &numChannels, 0);
	GLenum fmt = GL_NONE;
	if (numChannels == 3)
	{
		fmt = GL_RGB;
	}
	else if (numChannels == 4)
	{
		fmt = GL_RGBA;
	}


	if (data != nullptr)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, fmt, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture from " << texPath << '\n';
	}
	stbi_image_free(data);
	data = nullptr;
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}
