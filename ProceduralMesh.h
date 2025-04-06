#ifndef PROCEDURALMESH_H
#define PROCEDURALMESH_H
#include "Vec3Macros.h"
#include "OpenGLMacros.h"

#ifdef __cplusplus
extern "C"
{
#endif
struct ProceduralMesh
{
    const char* name;

    vec3* vertices;
    vec3* colours;
    uint16_t* indecies;
    int index_count;
    int vertex_count;
    int max_vertices;
    int max_indices;

    float position_x, position_y, position_z;
    float pitch, yaw, roll;
    float scale_x, scale_y, scale_z;
};
void InitProceduralMesh(
    struct ProceduralMesh* mesh,
    const char* name,
    int max_vertices,
    int max_indices
);
int PushVertexProceduralMesh(struct ProceduralMesh* mesh, vec3 position, vec3 colour);
void PushTriangleProceduralMesh(struct ProceduralMesh* mesh, uint16_t a, uint16_t b, uint16_t c);
void DrawProceduralMesh(struct ProceduralMesh* mesh);

void SetPositionProceduralMesh(struct ProceduralMesh* mesh, float x, float y, float z);
void SetScaleProceduralMesh(struct ProceduralMesh* mesh, float scale);
void SetRotationsProceduralMesh(struct ProceduralMesh* mesh, float pitch, float yaw, float roll);
#ifdef __cplusplus
}
#endif

#endif
