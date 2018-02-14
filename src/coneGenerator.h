#ifndef CONEGENERATOR_H
#define CONEGENERATOR_H

#include <cmath>
class Cone
{

	public:
		void createPyramid(float vertex[], float vertice)
		{
			float inverso = -vertice;
			float temp1[] =
			{
			//front
					0.0f, vertice * 2, 0.0f, //0
					-vertice, -vertice, vertice, //1
					vertice, -vertice, vertice, //2
					//right
					0.0f, vertice * 2, 0.0f, //3
					vertice, -vertice, vertice, //4
					vertice, -vertice, -vertice, //5
					//back
					0.0f, vertice * 2, 0.0f,			//6
					vertice, -vertice, -vertice,			//7
					-vertice, -vertice, -vertice,			//8
					//left
					0.0f, vertice * 2, 0.0f,			//9
					-vertice, -vertice, -vertice,			//10
					-vertice, -vertice, vertice,			//11

					};

			for (int i = 0; i < 36; i++)
				vertex[i] = temp1[i];
		}
		int vertexPyramid()
		{
			return 36;
		}

		void createCone(float* vertex, float lenght, int increment)
		{
			float factor = M_PI / 180;
			float toe[] =
			{ 0.0, lenght, 0.0f };
			double vertexA[3], vertexB[3];
			vertexA[1] = vertexB[1] = 0.0f;
			int cont = 0;
			int a = 0;
			for (float angle = 0.0f; angle <= 360; angle += 10)
			{
				vertexB[0] = vertexA[0];
				vertexB[2] = vertexA[2];

				vertexA[0] = cos(angle * factor);
				vertexA[2] = sin(angle * factor);

				if (angle != 0)
				{
					vertex[cont++] = toe[0];
					vertex[cont++] = toe[1];
					vertex[cont++] = toe[2];
					vertex[cont++] = vertexA[0];
					vertex[cont++] = vertexA[1];
					vertex[cont++] = vertexA[2];
					vertex[cont++] = vertexB[0];
					vertex[cont++] = vertexB[1];
					vertex[cont++] = vertexB[2];
				}
			}
		}
		int dimensionCone(int increment)
		{
			return (360 / increment) * 9;
		}
};

#endif
