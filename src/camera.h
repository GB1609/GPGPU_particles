#ifndef CAMERA_H
#define CAMERA_H

#include "../lib/GLAD/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
	FORWARD, BACKWARD, LEFT, RIGHT, ROTATERIGHT, ROTATELEFT, ROTATEUP, ROTATEDOWN
};

// Default camera values
const float YAW = -114.0f;
const float PITCH = 1.0f;
const float SPEED = 6.0f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
	public:
		// Camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		glm::vec3 P1;
		glm::vec3 F1;
		glm::vec3 U1;
		float Y1;
		float Pi1;
		// Eular Angles
		float Yaw;
		float Pitch;
		// Camera options
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		// Constructor with vectors
		Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front, float yaw = YAW, float pitch = PITCH) :
				MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
		{
			Position = position;

			WorldUp = up;
			P1 = position;
			U1 = up;
			F1 = front;
			Y1 = yaw;
			Pi1 = pitch;
			Front = front;
			Yaw = yaw;

			Pitch = pitch;

			updateCameraVectors();
		}
		void setYaw(float f)
		{
			Yaw = f;
			Y1 = f;
			updateCameraVectors();
		}
		void setPitch(float f)
		{
			Pitch = f;
			Pi1 = f;
			updateCameraVectors();
		}
		// Constructor with scalar values
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
				Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
		{
			Position = glm::vec3(posX, posY, posZ);
			WorldUp = glm::vec3(upX, upY, upZ);
			Yaw = yaw;
			Pitch = pitch;
			updateCameraVectors();
		}

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(Position, Position + Front, Up);
		}

		void ProcessKeyboard(Camera_Movement direction, float deltaTime)
		{
			float velocity = MovementSpeed * deltaTime;
			if (direction == FORWARD)
				Position += Front * velocity;
			if (direction == BACKWARD)
				Position -= Front * velocity;
			if (direction == LEFT)
				Position -= Right * velocity;
			if (direction == RIGHT)
				Position += Right * velocity;

			if (direction == ROTATERIGHT)
			{
				Yaw += velocity;
				updateCameraVectors();
			}
			if (direction == ROTATELEFT)
			{
				Yaw -= velocity;
				updateCameraVectors();
			}
			if (direction == ROTATEUP)
			{
				Pitch += velocity;
				updateCameraVectors();
			}
			if (direction == ROTATEDOWN)
			{
				Pitch -= velocity;
				updateCameraVectors();
			}
		}

		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
		{
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}

			// Update Front, Right and Up Vectors using the updated Eular angles
			updateCameraVectors();
		}

		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(float yoffset)
		{
			if (Zoom >= 1.0f && Zoom <= 45.0f)
				Zoom -= yoffset;
			if (Zoom <= 1.0f)
				Zoom = 1.0f;
			if (Zoom >= 45.0f)
				Zoom = 45.0f;
		}
		// Calculates the front vector from the Camera's (updated) Eular Angles
		void updateCameraVectors()
		{
			// Calculate the new Front vector
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);
			// Also re-calculate the Right and Up vector
			Right = glm::normalize(glm::cross(Front, WorldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = glm::normalize(glm::cross(Right, Front));
		}
		void resetVisual()
		{
			Position = P1;
			Front = F1;
			WorldUp = U1;
			Yaw = Y1;
			Pitch = Pi1;
			updateCameraVectors();

		}
};
#endif
