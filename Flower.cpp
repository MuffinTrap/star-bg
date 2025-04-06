
#include "Flower.h"
#include "ProceduralMesh.h"
#include "Vec3Macros.h"

// Create the petals of the flower
struct ProceduralMesh CreateFlowerMesh(float radius, float centerRatio) {
    int petals = 4;
    struct ProceduralMesh mesh;

    int vertices = petals * 4;
    int indices = petals * 6;
    InitProceduralMesh(&mesh, "Flower", vertices, indices);

    float fourth = M_PI * 2.0f / (float)petals;
    float d90 = M_PI / 2.0f;
    vec3 point = V3f_Create(1.0f, 0.0f, 0.0f);
    float petalWidth = (radius * 2.0f) / 3.0f;

    vec3 color = V3f_Create(0.0f, 0.5f, 0.7f);
    for (int p = 0; p < petals; p++) {
        vec3 forward, right;
        V3f_RotateZ(point, fourth * p, forward);
        V3f_RotateZ(point, fourth * p + d90, right);

        vec3 petalFoot;
        V3f_Scale(forward, petalWidth / 2.0f * centerRatio, petalFoot);

        vec3 topLeft, bottomLeft, topRight, bottomRight;
        V3f_Scale(right,  petalWidth / 2.0f, topLeft);     V3f_Add(petalFoot, topLeft, topLeft);
        V3f_Scale(right, -petalWidth / 2.0f, bottomLeft);  V3f_Add(petalFoot, bottomLeft, bottomLeft);

        V3f_Scale(forward, petalWidth, forward);
        V3f_Add(topLeft, forward, topRight);
        V3f_Add(bottomLeft, forward, bottomRight);


        int i0 = PushVertexProceduralMesh(&mesh, topRight,    color);
        int i1 = PushVertexProceduralMesh(&mesh, topLeft,     color);
        int i2 = PushVertexProceduralMesh(&mesh, bottomLeft,  color);
        int i3 = PushVertexProceduralMesh(&mesh, bottomRight, color);

        PushTriangleProceduralMesh(&mesh, i0, i1, i2);
        PushTriangleProceduralMesh(&mesh, i2, i3, i0);
    }

    return mesh;
}

// Create the center of the flower
struct ProceduralMesh CreateFlowerCenterMesh(float radius, float centerRatio) {
    struct ProceduralMesh mesh;

    int vertices = 4;
    int indices = 6;

    InitProceduralMesh(&mesh, "Flower center", vertices, indices);

    float petalWidth = (radius * 2.0f) / 3.0f;
    float d90 = M_PI / 2.0f;
    vec3 point = V3f_Create(1.0f, 0.0f, 0.0f);

    vec3 forward = V3f_Create(1.0f, 0.0f, 0.0f);
    vec3 right;
    V3f_RotateZ(point, d90, right);

    vec3 petalFoot;
    V3f_Scale(forward, petalWidth / 2.0f * centerRatio, petalFoot);

    vec3 topLeft, bottomLeft;
    V3f_Scale(right,  petalWidth / 2.0f * centerRatio, topLeft);     V3f_Add(petalFoot, topLeft, topLeft);
    V3f_Scale(right, -petalWidth / 2.0f * centerRatio, bottomLeft);  V3f_Add(petalFoot, bottomLeft, bottomLeft);

    vec3 backFoot;
    V3f_Scale(point, -petalWidth / 2.0f * centerRatio, backFoot);

    vec3 backTopLeft, backBottomLeft;
    V3f_Scale(right,  petalWidth / 2.0f * centerRatio, backTopLeft);     V3f_Add(backFoot, backTopLeft, backTopLeft);
    V3f_Scale(right, -petalWidth / 2.0f * centerRatio, backBottomLeft);  V3f_Add(backFoot, backBottomLeft, backBottomLeft);

    vec3 color = V3f_Create(1.0f, 0.9f, 0.3f);

    int i0 = PushVertexProceduralMesh(&mesh, backTopLeft,     color);
    int i1 = PushVertexProceduralMesh(&mesh, topLeft,         color);
    int i2 = PushVertexProceduralMesh(&mesh, bottomLeft,      color);
    int i3 = PushVertexProceduralMesh(&mesh, backBottomLeft,  color);

    PushTriangleProceduralMesh(&mesh, i0, i1, i2);
    PushTriangleProceduralMesh(&mesh, i3, i0, i2);

    return mesh;
}
