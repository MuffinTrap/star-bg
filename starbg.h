#ifndef STAR_BG
#define STAR_BG

#ifdef __cplusplus
extern "C"
{
#endif

struct StarParameters
{
	float radius; // ]0,n]
	float chonkiness; // ]0,1]
	float r, g, b;

	// if linethickness is < 1.0f only the outline is drawn
};

struct StarMesh
{
	float* vertices;
	int vertexAmount;
};

struct StarMesh CreateStarMesh();
struct StarMesh CreateStarMeshBorder(float thickness);
void SetStarParameters(struct StarParameters *params, float radius, float chonkiness, float r, float g, float b);
void DrawStar2D(float centerX, float centerY, float rotationRad, struct StarParameters* param, struct StarMesh* mesh);
void DrawStar2DBorder(float centerX, float centerY, float rotationRad, struct StarParameters* param, struct StarMesh* mesh);

#ifdef __cplusplus
}
#endif

#endif

