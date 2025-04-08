
#include "BezierCurve.h"
#include <stdio.h>  // for error handling
int InitBezierCurve( struct BezierCurve* curve, int vertices)
{
	if ((vertices-1) % 3 != 0)
	{
		fprintf(stderr, "Error: Bezier curve must have n*3+1 vertices\n");
	}
	curve->vertexAmount = vertices;
	curve->vertexCounter = 0;
	// Allocate memory and check if malloc succeeds
	curve->vertices = (vec3*)malloc(sizeof(vec3) * vertices);
	if (curve->vertices == NULL) {
		fprintf(stderr, "Error: Memory allocation for bezier curve failed\n");
		return -1;
	}
	return 0;
}

void FreeBezierCurve(struct BezierCurve* curve)
{
	free(curve->vertices);
	curve->vertexAmount = 0;
}

int PushVertexBezierCurveVec3(struct BezierCurve* curve, vec3 position)
{
	if (curve->vertexCounter >= curve->vertexAmount) {
		fprintf(stderr, "Error: Exceeded maximum number of vertices on Bezier curve\n");
		return -1;  // Error code
	}

	int i = curve->vertexCounter;

	V3f_Copy(position, curve->vertices[i]);
	curve->vertexCounter++;

	return curve->vertexCounter;
}

int PushVertexBezierCurveXYZ(struct BezierCurve* curve, float x, float y, float z)
{
	vec3 position = V3f_Create(x,y,z);
	return PushVertexBezierCurveVec3(curve, position);
}

int ConvertBezierCurveToFloatArray(struct BezierCurve* curve, float** destArray)
{
	float *array = *destArray;
	array = (float*)malloc(sizeof(float) * curve->vertexAmount * 3);
	if (destArray == NULL) {
		fprintf(stderr, "Error: Memory allocation for bezier curve float array conversion failed\n");
		return -1;
	}
	for (int i = 0; i < curve->vertexAmount; i++)
	{
		V3f_ToFloatArray(curve->vertices[i], array, i);
	}
	*destArray = array;
	return curve->vertexAmount * 3;
}
