#ifndef FLOWER
#define FLOWER

#include "ProceduralMesh.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct ProceduralMesh CreateFlowerMesh(float radius, float centerRatio);
struct ProceduralMesh CreateFlowerCenterMesh(float radius, float centerRatio);

#ifdef __cplusplus
}
#endif

#endif
