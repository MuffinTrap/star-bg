
#include "Ribbon.h"
#include "Vec3Macros.h"

/*
 *  Use previous indices
 *
 *
 *
 *
 *
 *
 */

// To avoid in-out parameters
static uint16_t prev_left = 0;
static uint16_t prev_right = 0;

static void PushSegment(struct ProceduralMesh* mesh, vec3 prev, vec3 next, vec3 color, bool isFirstPoint)
{
	//printf("Push segment %d, %d\n", prev_left, prev_right);
	vec3 dir; V3f_Sub(next, prev, dir);
	V3f_Normalize(dir);

	vec3 visible = V3f_Create(0.0f, 0.0f, 1.0f);
	vec3 right; V3f_Cross(dir, visible, right);
	vec3 up; V3f_Cross(dir, right, up);
	vec3 left; V3f_Scale(right, -1.0f, left);

	if (isFirstPoint)
	{
		vec3 prevLeft; V3f_Add(prev, left, prevLeft);
		vec3 prevRight; V3f_Add(prev, right, prevRight);
		//V3f_Print(prevLeft);
		//V3f_Print(prevRight);
		prev_left = PushVertexProceduralMesh(mesh, prevLeft, color);
		prev_right = PushVertexProceduralMesh(mesh, prevRight, color);
	}

	vec3 nextLeft; V3f_Add(next, left, nextLeft);
	vec3 nextRight;  V3f_Add(next, right, nextRight);
	//V3f_Print(nextRight);
	//V3f_Print(nextLeft);
	uint16_t next_left = PushVertexProceduralMesh(mesh, nextLeft, color);
	uint16_t next_right = PushVertexProceduralMesh(mesh, nextRight, color);

	PushTriangleProceduralMesh(mesh, prev_left, prev_right, next_right);
	PushTriangleProceduralMesh(mesh, next_right, next_left, prev_left);

	// Update for next call
	prev_left = next_left;
	prev_right = next_right;
}

struct ProceduralMesh CreateRibbonMesh(const float* points3D, int pointAmount, int pointsPerBezier)
{
	struct ProceduralMesh mesh;

	int pointsPerSegment = 2; // Flat ribbon
	// Point amount:
	int vertices = 2 * (pointsPerBezier);
	int indices = 6 * (pointsPerBezier-1);

	InitProceduralMesh(&mesh, "Ribbon", vertices, indices);
	// Create a bezier curve that goes through the points
	// Generate ribbon along the curve
	vec3 color;
	V3f_HexToColor(0xF2, 0xC6, 0xDE, color);

	vec3 P0;
	vec3 C0;
	vec3 C1;
	vec3 P1;

	vec3 next;
	vec3 current;

	// Take first 4 points

	for (int i = 0; i < pointAmount; i += 4)
	{
		V3f_FromFloatArray(points3D, (i*3), P0);
		V3f_FromFloatArray(points3D, ((i+1)*3), C0);
		V3f_FromFloatArray(points3D, ((i+2)*3), C1);
		V3f_FromFloatArray(points3D, ((i+3)*3), P1);

		/*
		printf("Segment %d \n", i);
			V3f_Print(P0);
			V3f_Print(C0);
			V3f_Print(C1);
			V3f_Print(P1);
			*/

		float s = 0;
		float step = 1.0f / (float)pointsPerBezier;


		for (int p = 0; p < pointsPerBezier-1; p++)
		{
			bool isFirstSegment = (i == 0 && p == 0);
			V3f_BezierFuncV3(s, P0, C0, C1, P1, current);
			V3f_BezierFuncV3(s+step, P0, C0, C1, P1, next);

			PushSegment(&mesh, current, next, color, isFirstSegment);

			s += step;
		}
	}

	return mesh;
}
