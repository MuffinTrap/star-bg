
#include "Ribbon.h"
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
static uint16_t* prevSegmentIndices;
static uint16_t* nextSegmentIndices;
static float* xyDotProducts;

static void PushSegment(struct ProceduralMesh* mesh, vec3 prev, vec3 next, vec3 color, int vertices, float radius, bool isFirstPoint)
{
	//printf("Push segment %d, %d\n", prev_left, prev_right);
	vec3 dir; V3f_Sub(next, prev, dir);
	V3f_Normalize(dir);

	vec3 visible = V3f_Create(0.0f, 0.0f, 1.0f);
	vec3 right; V3f_Cross(dir, visible, right);
	vec3 up; V3f_Cross(dir, right, up);

	vec3 dotRight;
	vec3 dotUp;
	vec3 vertex;

	// The right and up are in relation to the dir
	// Multiply them by the dot products from the XY plane points

	if (isFirstPoint)
	{
		for(int v = 0; v < vertices; v++)
		{
			V3f_Scale(right, xyDotProducts[v*2 + 0], dotRight);
			V3f_Scale(up, xyDotProducts[v*2 + 1], dotUp);

			V3f_Add(dotRight, dotUp, vertex);
			V3f_Scale(vertex, radius, vertex);
			V3f_Add(prev, vertex, vertex);
			//V3f_Print(prevLeft);
			//V3f_Print(prevRight);
			prevSegmentIndices[v] = PushVertexProceduralMesh(mesh, vertex, color);
		}
	}

	for(int v = 0; v < vertices; v++)
	{
		V3f_Scale(right, xyDotProducts[v*2 + 0], dotRight);
		V3f_Scale(up, xyDotProducts[v*2 + 1], dotUp);

		V3f_Add(dotRight, dotUp, vertex);
		V3f_Scale(vertex, radius, vertex);
		V3f_Add(next, vertex, vertex);
		//V3f_Print(prevLeft);
		//V3f_Print(prevRight);
		nextSegmentIndices[v] = PushVertexProceduralMesh(mesh, vertex, color);
	}

	int faces = vertices;
	if (vertices == 2)
	{
		faces = 1;
	}
	int indice = 0;
	int nextIndice = 0;
	uint16_t A, B, C, D;
	for (int face = 0; face < faces; face++)
	{
		/* prev     next
		 *  A ----C
		 *  | \   |
		 *  |  \  |
		 *  |   \ |
		 *  B ----D
		 */
		nextIndice = (indice + 1) % vertices;// Wrap around to first
		A = prevSegmentIndices[indice];
		B = prevSegmentIndices[nextIndice];
		C = nextSegmentIndices[indice];
		D = nextSegmentIndices[nextIndice];

		PushTriangleProceduralMesh(mesh, A, B, D);
		PushTriangleProceduralMesh(mesh, D, C, A);
		indice += 1;
	}

	// Update for next call
	for(int v = 0; v < vertices; v++)
	{
		prevSegmentIndices[v] = nextSegmentIndices[v];
	}
}

/*  Generates a 3D ribbon mesh
 *  paramters
 *  @ points3D : array containing the coordinates of bezier points and control points.
 *  x,y,z,x,y,z etc..
 *  Point role:
 *  0 : starting point of first segment
 *  1 & 2 : Control points of first segment
 *  3 : end point of first segment and start of second
 *  3 & 4 : Control points of second segment
 *  5 : end and start of third etc...
 *
 *  @ pointAmount: how many 3D points are in the array. Array size / 3
 *  @ pointsPerBezier : how many points are sampled from each bezier segment. Makes the ribbon more smooth
 *  @ verticesOnCrossSection : affects the shape of the cross section of the ribbon. Bigger values make it smoother.
 *  2 : Flat ribbon
 *  3 : Triangle
 *  4 : Square
 *  5 : Pentagon
 * 	etc...
 */

struct Ribbon CreateRibbonMesh(const float* points3D, int pointAmount, int pointsPerBezier, int verticesOnCrossSection, float radius, int r, int g, int b)
{
	struct Ribbon ribbon;
	if ((pointAmount - 1) % 3 != 0)
	{
		printf("Ribbon pointAmount has to be n*3 + 1\n");
		return ribbon;
	}

	if (points3D == NULL)
	{
		printf("Ribbon got null pointer as points array \n");
		return ribbon;

	}
	ribbon.mesh = (ProceduralMesh*)malloc(sizeof(ProceduralMesh));

	int segments = (pointAmount-1) / 3;
	int verticesForSample = verticesOnCrossSection;

	// Point amount:
	int vertices = verticesForSample * pointsPerBezier * segments;
	int indices = verticesForSample * 6 * (pointsPerBezier-1) * segments;

	InitProceduralMesh(ribbon.mesh, "Ribbon", vertices, indices);

	// Calculate the cross section points on XY plane and then
	// adjust them based on the direction of each sample
	vec3 forward = V3f_Create(1.0f, 0.0f, 0.0f);
	vec3 dirX = V3f_Create(1.0f, 0.0f, 0.0f);
	vec3 dirY = V3f_Create(0.0f, 1.0f, 0.0f);
	float rotationStep = M_PI*2/(float)verticesForSample;

	prevSegmentIndices = (uint16_t*)malloc(sizeof(uint16_t)* verticesForSample);
	nextSegmentIndices = (uint16_t*)malloc(sizeof(uint16_t)* verticesForSample);
	// Save the dot products against dirX and dirY
	xyDotProducts = (float*)malloc(sizeof(float) * verticesForSample * 2);

	vec3 rotated;
	for (int a = 0; a < verticesForSample; a++)
	{
		V3f_RotateZ(forward, rotationStep * a, rotated);
		float xDot = V3f_Dot(rotated, dirX);
		float yDot = V3f_Dot(rotated, dirY);
		xyDotProducts[a*2 + 0] = xDot;
		xyDotProducts[a*2 + 1] = yDot;
	}

	// Create a bezier curve that goes through the points
	// Generate ribbon along the curve
	vec3 color;
	//V3f_HexToColor(0xF2, 0xC6, 0xDE, color);
	V3f_HexToColor(r, g, b, color);

	vec3 P0;
	vec3 C0;
	vec3 C1;
	vec3 P1;

	vec3 next;
	vec3 current;

	// Take first 4 points
	// Advance 3 points, so that P1 becomes P0 for the next segment
	bool isFirstSegment = true;

	int segmentIndex = 0;
	for (int i = 0; i < pointAmount && segmentIndex < segments; i += 3)
	{
		segmentIndex += 1;
		V3f_FromFloatArray(points3D, ((i+0)*3), P0);
		V3f_FromFloatArray(points3D, ((i+1)*3), C0);
		V3f_FromFloatArray(points3D, ((i+2)*3), C1);
		V3f_FromFloatArray(points3D, ((i+3)*3), P1);

		/*
		 *		printf("Segment %d \n", i);
		 *			V3f_Print(P0);
		 *			V3f_Print(C0);
		 *			V3f_Print(C1);
		 *			V3f_Print(P1);
		 */

		float s = 0;
		float step = 1.0f / (float)(pointsPerBezier-1);


		for (int p = 0; p < pointsPerBezier-1; p++)
		{
			V3f_BezierFuncV3(s, P0, C0, C1, P1, current);
			V3f_BezierFuncV3(s+step, P0, C0, C1, P1, next);

			PushSegment(ribbon.mesh, current, next, color, verticesForSample, radius, isFirstSegment);

			s += step;
			isFirstSegment = false;
			/* Rainbow roading
			 *			float r = V3f_X(color);
			 *			V3f_X(color) = V3f_Y(color);
			 *			V3f_Y(color) = V3f_Z(color);
			 *			V3f_Z(color) = r;
			 */
		}
	}

	free(nextSegmentIndices);
	free(prevSegmentIndices);
	free(xyDotProducts);

	ribbon.vertexAmount = vertices;
	ribbon.indexAmount = indices;
	ribbon.verticesOnCrossSection = verticesOnCrossSection;
	ribbon.pointAmount = (pointsPerBezier-1) * segments;
	ribbon.pointsPerBezier = pointsPerBezier;
	printf("Ribbon point amount %d\n", ribbon.pointAmount);
	return ribbon;
}

// This has some math wrong.
// It should somehow always draw a bit more indices if possible.
void DrawRibbonPartially(struct Ribbon* ribbon, int startPercentage, int endPercentage)
{
	if (startPercentage < 0) startPercentage = 0;
	if (startPercentage > 100) startPercentage = 100;

	if (endPercentage < 0) endPercentage = 0;
	if (endPercentage > 100) endPercentage = 100;

	int startPoints = (int)( (float)startPercentage/100.0f * ribbon->pointAmount);
	int amountPoints = (int) ((float)(endPercentage-startPercentage)/100.0f * ribbon->pointAmount);

	int indicesPerPoint = ribbon->verticesOnCrossSection * 6;
	int indiceCount = amountPoints * indicesPerPoint;
	int vertexStart = startPoints * ribbon->verticesOnCrossSection;

	/* Debug prints
	 *	static int frame = 0;
	 *	if (frame % 60 == 0)
	 *	{
	 *		//printf("Start perc %d end perc %d\n", startPercentage, endPercentage);
	 *		//printf("Start points %d amount points %d\n", startPoints, amountPoints);
	 *		// printf("Draw from V %d/%d, %d indices \n", vertexStart, ribbon->mesh->max_vertices, indiceCount);
}
frame++;
*/
	DrawProceduralMeshPartial(ribbon->mesh, 0, indiceCount, vertexStart);
}
