#include "../lib/GLAD/glad.h"
#include <GLFW/glfw3.h>
#include "../lib/shader.h"
#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#include "cube.h"
#include "particle.h"
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
float generateRandomCoord(float);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
bool moved = false;
float yawCamera = -90.0f;
float pitchCamera = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
float vertLC = 0.8f;
float vertBC = 2.5f;
float radius = 0.02f;
int precisionSphere = 60;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float currentFrame;
int numberParticles = 200;
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

	Shader cubesShader("src/cameraVS.vs", "src/cameraFS.fs");
//	Shader particleShader("src/particleFS.fs", "src/particleVs.vs");

	Cube cube(vertLC);
	float vertexLC[cube.getDimV()];
	unsigned int indexLC[cube.getDimI()];
	cube.setVertexAndIndices(vertLC, indexLC, vertexLC);
	float vertexBC[cube.getDimV()];
	unsigned int indexBC[cube.getDimI()];
	cube.setVertexAndIndices(vertBC, indexBC, vertexBC);

	unsigned int VBOlc, VAOlc, EBOlc, VBObc, VAObc, EBObc;
	glGenVertexArrays(1, &VAOlc);
	glGenBuffers(1, &VBOlc);
	glGenBuffers(1, &EBOlc);
	glBindVertexArray(VAOlc);
	glBindBuffer(GL_ARRAY_BUFFER, VBOlc);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLC), vertexLC, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOlc);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexLC), indexLC, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	cubesShader.use();

	glGenVertexArrays(1, &VAObc);
	glGenBuffers(1, &VBObc);
	glGenBuffers(1, &EBObc);
	glBindVertexArray(VAObc);
	glBindBuffer(GL_ARRAY_BUFFER, VBObc);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBC), vertexBC, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBObc);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBC), indexBC, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	unsigned int VBOpart, VAOpart, EBOpart;
	Particle particle(radius, precisionSphere, precisionSphere);
	float vertexPart[particle.getNumberVertex()];
	particle.setVertex(vertexPart);
	for (int i = 0; i < numberParticles; i++)
	{
		particle.addPosition(generateRandomCoord(vertLC), generateRandomCoord(vertLC), generateRandomCoord(vertLC));
	}



//	particleShader.use();
	glGenVertexArrays(1, &VAOpart);
	glGenBuffers(1, &VBOpart);
	glGenBuffers(1, &EBOpart);
	glBindVertexArray(VAOpart);
	glBindBuffer(GL_ARRAY_BUFFER, VBOpart);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPart), vertexPart, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		cubesShader.use();

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
				100.0f);
		cubesShader.setMat4("projection", projection);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		cubesShader.setMat4("view", view);

		glBindVertexArray(VAOlc);
		glm::mat4 model;
		model = glm::translate(model, cube.getPosition());
		float angle = 0;
//		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		cubesShader.setMat4("model", model);
		glDrawElements(GL_LINES, cube.getDimI(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAObc);
		glDrawElements(GL_LINES, cube.getDimI(), GL_UNSIGNED_INT, 0);

//		particleShader.use();
//		particleShader.setMat4("projection", projection);
//		particleShader.setMat4("view", view);
//		particleShader.setMat4("model", model);
		glBindVertexArray(VAOpart);
		for (int i = 0; i < numberParticles; i++)
		{
			model = glm::translate(model, particle.getPosition(i));
//			float angle = 20.0f * (i % 3 == 0 ? glfwGetTime() : i);
//			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); //per farli muovere
			cubesShader.setMat4("model", model);
			glDrawArrays( GL_TRIANGLES, 0, particle.getNumberVertex());
			model = glm::translate(model, particle.getPositionI(i));
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAOlc);
	glDeleteBuffers(1, &VBOlc);
	glDeleteVertexArrays(1, &VAObc);
	glDeleteBuffers(1, &VBObc);
	glDeleteVertexArrays(1, &VAOpart);
	glDeleteBuffers(1, &VBOpart);

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
		cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 1.8f * deltaTime;
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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		if (firstMouse || moved)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
			moved = false;
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
float generateRandomCoord(float verticeCuboL)
{
	float randomCoord = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / verticeCuboL));
	if (rand() % 2 == 0)
		randomCoord *= -1;
	return randomCoord;
}
