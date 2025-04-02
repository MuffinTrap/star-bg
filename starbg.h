#ifndef STAR_BG
#define STAR_BG

#include "color.h"
#include "mesh.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct Mesh CreateStarMesh(float radius, float chonkiness);
struct Mesh CreateStarMeshBorder(float radius, float chonkiness, float borderThickness);


#ifdef __cplusplus
}
#endif

#endif

