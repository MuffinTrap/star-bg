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

void DrawStar2D(float centerX, float centerY, float rotationRad, struct StarParameters* param, struct StarMesh* mesh)
{
	glPushMatrix();
	glTranslatef(centerX, centerY, 0.0f);
	glRotatef(rotationRad, 0.0f, 0.0f, 1.0f);
	glScalef(param->radius, param->radius, 1.0f);
	glBegin(GL_TRIANGLES);

	int tris = mesh->vertexAmount / 3;
	int vi = 0;
	glColor3f(param->r, param->g, param->b);

	for (int i = 0; i < tris; i++)
	{
		vi = i*3*3;
		glVertex2f(mesh->vertices[vi+0], mesh->vertices[vi+1]);//, mesh->vertices[vi+2]);
		glVertex2f(mesh->vertices[vi+3], mesh->vertices[vi+4]);//, mesh->vertices[vi+5]);
		glVertex2f(mesh->vertices[vi+6], mesh->vertices[vi+7]);//, mesh->vertices[vi+8]);
	}

	glEnd();
	glPopMatrix();
}

void SetStarParameters(struct StarParameters *params, float radius, float chonkiness, float r, float g, float b)
{
	params->radius = radius;
	params->chonkiness = chonkiness;
	params->r=r;
	params->g=g;
	params->b=b;
}

static void PushVertex(struct StarMesh* mesh, vec3 v)
{
	int va = mesh->vertexAmount;
	mesh->vertices[va*3 + 0] = v.x;
	mesh->vertices[va*3 + 1] = v.y;
	mesh->vertices[va*3 + 2] = v.z;
	mesh->vertexAmount++;
}

// -- rotate around z
static vec3 rotatez(vec3 p, float angle) {
	float xt = p.x*cos(angle) - p.y*sin(angle);
	float yt = p.x*sin(angle) + p.y*cos(angle);
	float zt = p.z;
	return vec3New(xt, yt, zt);
}
struct StarMesh CreateStarMeshBorder(float thickness)
{
	float pointRadius = 1.0f;
	int sides = 1.0f;
	float ratio = 0.5f;
	float baseRadius = pointRadius * ratio;
	//////////////////////////////////////////

	struct StarMesh mesh;
	mesh.vertices = (float*)malloc((sizeof(float) * 3) * 5 * 12);
	mesh.vertexAmount = 0;

    vec3 point = vec3New(1.0f, 0.0f, 0.0f);
    float fifth = M_PI*2/5.0f;
    float tenth = fifth/2.0f;

    for (short side = 0; side < sides; side++)
    {
        for (int p = 0; p < 5; p++)
        {
            // star is facing Z axis
            vec3 baseRot1 = rotatez(point, fifth * p);
            vec3 baseRot2 = rotatez(point, fifth * (p+1));
            vec3 pointRot = rotatez(point, fifth * p + tenth);

			// Inner points
            vec3 rimPointI = vec3Multiply(pointRot, pointRadius - thickness);
            vec3 basePoint1I = vec3Multiply(baseRot1 , baseRadius - thickness);
            vec3 basePoint2I = vec3Multiply(baseRot2 , baseRadius - thickness);

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
            }
            if (side == 0)
            {

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

struct StarMesh CreateStarMesh()
{
	float pointRadius = 1.0f;
	int sides = 1.0f;
	float ratio = 0.5f;
	float baseRadius = pointRadius * ratio;
	float thickness = 0.0f;
	//////////////////////////////////////////

	struct StarMesh mesh;
	mesh.vertices = (float*)malloc((sizeof(float) * 3) * 5 * 6);
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
            }
            /*
            else
            {
                S.positions.push_back(topCenter);
                S.positions.push_back(basePoint1);
                S.positions.push_back(rimPoint);
                //normal1 = CalculateTriangleNormal(basePoint1, rimPoint, topCenter);
            }
            for (int n = 0; n < 4; n++)
            {
                S.normals.push_back(normal1);
            }
            */

            // vec3 normal2 = CalculateTriangleNormal(basePoint2, rimPoint, topCenter);
            if (side == 0)
            {
                PushVertex(&mesh, topCenter);
                PushVertex(&mesh, basePoint2);
                PushVertex(&mesh, rimPoint);
            }
            /*
            else
            {
                S.positions.push_back(topCenter);
                S.positions.push_back(rimPoint);
                S.positions.push_back(basePoint2);
                // normal2 = CalculateTriangleNormal(basePoint2, topCenter, rimPoint);
            }

            for (int n = 0; n < 4; n++)
            {
                S.normals.push_back(normal2);
            }
            */
        }
    }

    printf("Created star mesh with %d vertices\n", mesh.vertexAmount);
    return mesh;
}

#ifdef __cplusplus
}
#endif
