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
#include "camera.h"

using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, Particle& particle);
float generateRandomCoord(float);

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// camera
glm::vec3 cameraPos = glm::vec3(7.6f, 0.003f, 13.0f);
glm::vec3 cameraFront = glm::vec3(-0.50f, -0.04f, -0.87f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 lightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 cubeColor = glm::vec3(0.8f, 0.8f, 0.8f);
glm::vec3 particleColor = glm::vec3(1.0f, 0.9f, 0.0f);

bool firstMouse = true;
bool moved = false;

Camera cam(cameraPos, cameraUp, cameraFront);
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
float vertLC = 0.8f;
float vertBC = 2.5f;
float radius = 0.03f;
int precisionSphere = 225;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float currentFrame;
int numberParticles = 100;
bool beginShow = false;

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
	Cube cube;
	///////////////////////////////littleCUBE/////////////////////////////////
	unsigned int VBOlc, VAOlc;
	float vertexLC[cube.getNumbDetails()];
	cube.setVertex(vertLC, vertexLC);
	glGenVertexArrays(1, &VAOlc);
	glGenBuffers(1, &VBOlc);
	glBindBuffer(GL_ARRAY_BUFFER, VBOlc);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLC), vertexLC, GL_STATIC_DRAW);
	glBindVertexArray(VAOlc);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	///////////////////////////////littleCUBE//////////////////////
	///////////////////////////////////////////bigCUBE///////////////////////////////
	float vertexBC[cube.getDimV()];
	unsigned int indexBC[cube.getDimI()];
	cube.setVertexAndIndices(vertBC, indexBC, vertexBC);
	unsigned int VBObc, VAObc, EBObc;
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
	//////////////////////////////////////////bigCUBE//////////////////////////////
	//////////////////////////////////PARTICLES//////////////////////////////////
	unsigned int VBOpart, VAOpart;
	Particle particle(radius, precisionSphere, precisionSphere);
	float vertexPart[particle.getNumberVertex()];
	particle.setVertex(vertexPart);
	for (int i = 0; i < numberParticles; i++)
	{
		particle.addPosition(generateRandomCoord(vertLC), generateRandomCoord(vertLC), generateRandomCoord(vertLC));
	}
	glGenVertexArrays(1, &VAOpart);
	glGenBuffers(1, &VBOpart);
	glBindVertexArray(VAOpart);
	glBindBuffer(GL_ARRAY_BUFFER, VBOpart);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPart), vertexPart, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(1);
	//////////////////////////PARTICLES////////////////////////////////////////
	////////////////////////SHADERS////////////////////////////////////
	Shader lightingShader("src/lightShader.vs", "src/lightShader.fs");
	Shader cubesShader("src/cameraVS.vs", "src/cameraFS.fs");
	////////////////////////////SHDAERS//////////////////////////////
//	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window, particle);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		lightingShader.setVec3("light.position", lightPos);
		lightingShader.setVec3("light.direction", lightDirection);
		lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(8.5f)));
		lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(30.5f)));
		lightingShader.setVec3("viewPos", lightPos);
		lightingShader.setVec3("light.ambient", lightColor * 0.1f);
		lightingShader.setVec3("light.diffuse", lightColor * 0.8f);
		lightingShader.setVec3("light.specular", lightColor);
		lightingShader.setFloat("light.constant", 1.0f);
		lightingShader.setFloat("light.linear", 0.1f);
		lightingShader.setFloat("light.quadratic", 0.035f);
		lightingShader.setFloat("material.alpha", 0.4f);
		lightingShader.setVec3("material.ambient", (cubeColor * 0.8f));
		lightingShader.setVec3("material.diffuse", (cubeColor * 0.9f));
		lightingShader.setVec3("material.specular", cubeColor);
		lightingShader.setFloat("material.shininess", 50.0f);

		//glm::mat4 projectionProspective = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
		glm::mat4 projectionL = glm::perspective(glm::radians(fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f,
				100.0f);
		glm::mat4 viewL = cam.GetViewMatrix();
		lightingShader.setMat4("projection", projectionL);
		lightingShader.setMat4("view", viewL);
		glm::mat4 modelL;
		lightingShader.setMat4("model", modelL);

		glBindVertexArray(VAOlc);
		modelL = glm::translate(modelL, cube.getPosition());
		float angleL = 0;
		modelL = glm::rotate(modelL, glm::radians(angleL), glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader.setMat4("model", modelL);
		glDrawArrays(GL_TRIANGLES, 0, cube.getCubePrint());

		lightingShader.setVec3("material.ambient", (particleColor * 0.8f));
		lightingShader.setVec3("material.diffuse", (particleColor * 0.9f));
		lightingShader.setVec3("material.specular", particleColor);
		lightingShader.setFloat("material.shininess", 40.0f);
		lightingShader.setFloat("material.alpha", 1.0f);
		glBindVertexArray(VAOpart);
		for (int i = 0; i < numberParticles; i++)
		{
			modelL = glm::translate(modelL, particle.getPosition(i));
			lightingShader.setMat4("model", modelL);
			glDrawArrays( GL_TRIANGLES, 0, particle.getNumberVertex());

			modelL = glm::translate(modelL, particle.getPositionInverse(i));
			if (beginShow)
				particle.update(vertBC);
		}

		cubesShader.use();
		glm::mat4 model;
		//		glm::mat4 projectionProspective = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
		glm::mat4 projectionProspective = glm::perspective(glm::radians(fov), (float) SCR_WIDTH / (float) SCR_HEIGHT,
				0.1f, 100.0f);
		cubesShader.setMat4("projection", projectionProspective);
		glm::mat4 view = cam.GetViewMatrix();
		cubesShader.setMat4("view", view);
		model = glm::translate(model, cube.getPosition());
		float angle = 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		cubesShader.setMat4("model", model);
		glBindVertexArray(VAObc);
		glDrawElements(GL_LINES, cube.getDimI(), GL_UNSIGNED_INT, 0);
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
void processInput(GLFWwindow *window, Particle& particella)
{

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		cam.resetVisual();
		moved = true;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		particella.printPosition();
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		beginShow = true;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		beginShow = false;

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		cout << cam.Position.x << "/" << cam.Position.y << "/" << cam.Position.z << endl;
		cout << cam.Front.x << "/" << cam.Front.y << "/" << cam.Front.z << endl;
		cout << cam.Up.x << "/" << cam.Up.y << "/" << cam.Up.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(FORWARD, deltaTime);
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(BACKWARD, deltaTime);
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(LEFT, deltaTime);
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.ProcessKeyboard(RIGHT, deltaTime);
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
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	cam.ProcessMouseMovement(xoffset, yoffset);
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.ProcessMouseScroll(yoffset);
}
float generateRandomCoord(float verticeCuboL)
{
	float randomCoord = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / verticeCuboL));
	if (rand() % 2 == 0)
		randomCoord *= -1;
	return randomCoord;
}
