/*
 * Cube.h
 *
 *  Created on: 07 feb 2018
 *      Author: gb1609
 */

#ifndef SRC_CUBE_H_
#define SRC_CUBE_H_
using namespace std;
class Cube
{
	private:
		glm::vec3 position;
		unsigned int dimV;
		unsigned int dimI;
		unsigned int numbDetails;
	public:
		Cube()
		{
			dimV = 24;
			dimI = 32;
			numbDetails = 6 * 6 * 6;
			position = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		Cube(float vertex)
		{
//			vertice = vertex;
			dimV = 24;
			dimI = 32;
			numbDetails = 144;
			position = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		void setVertexAndIndices(float vertice, unsigned int indices[], float vertices[])
		{
			unsigned int temp2[] =
			{ 0, 1, 1, 2, 2, 3, 3, 0,  // front face
					7, 4, 4, 5, 5, 6, 6, 7,  // back face
					1, 2, 2, 6, 6, 5, 5, 1,  //bottom face
					0, 3, 3, 7, 7, 4, 4, 0,  //top face
					};
			float inverso = vertice * -1;
			float temp[] =
			{ vertice, vertice, vertice,  // top right
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
					vertices[i] = temp[i];
				indices[i] = temp2[i];
			}
		}

		void setVertex(float vertice, float vertices[])
		{
			float temp[numbDetails] =
			{

			-vertice, -vertice, -vertice, 0.0f, 0.0f, -1.0f, vertice, -vertice, -vertice, 0.0f, 0.0f, -1.0f, vertice,
					vertice, -vertice, 0.0f, 0.0f, -1.0f, vertice, vertice, -vertice, 0.0f, 0.0f, -1.0f, -vertice,
					vertice, -vertice, 0.0f, 0.0f, -1.0f, -vertice, -vertice, -vertice, 0.0f, 0.0f, -1.0f,

					-vertice, -vertice, vertice, 0.0f, 0.0f, 1.0f, vertice, -vertice, vertice, 0.0f, 0.0f, 1.0f,
					vertice, vertice, vertice, 0.0f, 0.0f, 1.0f, vertice, vertice, vertice, 0.0f, 0.0f, 1.0f, -vertice,
					vertice, vertice, 0.0f, 0.0f, 1.0f, -vertice, -vertice, vertice, 0.0f, 0.0f, 1.0f,

					-vertice, vertice, vertice, -1.0f, 0.0f, 0.0f, -vertice, vertice, -vertice, -1.0f, 0.0f, 0.0f,
					-vertice, -vertice, -vertice, -1.0f, 0.0f, 0.0f, -vertice, -vertice, -vertice, -1.0f, 0.0f, 0.0f,
					-vertice, -vertice, vertice, -1.0f, 0.0f, 0.0f, -vertice, vertice, vertice, -1.0f, 0.0f, 0.0f,

					vertice, vertice, vertice, 1.0f, 0.0f, 0.0f, vertice, vertice, -vertice, 1.0f, 0.0f, 0.0f, vertice,
					-vertice, -vertice, 1.0f, 0.0f, 0.0f, vertice, -vertice, -vertice, 1.0f, 0.0f, 0.0f, vertice,
					-vertice, vertice, 1.0f, 0.0f, 0.0f, vertice, vertice, vertice, 1.0f, 0.0f, 0.0f,

					-vertice, -vertice, -vertice, 0.0f, -1.0f, 0.0f, vertice, -vertice, -vertice, 0.0f, -1.0f, 0.0f,
					vertice, -vertice, vertice, 0.0f, -1.0f, 0.0f, vertice, -vertice, vertice, 0.0f, -1.0f, 0.0f,
					-vertice, -vertice, vertice, 0.0f, -1.0f, 0.0f, -vertice, -vertice, -vertice, 0.0f, -1.0f, 0.0f,

					-vertice, vertice, -vertice, 0.0f, 1.0f, 0.0f, vertice, vertice, -vertice, 0.0f, 1.0f, 0.0f,
					vertice, vertice, vertice, 0.0f, 1.0f, 0.0f, vertice, vertice, vertice, 0.0f, 1.0f, 0.0f, -vertice,
					vertice, vertice, 0.0f, 1.0f, 0.0f, -vertice, vertice, -vertice, 0.0f, 1.0f, 0.0f,
							 };

			for (int i = 0; i < numbDetails; i++)
				vertices[i] = temp[i];
		}
		unsigned int getDimI() const
		{
			return dimI;
		}

		unsigned int getDimV() const
		{
			return dimV;
		}

		const glm::vec3& getPosition() const
		{
			return position;
		}
		unsigned int getCubePrint()
		{
			return 3 * 2 * 6;
		}

		unsigned int getNumbDetails() const
		{
			return numbDetails;
		}
		void printVertex(float vertices[])
		{
			int cont = 0;
			for (int i = 0; i < numbDetails; i++)
				if (cont < 7)
				{
					cont++;
					cout << vertices[i] << "  ";
				}
				else
				{
					cont = 0;
					cout << vertices[i] << endl;
				}
		}
}
;

#endif /* SRC_CUBE_H_ */
