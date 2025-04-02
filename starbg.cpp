#include "starbg.h"


// Vec3
#include <mgdl/ccVector/ccVector.h>

// OpenGL
#include <mgdl.h>

// M_PI, sin, cos
#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct Mesh CreateStarMeshBorder(float radius, float chonkiness, float borderThickness)
{
	float pointRadius = radius;
	int sides = 1; // Change this to 2 to make both sides
	float ratio = chonkiness;
	float baseRadius = pointRadius * ratio;
	//////////////////////////////////////////

	struct Mesh mesh;
	mesh.vertices = (float*)malloc((sizeof(float) * 3) * 5 * 12 * sides);
	mesh.vertexAmount = 0;

    vec3 point = vec3New(1.0f, 0.0f, 0.0f);
    float fifth = M_PI*2/5.0f;
    float tenth = fifth/2.0f;

    // This is a magic number to make the borders of even thickness
    float fixRatio = 0.50f;

    for (short side = 0; side < sides; side++)
    {
        for (int p = 0; p < 5; p++)
        {
            // star is facing Z axis
            vec3 baseRot1 = rotatez(point, fifth * p);
            vec3 baseRot2 = rotatez(point, fifth * (p+1));
            vec3 pointRot = rotatez(point, fifth * p + tenth);

			// Inner points
            vec3 rimPointI = vec3Multiply(pointRot, pointRadius - borderThickness);
            vec3 basePoint1I = vec3Multiply(baseRot1 , baseRadius - borderThickness * fixRatio);
            vec3 basePoint2I = vec3Multiply(baseRot2 , baseRadius - borderThickness * fixRatio);

			// Outer points
            vec3 rimPointO = vec3Multiply(pointRot, pointRadius);
            vec3 basePoint1O = vec3Multiply(baseRot1 , baseRadius);
            vec3 basePoint2O = vec3Multiply(baseRot2 , baseRadius);

            if (side == 0)
            {
                PushVertex(&mesh, rimPointI);
                PushVertex(&mesh, basePoint1I);
                PushVertex(&mesh, basePoint1O);

                PushVertex(&mesh, basePoint1O);
                PushVertex(&mesh, rimPointO);
                PushVertex(&mesh, rimPointI);

                PushVertex(&mesh, rimPointI);
                PushVertex(&mesh, basePoint2I);
                PushVertex(&mesh, basePoint2O);

                PushVertex(&mesh, basePoint2O);
                PushVertex(&mesh, rimPointO);
                PushVertex(&mesh, rimPointI);
            }
        }
    }

    printf("Created star border mesh with %d vertices\n", mesh.vertexAmount);
    return mesh;
}

struct Mesh CreateStarMesh(float radius, float chonkiness)
{
	float pointRadius = radius;
	int sides = 1;
	float ratio = chonkiness;
	float baseRadius = pointRadius * ratio;
	float thickness = 0.0f;
	//////////////////////////////////////////

	struct Mesh mesh;
	mesh.vertices = (float*)malloc((sizeof(float) * 3) * 5 * 6 * sides);
	mesh.vertexAmount = 0;

    vec3 point = vec3New(1.0f, 0.0f, 0.0f);
    float fifth = M_PI*2/5.0f;
    float tenth = fifth/2.0f;

    for (short side = 0; side < sides; side++)
    {
        vec3 topCenter;
        topCenter = vec3New(0, thickness * side * -1.0f, 0);

        for (int p = 0; p < 5; p++)
        {
            // star is facing Z axis
            vec3 baseRot1 = rotatez(point, fifth * p);
            vec3 baseRot2 = rotatez(point, fifth * (p+1));
            vec3 pointRot = rotatez(point, fifth * p + tenth);

            vec3 rimPoint = vec3Multiply(pointRot, pointRadius);
            vec3 basePoint1 = vec3Multiply(baseRot1 , baseRadius);
            vec3 basePoint2 = vec3Multiply(baseRot2 , baseRadius);

            // vec3 normal1 = CalculateTriangleNormal(basePoint1, topCenter, rimPoint);
            if (side == 0)
            {
                PushVertex(&mesh, topCenter);
                PushVertex(&mesh, rimPoint);
                PushVertex(&mesh, basePoint1);

                PushVertex(&mesh, topCenter);
                PushVertex(&mesh, basePoint2);
                PushVertex(&mesh, rimPoint);
            }
        }
    }

    printf("Created star mesh with %d vertices\n", mesh.vertexAmount);
    return mesh;
}

#ifdef __cplusplus
}
#endif
