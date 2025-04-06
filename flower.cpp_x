#include "flower.h"

struct Mesh CreateFlowerMesh(float radius, float centerRatio)
{
	struct Mesh mesh;
	int petals = 4;
	int vertexCount = petals * 6;
	mesh.vertices = (float*)malloc((sizeof(float) * 3)  * vertexCount);
	mesh.vertexAmount = 0;

	float fourth = M_PI*2.0f / (float)petals;
	float d90 = M_PI / 2.0f;
	vec3 point = vec3New(1.0f, 0.0f, 0.0f);

	float petalWidth = (radius * 2.0f) / 3.0f;

	for (int p = 0; p < petals; p++)
	{
		vec3 forward = rotatez(point, fourth * p);
		vec3 petalFoot = vec3Multiply(forward, petalWidth/2.0f * centerRatio);
		vec3 right = rotatez(point, fourth * p + d90);

		vec3 topLeft = vec3Add(petalFoot, vec3Multiply(right, petalWidth/2.0f));
		vec3 bottomLeft = vec3Add(petalFoot, vec3Multiply(right, -petalWidth/2.0f));

		vec3 topRight = vec3Add(topLeft, vec3Multiply(forward, petalWidth));
		vec3 bottomRight = vec3Add(bottomLeft, vec3Multiply(forward, petalWidth));

		PushVertex(&mesh, topRight);
		PushVertex(&mesh, topLeft);
		PushVertex(&mesh, bottomLeft);

		PushVertex(&mesh, bottomLeft);
		PushVertex(&mesh, bottomRight);
		PushVertex(&mesh, topRight);
	}

	return mesh;
}

struct Mesh CreateFlowerCenterMesh(float radius, float centerRatio)
{
	struct Mesh mesh;
	int vertexCount = 6;
	mesh.vertices = (float*)malloc((sizeof(float) * 3)  * vertexCount);
	mesh.vertexAmount = 0;
	int petals = 4.0f;

	float fourth = M_PI*2.0f / 2.0f;
	float d90 = M_PI / 2.0f;
	vec3 point = vec3New(1.0f, 0.0f, 0.0f);

	float petalWidth = (radius * 2.0f) / 3.0f;

		vec3 forward = point;
		vec3 right = rotatez(point,  d90);

		vec3 petalFoot = vec3Multiply(forward, petalWidth/2.0f * centerRatio);
		vec3 topLeft = vec3Add(petalFoot, vec3Multiply(right, petalWidth/2.0f * centerRatio));
		vec3 bottomLeft = vec3Add(petalFoot, vec3Multiply(right, -petalWidth/2.0f * centerRatio));

		vec3 backward = vec3Multiply(point, -1.0f);
		petalFoot = vec3Multiply(backward, petalWidth/2.0f * centerRatio);
		vec3 backTopLeft = vec3Add(petalFoot, vec3Multiply(right, petalWidth/2.0f * centerRatio));
		vec3 backBottomLeft = vec3Add(petalFoot, vec3Multiply(right, -petalWidth/2.0f * centerRatio));

		PushVertex(&mesh, backTopLeft);
		PushVertex(&mesh, topLeft);
		PushVertex(&mesh, bottomLeft);

		PushVertex(&mesh, backBottomLeft);
		PushVertex(&mesh, backTopLeft);
		PushVertex(&mesh, bottomLeft);

	return mesh;
}
