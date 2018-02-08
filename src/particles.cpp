#include "../lib/GLAD/glad.h"
#include <GLFW/glfw3.h>
#include "../lib/shader.h"
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#include "cube.h"
using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
bool mouseActived = false;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 9.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
bool moved = false;
float yawCamera = -90.0f;
float pitchCamera = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader particlesShader("src/cameraVS.vs", "src/cameraFS.fs");
	float verticeLC = 0.6f;
	float verticeBC = 2.0f;
	Cube cube(verticeLC);
	float verticesLC[cube.getDimV()];
	unsigned int indicesLC[cube.getDimI()];
	cube.setVertexAndIndices(verticeLC, indicesLC, verticesLC);

	float verticesBC[cube.getDimV()];
	unsigned int indicesBC[cube.getDimI()];
	cube.setVertexAndIndices(verticeBC, indicesBC, verticesBC);

	unsigned int VBOlc, VAOlc, EBOlc, VBObc, VAObc, EBObc;

	glGenVertexArrays(1, &VAOlc);
	glGenBuffers(1, &VBOlc);
	glGenBuffers(1, &EBOlc);
	glBindVertexArray(VAOlc);
	glBindBuffer(GL_ARRAY_BUFFER, VBOlc);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLC), verticesLC, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOlc);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLC), indicesLC, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	particlesShader.use();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
	particlesShader.setMat4("projection", projection);

	glGenVertexArrays(1, &VAObc);
	glGenBuffers(1, &VBObc);
	glGenBuffers(1, &EBObc);
	glBindVertexArray(VAObc);
	glBindBuffer(GL_ARRAY_BUFFER, VBObc);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBC), verticesBC, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBObc);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBC), indicesBC, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate camerashader
		particlesShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
				100.0f);
		particlesShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		particlesShader.setMat4("view", view);

		glBindVertexArray(VAOlc);
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		float angle = 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		particlesShader.setMat4("model", model);
		glDrawElements(GL_LINES, 32, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAObc);
		glDrawElements(GL_LINES, 32, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAOlc);
	glDeleteBuffers(1, &VBOlc);
	glDeleteVertexArrays(1, &VAObc);
	glDeleteBuffers(1, &VBObc);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cameraPos = glm::vec3(0.0f, 0.0f, 9.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		mouseActived = true;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		mouseActived = false;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		moved = true;
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (mouseActived)
	{
		if (firstMouse || moved)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
			moved=false;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.3f; // change this value to your liking
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yawCamera += xoffset;
		pitchCamera += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (pitchCamera > 89.0f)
			pitchCamera = 89.0f;
		if (pitchCamera < -89.0f)
			pitchCamera = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(yawCamera)) * cos(glm::radians(pitchCamera));
		front.y = sin(glm::radians(pitchCamera));
		front.z = sin(glm::radians(yawCamera)) * cos(glm::radians(pitchCamera));
		cameraFront = glm::normalize(front);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
