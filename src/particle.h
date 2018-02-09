/*
 * Cube.h
 *
 *  Created on: 07 feb 2018
 *      Author: gb1609
 */

#ifndef SRC_PARTICLE_H_
#define SRC_PARTICLE_H_

#include <glm/glm.hpp>

class Particle
{
	public:
		float radius;
		float size;
		int numberVertex, lats, longs;
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
};

#endif /* SRC_PARTICLE_H_ */

