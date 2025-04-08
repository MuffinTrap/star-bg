#ifndef RIBBON_H
#define RIBBON_H

#include "ProceduralMesh.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct Ribbon
{
	struct ProceduralMesh* mesh;
	int vertexAmount;
	int indexAmount;
	int verticesOnCrossSection;
	int pointAmount; // this is how many points were sampled from the spline
	int pointsPerBezier;
};

struct Ribbon CreateRibbonMesh(const float* points3D, int pointAmount, int pointsPerBezier, int verticesOnCrossSection, float radius, int r, int g, int b);

void DrawRibbonPartially(struct Ribbon* ribbon, int startPercentage, int endPercentage);
void FreeRibbonMemory();

#ifdef __cplusplus
}
#endif

#endif
