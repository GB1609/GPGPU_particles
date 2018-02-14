#ifndef CONEGENERATOR_H
#define CONEGENERATOR_H

#include <cmath>

int LightConeNumVertices(int numVerts, int componentsPerVertex);
int LightConeNumIndices(int numVerts, float topRadius, float bottomRadius, bool outside, bool inside);
void CreateLightCone(float outVertices[], unsigned int outIndices[], int numVerts, float topRadius, float bottomRadius,
		float length, int openingAngle, bool inside, bool outside);

int LightConeNumVertices(int numVerts, int componentsPerVertex)
{
	return 2 * numVerts * componentsPerVertex;
}

int LightConeNumIndices(int numVerts, float topRadius, float bottomRadius, bool outside, bool inside)
{
	return numVerts * ((topRadius == 0.0f || bottomRadius == 0.0f) ? 3 : 6) * ((outside ? 1 : 0) + (inside ? 1 : 0));
}

void CreateLightCone(float outVertices[], unsigned int outIndices[], int numVerts, float topRadius, float bottomRadius,
		float length, int openingAngle, bool inside, bool outside)
{
	////input: vertices struct with dimension 2*nTriangle*nComponent, indices struct (dimension 2*nTriangle*nComponent), nIndex, Ntriangle, topRadius,BottomRadius,length, opening angle

	if (openingAngle > 0 && openingAngle < 180)
	{
		topRadius = 0.0f;
		bottomRadius = length * tanf(float(openingAngle) * 0.0174532925f * 0.5f);
	}

	// Generate vertices
	float invNumVerts = 1.0f / float(numVerts);
	for (int i = 0, j = 0; i < numVerts * 3; i += 3, j++)
	{
		float angle = 2 * M_PI * j * invNumVerts;
		float angleSin = sin(angle);
		float angleCos = cos(angle);
		outVertices[i] = topRadius * angleCos;
		outVertices[i + 1] = topRadius * angleSin;
		outVertices[i + 2] = 0.0f;

		outVertices[i + (numVerts * 3)] = bottomRadius * angleCos;
		outVertices[i + (numVerts * 3) + 1] = bottomRadius * angleSin;
		outVertices[i + (numVerts * 3) + 2] = length;
	}

	// Generate indices
	int count = 0;
	int offset = (outside && inside) ? 2 * numVerts : 0;
	if (topRadius == 0.0f)
	{ // Top cone
		if (outside)
		{
			for (int i = 0; i < numVerts; ++i)
			{
				outIndices[count++] = i + numVerts;
				outIndices[count++] = i;
				if (i == numVerts - 1)
					outIndices[count++] = numVerts;
				else
					outIndices[count++] = i + 1 + numVerts;
			}
		}
		if (inside)
		{
			for (int i = offset; i < numVerts + offset; ++i)
			{
				outIndices[count++] = i + numVerts;
				outIndices[count++] = i;
				if (i == numVerts - 1 + offset)
					outIndices[count++] = numVerts + offset;
				else
					outIndices[count++] = i + 1 + numVerts;
			}
		}
	}
	else if (bottomRadius == 0.0f)
	{ // Bottom cone
		if (outside)
		{
			for (int i = 0; i < numVerts; ++i)
			{
				outIndices[count++] = i;
				if (i == numVerts - 1)
					outIndices[count++] = 0;
				else
					outIndices[count++] = i + 1;
				outIndices[count++] = i + numVerts;
			}
		}
		if (inside)
		{
			for (int i = 0; i < numVerts + offset; ++i)
			{
				if (i == numVerts - 1 + offset)
					outIndices[count++] = offset;
				else
					outIndices[count++] = i + 1;
				outIndices[count++] = i;
				outIndices[count++] = i + numVerts;
			}
		}
	}
	else
	{ // Truncated cone
		if (outside)
		{
			for (int i = 0; i < numVerts; ++i)
			{
				int ip1 = i + 1;
				if (ip1 == numVerts)
					ip1 = 0;

				outIndices[count++] = i;
				outIndices[count++] = ip1;
				outIndices[count++] = i + numVerts;

				outIndices[count++] = ip1 + numVerts;
				outIndices[count++] = i + numVerts;
				outIndices[count++] = ip1;
			}
		}
		if (inside)
		{
			for (int i = 0; i < numVerts + offset; ++i)
			{
				int ip1 = i + 1;
				if (ip1 == numVerts + offset)
					ip1 = offset;

				outIndices[count++] = ip1;
				outIndices[count++] = i;
				outIndices[count++] = i + numVerts;

				outIndices[count++] = i + numVerts;
				outIndices[count++] = ip1 + numVerts;
				outIndices[count++] = ip1;
			}
		}
	}
}
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
#endif
