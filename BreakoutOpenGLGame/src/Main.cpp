#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include "Game.h"
#include "Manager/ResourceManager.h"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

Game GAME = Game(WINDOW_WIDTH, WINDOW_HEIGHT);
ResourceManager RESOURCE_MANAGER = ResourceManager();

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Breakout", nullptr, nullptr);
	if (!window) {
		std::cout << "ERROR: Failed to create window\n";
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Callbacks
	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// OpenGL configuration
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double deltaTime = 0.0f;
	double lastFrameTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		glfwPollEvents();

		// Process input
		GAME.ProcessInput(deltaTime);

		// Update
		GAME.Update(deltaTime);

		// Render
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		GAME.Render();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (action == GLFW_PRESS)
	{
		GAME.setKey(key, true);
	}
	else if (action == GLFW_RELEASE)
	{
		GAME.setKey(key, false);
	}
}
