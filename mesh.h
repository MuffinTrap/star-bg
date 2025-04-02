#ifndef MESH_H
#define MESH_H

// Vec3
#include <mgdl/ccVector/ccVector.h>

#include "color.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct Mesh
{
	float* vertices;
	int vertexAmount;
};

void PushVertex(struct Mesh* mesh, vec3 v);
void DrawMesh(float centerX, float centerY, float rotationRad, float scale, struct Color* color, struct Mesh* mesh);

// -- rotate around z
vec3 rotatez(vec3 p, float angle);

#ifdef __cplusplus
}
#endif

#endif
