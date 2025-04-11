#ifndef CLOUD_H
#define CLOUD_H

#include "ProceduralMesh.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct ProceduralMesh CreateCloudMesh(float radius, int steps, float randomisation);


#ifdef __cplusplus
}
#endif


#endif
