/*
 * Cube.h
 *
 *  Created on: 07 feb 2018
 *      Author: gb1609
 */

#ifndef SRC_CUBE_H_
#define SRC_CUBE_H_

class Cube
{
	private:
		float vertice;
		unsigned int dimV;
		unsigned int dimI;
	public:
		Cube(float vertex)
		{
			vertice = vertex;
			dimV=24;
			dimI=32;
		}
		void setVertexAndIndices(float vertice, unsigned int indices[], float vertices[])
		{
			unsigned int temp2[] = { 0, 1, 1, 2, 2, 3, 3, 0,  // front face
					7, 4, 4, 5, 5, 6, 6, 7,  // back face
					1, 2, 2, 6, 6, 5, 5, 1,  //bottom face
					0, 3, 3, 7, 7, 4, 4, 0,  //top face
					};
			float inverso = vertice * -1;
			float temp1[] = { vertice, vertice, vertice,  // top right
					vertice, inverso, vertice,  // bottom right
					inverso, inverso, vertice,  // bottom left
					inverso, vertice, vertice,  // top left
					vertice, vertice, inverso,  // top right
					vertice, inverso, inverso,  // bottom right
					inverso, inverso, inverso,  // bottom left
					inverso, vertice, inverso };
			for (int i = 0; i < dimI; i++)
			{
				if (i < dimV)
					vertices[i] = temp1[i];
				indices[i] = temp2[i];
			}
		}

		unsigned int getDimI() const
		{
			return dimI;
		}

		unsigned int getDimV() const
		{
			return dimV;
		}
};

#endif /* SRC_CUBE_H_ */
