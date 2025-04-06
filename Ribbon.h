#ifndef RIBBON_H
#define RIBBON_H

#include "ProceduralMesh.h"

struct ProceduralMesh CreateRibbonMesh(const float* points3D, int pointAmount, int pointsPerBezier);

#endif
