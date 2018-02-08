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
		int numberVertex, colMax, rowMax;
		void setVertexAndIndices(float vertex[], unsigned int indices[])
		{
			float Pi = 3.1415926535f;
			for (int j = 0; j <= rowMax; j++)
			{
				for (int i = 0; i <= colMax; i++)
				{
					int k = coordinate(i, j);
					float teta = ((float) i / colMax) * 2 * Pi;
					float fi = ((float) j / rowMax) * Pi;
					vertex[k] = radius * (cos(teta)) * (sin(fi));
					vertex[k + 1] = radius * (cos(fi));
					vertex[k + 2] = 1.0f * (sin(teta)) * (sin(fi));
				}
			}

			int k = 0;
			for (int j = 0; j < rowMax; j++)
			{
				for (int i = 0; i < colMax; i++)
				{
					indices[k++] = coordinate(i, j);
					indices[k++] = coordinate(i + 1, j + 1);
					indices[k++] = coordinate(i, j + 1);
					indices[k++] = coordinate(i, j);
					indices[k++] = coordinate(i + 1, j);
					indices[k++] = coordinate(i + 1, j + 1);
				}
			}
		}

		Particle(float r, int cMax, int rMax)
		{
			size = r * 2;
			radius = r;
			numberVertex = 3 * rMax * cMax;
			colMax = cMax;
			rowMax = rMax;

		}
		int coordinate(int i, int j)
		{
			return i + j * (colMax + 1);
		}

		int getNumberVertex() const
		{
			return numberVertex;
		}
};

#endif /* SRC_PARTICLE_H_ */

