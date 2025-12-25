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

	//Shader simpleShader("./Shaders/simpleVert.glsl", "./Shaders/simpleFrag.glsl");
	Shader lightingShader("./Shaders/lightingVert.glsl", "./Shaders/lightingFrag.glsl");
	Shader lightSourceShader("./Shaders/lightingVert.glsl", "./Shaders/lightSourceFrag.glsl");
	lightingShader = Shader("./Shaders/gouraudLightingVert.glsl", "./Shaders/gouraudLightingFrag.glsl");

	GLuint tex0 = createTex("./Resources/container.jpg", GL_CLAMP, GL_CLAMP);
	GLuint tex1 = createTex("./Resources/awesomeface.png", GL_REPEAT, GL_REPEAT);

	GLuint objectVAO = getBoxVAO();
	GLuint lightVAO = getLightSourceVAO();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); Use this for wireframe
	glEnable(GL_DEPTH_TEST);

	std::vector<glm::vec3> cubePositions = tenRandomPositions();
	glm::vec3 lightPos(1.2f, 1.f, 2.f);

	float mixStrength = 0.5;
	while (!glfwWindowShouldClose(window))
	{
		float time = (float)glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		processInput(window, mixStrength, deltaTime);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_STENCIL_BUFFER_BIT);

		glm::mat4 view = camera.getView();

		lightingShader.use();
		lightingShader.setMatrix4("view", view);
		lightingShader.setMatrix4("proj", camera.getProj());
		lightingShader.setVector3("objectColor", glm::vec3(1.f, 0.5f, 0.31f));
		lightingShader.setVector3("lightColor", glm::vec3(1.f, 1.f, 1.f));
		//lightPos = glm::vec3(cos(time), sin(time), sin(time));
		lightingShader.setVector3("lightPos", view * glm::vec4(lightPos, 1.f));

		glBindVertexArray(objectVAO);
		glm::mat4 model(1.f);
		glm::mat3 normal = glm::transpose(glm::inverse(view * model));
		lightingShader.setMatrix4("model", model);
		lightingShader.setMatrix3("normal", normal);
		lightingShader.setFloat("ambientIntensity", .1f);
		lightingShader.setInt("shininess", 32);
		lightingShader.setFloat("specIntensity", .5f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// The light source object is simply so that we can visualize where the light source is
		lightSourceShader.use();
		lightSourceShader.setMatrix4("view", view);
		lightSourceShader.setMatrix4("proj", camera.getProj());
		glBindVertexArray(lightVAO);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightSourceShader.setMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.moveDown(deltaTime);

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.moveUp(deltaTime);
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
