#ifndef FLOWER
#define FLOWER

#include "mesh.h"

#ifdef __cplusplus
extern "C"
{
#endif

	struct FlowerMesh
	{
		float* vertices;
		int vertexAmount;
	};

	struct Mesh CreateFlowerMesh(float radius, float centerRatio);
	struct Mesh CreateFlowerCenterMesh(float radius, float centerRatio);
	void DrawFlower2D(float centerX, float centerY, float rotationRad, float scale, struct Color* color, struct FlowerMesh* mesh);

#ifdef __cplusplus
}
#endif

#endif
