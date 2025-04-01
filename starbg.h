#ifndef STAR_BG
#define STAR_BG

#ifdef __cplusplus
extern "C"
{
#endif


struct StarMesh
{
	float* vertices;
	int vertexAmount;
};

struct StarColor
{
	float r;
	float g;
	float b;
};

struct StarMesh CreateStarMesh(float radius, float chonkiness);
struct StarMesh CreateStarMeshBorder(float radius, float chonkiness, float borderThickness);

void DrawStar2D(float centerX, float centerY, float rotationRad, float scale, struct StarColor* color, struct StarMesh* mesh);

#ifdef __cplusplus
}
#endif

#endif

