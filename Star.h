#ifndef STAR_BG
#define STAR_BG

#include "ProceduralMesh.h"


#ifdef __cplusplus
extern "C"
{
#endif

struct ProceduralMesh CreateStarMesh(float radius, float chonkiness);
struct ProceduralMesh CreateStarMeshBorder(float radius, float chonkiness, float borderThickness);


#ifdef __cplusplus
}
#endif

#endif

