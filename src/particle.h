#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glm/glm.hpp>

class Particle
{
	public:
		// Ball state
		float Radius;
		bool Stuck;
		float Size;
		glm::vec3 Pos;
		glm::vec3 Velocity;
		// Constructor
		Particle(glm::vec3 pos, float radius, glm::vec3 velocity, bool stuck)
		{
			Pos = pos;
			Size = radius * 2;
			Stuck = stuck;
			Radius = radius;
			Velocity = velocity;
		}
		;
		// Moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
		glm::vec3 Move(float dt, int limit_width, int limit_height, int limit_z)
		{

			// If not stuck to player board
			if (!this->Stuck)
			{
				// Move the ball
				Pos += Velocity * dt;
				// Then check if outside window bounds and if so, reverse velocity and restore at correct position
				if (Pos.x <= 0.0f)
				{
					Velocity.x = -Velocity.x;
					Pos.x = 0.0f;
				}
				else if (Pos.x + Size >= limit_width)
				{
					this->Velocity.x = -this->Velocity.x;
					this->Pos = limit_width - Size;
				}
//				if (this->Position.y <= 0.0f)
//				{
//					this->Velocity.y = -this->Velocity.y;
//					this->Position.y = 0.0f;
//				}
			}
			return Pos;
		}
		;
};

#endif

