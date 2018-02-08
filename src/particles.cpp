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
void setCubes(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void drawCubes(unsigned int, Shader);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yawCamera = -90.0f;
float pitchCamera = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
bool mouseActived = false;

// timing
float deltaTime = 0.0f;
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

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader particlesShader("src/cameraVS.vs", "src/cameraFS.fs");

	unsigned int VBOcubeBC, VAOcubeBC, EBOcubeBC, VBOcubeLC, VAOcubeLC, EBOcubeLC;
	setCubes(VBOcubeBC, VAOcubeBC, EBOcubeBC, VBOcubeLC, VAOcubeLC, EBOcubeLC);

	// render loop
	// -----------

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	particlesShader.use();
	// pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
	// -----------------------------------------------------------------------------------------------------------
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
	particlesShader.setMat4("projection", projection);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		particlesShader.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
				100.0f);
		particlesShader.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		particlesShader.setMat4("view", view);

		drawCubes(VAOcubeBC, particlesShader);
		drawCubes(VAOcubeLC, particlesShader);
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAOcubeBC);
	glDeleteBuffers(1, &VBOcubeBC);
	glDeleteVertexArrays(1, &VAOcubeLC);
	glDeleteBuffers(1, &VBOcubeLC);

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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		mouseActived = true;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		mouseActived = false;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (mouseActived)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
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
void setCubes(unsigned int VBOcubeBC, unsigned int VAOcubeBC, unsigned int EBOcubeBC, unsigned int VBOcubeLC,
		unsigned int VAOcubeLC, unsigned int EBOcubeLC)
{
	glGenVertexArrays(1, &VAOcubeBC);
	glGenBuffers(1, &VBOcubeBC);
	glGenBuffers(1, &EBOcubeBC);
	glGenVertexArrays(1, &VAOcubeLC);
	glGenBuffers(1, &VBOcubeLC);
	glGenBuffers(1, &EBOcubeLC);
	float verticeLC = 0.3f;
	float vertceBC = 0.8f;
	Cube lcube(verticeLC);
	unsigned int indicesLC[lcube.getDimI()];
	unsigned int indicesBC[lcube.getDimI()];
	float verticesLC[lcube.getDimV()];
	float verticesBC[lcube.getDimV()];
	lcube.setVertexAndIndices(verticeLC, indicesLC, verticesLC);
	lcube.setVertexAndIndices(vertceBC, indicesBC, verticesBC);

	glBindVertexArray(VAOcubeLC);

	glBindBuffer(GL_ARRAY_BUFFER, VBOcubeLC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLC), verticesLC, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOcubeLC);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesLC), indicesLC, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(VAOcubeBC);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcubeBC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBC), verticesBC, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOcubeBC);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBC), indicesBC, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
void drawCubes(unsigned int VAOcube, Shader particleShader)
{
	particleShader.use();
	glBindVertexArray(VAOcube);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	float angle = 0;
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	particleShader.setMat4("model", model);
	glDrawElements(GL_LINES, 32, GL_UNSIGNED_INT, 0);
}
