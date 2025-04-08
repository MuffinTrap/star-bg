#include "Star.h"

// Vec3
#include "Vec3Macros.h"

// M_PI, sin, cos
#include <math.h>

#ifdef __cplusplus
extern "C"
{
    #endif

    struct ProceduralMesh CreateStarMeshBorder(float radius, float chonkiness, float borderThickness)
    {
        float pointRadius = radius;
        float ratio = chonkiness;
        float baseRadius = pointRadius * ratio;
        //////////////////////////////////////////

        struct ProceduralMesh mesh;
        InitProceduralMesh(&mesh, "Star Border", 5 * 6, 12 * 5);

        vec3 point = V3f_Create(1.0f, 0.0f, 0.0f);
        float fifth = M_PI*2/5.0f;
        float tenth = fifth/2.0f;

        // This is a magic number to make the borders of even thickness
        float fixRatio = 0.50f;

        vec3 color = V3f_Create(0.0f, 0.5f, 0.7f);
        for (int p = 0; p < 5; p++)
        {
            // star is facing Z axis
            vec3 baseRot1;  V3f_RotateZ(point, fifth * p, baseRot1);
            vec3 baseRot2;  V3f_RotateZ(point, fifth * (p+1), baseRot2);
            vec3 pointRot;  V3f_RotateZ(point, fifth * p + tenth, pointRot);

            // Inner points
            vec3 rimPointI ;   V3f_Scale(pointRot, pointRadius - borderThickness, rimPointI);
            vec3 basePoint1I ; V3f_Scale(baseRot1 , baseRadius - borderThickness * fixRatio, basePoint1I);
            vec3 basePoint2I ; V3f_Scale(baseRot2 , baseRadius - borderThickness * fixRatio, basePoint2I);

            // Outer points
            vec3 rimPointO ;   V3f_Scale(pointRot, pointRadius, rimPointO);
            vec3 basePoint1O ; V3f_Scale(baseRot1 , baseRadius, basePoint1O);
            vec3 basePoint2O ; V3f_Scale(baseRot2 , baseRadius, basePoint2O);

            uint16_t rim_in =    PushVertexProceduralMesh(&mesh, rimPointI, color);
            uint16_t base1_in =  PushVertexProceduralMesh(&mesh, basePoint1I, color);
            uint16_t base1_out = PushVertexProceduralMesh(&mesh, basePoint1O, color);

            uint16_t rim_out =   PushVertexProceduralMesh(&mesh, rimPointO, color);
            uint16_t base2_in =  PushVertexProceduralMesh(&mesh, basePoint2I, color);
            uint16_t base2_out = PushVertexProceduralMesh(&mesh, basePoint2O, color);

            PushTriangleProceduralMesh(&mesh, rim_in, base1_in, base1_out); //3
            PushTriangleProceduralMesh(&mesh, base1_out, rim_out, rim_in); //6
            PushTriangleProceduralMesh(&mesh, rim_in, base2_in, base2_out); //9
            PushTriangleProceduralMesh(&mesh, base2_out, rim_out, rim_in); //12
        }


        return mesh;
    }

    struct ProceduralMesh CreateStarMesh(float radius, float chonkiness)
    {
        float pointRadius = radius;
        float ratio = chonkiness;
        float baseRadius = pointRadius * ratio;
        float thickness = 0.0f;
        //////////////////////////////////////////
        vec3 color = V3f_Create(0.0f, 0.5f, 0.7f);

        struct ProceduralMesh mesh;
        InitProceduralMesh(&mesh, "Star", 1 + 5*3, 6 * 5);

        vec3 point = V3f_Create(1.0f, 0.0f, 0.0f);
        float fifth = M_PI*2/5.0f;
        float tenth = fifth/2.0f;

        vec3 topCenter = V3f_Create(0, thickness * -1.0f, 0);

        uint16_t top_center = PushVertexProceduralMesh(&mesh, topCenter, color);

        for (int p = 0; p < 5; p++)
        {
            // star is facing Z axis
            vec3 baseRot1 ; V3f_RotateZ(point, fifth * p, baseRot1);
            vec3 baseRot2 ; V3f_RotateZ(point, fifth * (p+1), baseRot2);
            vec3 pointRot ; V3f_RotateZ(point, fifth * p + tenth, pointRot);

            vec3 rimPoint   ; V3f_Scale(pointRot, pointRadius, rimPoint);
            vec3 basePoint1 ; V3f_Scale(baseRot1 , baseRadius, basePoint1);
            vec3 basePoint2 ; V3f_Scale(baseRot2 , baseRadius, basePoint2);

            // vec3 normal1 = CalculateTriangleNormal(basePoint1, topCenter, rimPoint);
            uint16_t rim =   PushVertexProceduralMesh(&mesh, rimPoint, color);
            uint16_t base1 = PushVertexProceduralMesh(&mesh, basePoint1, color);
            uint16_t base2 = PushVertexProceduralMesh(&mesh, basePoint2, color);

            PushTriangleProceduralMesh(&mesh, top_center, rim, base1);
            PushTriangleProceduralMesh(&mesh, top_center, base2, rim);
        }

        return mesh;
    }

    #ifdef __cplusplus
}
#endif
