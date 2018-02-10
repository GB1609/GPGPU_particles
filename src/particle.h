/*
 * Cube.h
 *
 *  Created on: 07 feb 2018
 *      Author: gb1609
 */

#ifndef SRC_PARTICLE_H_
#define SRC_PARTICLE_H_

#include <glm/glm.hpp>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

class Particle
{
	private:
		float radius;
		float size;
		int numberVertex, lats, longs;
		vector<glm::vec3> positions;
		vector<glm::vec3> positionsInverse;
	public:
		void addPosition(float a, float b, float c)
		{
			positions.push_back(glm::vec3(a, b, c));
			positionsInverse.push_back(glm::vec3(a * -1, b * -1, c * -1));
		}
		void setVertex(float vertex[])
		{
			int cont = 0;
			float radiusNegative = radius * -1;
			for (int i = 0; i <= lats; i++)
			{
				float lat0 = M_PI * (-0.5 + (float) (i - 1) / lats);
				float alpha1 = sin(lat0) * radius;
				float theta1 = cos(lat0) * radius;

				float lat1 = M_PI * (-0.5 + (float) i / lats);
				float alpha2 = sin(lat1) * radius;
				float theta2 = cos(lat1) * radius;

				for (int j = 0; j <= longs; j++)
				{
					float lng = 2 * M_PI * (float) (j - 1) / longs;
					float x = cos(lng);
					float y = sin(lng);

					vertex[cont++] = x * theta1;
					vertex[cont++] = y * theta1;
					vertex[cont++] = alpha1;
					vertex[cont++] = x * theta2;
					vertex[cont++] = y * theta2;
					vertex[cont++] = alpha2;

				}
			}

		}

		Particle(float r, int inLats, int inLongs)
		{
			size = r * 2;
			radius = r;
			numberVertex = 6 * (inLongs + 1) * (inLats + 1);
			lats = inLats;
			longs = inLongs;
		}
		int coordinate(int i, int j)
		{
			return i + j * (lats + 1);
		}

		int getNumberVertex() const
		{
			return numberVertex;
		}

		const glm::vec3& getPosition(int i) const
		{
			return positions[i];
		}
		const glm::vec3& getPositionInverse(int i) const
		{
			return positionsInverse[i];
		}
		void update(float maxDimension)
		{
			float inverseMax = maxDimension * -1;
			int randomUpdateDirection, negative;
			float increment = 0.03f, value;
			for (int i = 0; i < positions.size(); i++)
			{
				randomUpdateDirection = rand() % 3;
				negative = rand() % 2;
				if (negative == 0)
					increment *= -1;

				switch (randomUpdateDirection)
				{
					case 0:
						value = positions[i].x + increment;
						if ((value < maxDimension) && (value > (inverseMax)))
						{
							positions[i].x = value;
							positionsInverse[i].x = -positions[i].x;
						}
						break;
					case 1:
						value = positions[i].y + increment;
						if ((value < maxDimension) && (value > (inverseMax)))
						{
							positions[i].y = value;
							positionsInverse[i].y = -positions[i].y;
						}
						break;
					case 2:
						value = positions[i].z + increment;
						if ((value < maxDimension) && (value > (inverseMax)))
						{
							positions[i].z = value;
							positionsInverse[i].z = -positions[i].z;
						}
						break;
					default:
						cout << "error update" << endl;
				}

			}
		}
		void printPosition()
		{
			for (int i = 0; i < positions.size(); i++)
				cout << positions[i].x << "/" << positions[i].y << "/" << positions[i].z << "  ------- "
						<< positionsInverse[i].x << "/" << positionsInverse[i].y << "/" << positionsInverse[i].z
						<< endl;
			cout << positions.size() << "             " << positionsInverse.size() << endl;
		}

};

#endif /* SRC_PARTICLE_H_ */

