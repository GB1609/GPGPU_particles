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
		void setVertexAndIndices(float vertex[], unsigned int indices[])
		{

			int i, j;

			int cont = 0;
			for (i = 0; i <= lats; i++)
			{
				double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
				double z0 = sin(lat0);
				double zr0 = cos(lat0);

				double lat1 = M_PI * (-0.5 + (double) i / lats);
				double z1 = sin(lat1);
				double zr1 = cos(lat1);

				for (j = 0; j <= longs; j++)
				{
					double lng = 2 * M_PI * (double) (j - 1) / longs;
					double x = cos(lng);
					double y = sin(lng);

					vertex[cont++] = x * zr0;
					vertex[cont++] = y * zr0;
					vertex[cont++] = z0;
					vertex[cont++] = x * zr1;
					vertex[cont++] = y * zr1;
					vertex[cont++] = z1;

				}
			}

		}

		Particle(float r, int cMax, int rMax)
		{
			size = r * 2;
			radius = r;
			numberVertex = 7 * rMax * cMax;
			lats = cMax;
			longs = rMax;
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

