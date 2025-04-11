
#include "Cloud.h"
#include <stdlib.h> // For rand()

static float RandFloat()
{
	return (float)(rand()%1000) / 1000.0f;
}

struct ProceduralMesh CreateCloudMesh(float radius, int steps, float randomisation)
{
	int pointAmountTotal = 0;
	int indexAmountTotal = 0;
	// Make point amount dependent on radius
	const int generations = steps;
	int* pointAmounts = (int*)malloc(sizeof(int)*generations);
	float* radiuses = (float*)malloc(sizeof(float)*generations);

	float dwindlingRadius = radius;
	float bottomY = 0.0f;
	float centerYdecrease = 0.25f;
	for (int i = 0; i < generations; i++)
	{
		float circumfere = 2.0f * M_PI * radius;
		int pointsOnRim = (int)(circumfere / 3.0f);
		// Amount must be even, otherwise the circles will join together
		if (pointsOnRim % 2 == 1)
		{
			pointsOnRim += 1;
		}
		int pointAmount =  pointsOnRim + 1;
		int indexAmount = 3 * (pointsOnRim);
		int cloudAmount = 1;

		// printf("Cloud rimPoints %d ->  vertices: %d , indices: %d\n", pointsOnRim, pointAmount, indexAmount);

		pointAmounts[i] = pointAmount;
		radiuses[i] = dwindlingRadius;

		if (i > 0) { cloudAmount = 2; }
		pointAmountTotal += pointAmount * cloudAmount;
		indexAmountTotal += indexAmount * cloudAmount;

		bottomY -= dwindlingRadius * centerYdecrease;
		dwindlingRadius *= 0.65;
	}
	//bottomY *= 0.85f;

	//////////////////////////////////////////
	vec3 color = V3f_Create(1.0f, 1.0f, 1.0f);

	struct ProceduralMesh mesh;
	InitProceduralMesh(&mesh, "Cloud", pointAmountTotal, indexAmountTotal);
	float cloudX = 0.0f;
	float cloudY = 0.0f;
	float parentCenterX = 0.0f;

	for (int i = 0; i < generations; i++)
	{
		// printf("Generation: %d\n", i);
		float angleStep = M_PI*2/ (float)(pointAmounts[i]-1);

		int clouds = 1;
		if (i > 0) { clouds = 2;};
		float randomX;
		for (int c = 0; c < clouds; c++)
		{
			//printf("-- Cloud: %d/%d\n", c+1, clouds);

			float rad = radiuses[i];
			randomX = RandFloat() * randomisation * rad;
			float randomY = RandFloat() * randomisation * rad * centerYdecrease;
			float randomR = RandFloat() * randomisation * rad * centerYdecrease;

			// On both sides
			float centerX = cloudX;
			if (c==1) {
				centerX *= -1.0f;
			}


			vec3 Center = V3f_Create(centerX + randomX, cloudY + randomY, 0);
			vec3 point = V3f_Create(radiuses[i] + randomR, 0.0f, 0.0f);
			vec3 rimPoint;
			uint16_t center = PushVertexProceduralMesh(&mesh, Center, color);
			V3f_Copy(point, rimPoint);
			V3f_Add(Center, rimPoint, rimPoint);

			// Clamp Y
			if (V3f_Y(rimPoint) < bottomY) { V3f_Y(rimPoint) = bottomY; }

			uint16_t first_rim = PushVertexProceduralMesh(&mesh, rimPoint, color);
			uint16_t prev_index = first_rim;

			// printf("Center: %d First rim: %d\n", center, first_rim);
			for (int p = 1; p < pointAmounts[i]-1; p++)
			{
				// cloud is facing Z axis
				V3f_RotateZ(point, angleStep * p, rimPoint);
				V3f_Add(Center, rimPoint, rimPoint);

				if (V3f_Y(rimPoint) < bottomY) { V3f_Y(rimPoint) = bottomY; }
				uint16_t point = PushVertexProceduralMesh(&mesh, rimPoint, color);

				PushTriangleProceduralMesh(&mesh, center, point, prev_index);
				prev_index = point;
			}
			PushTriangleProceduralMesh(&mesh, center, first_rim, prev_index);
		}
		// printf("Cloud done\n");

		// Need to adjust for randomization
		cloudX += radiuses[i] * (1.0f + RandFloat() * randomisation *  0.2f);
		cloudY -= radiuses[i] * centerYdecrease;
	}
	// printf("Full cloud done\n");

	free(pointAmounts);
	free(radiuses);

	return mesh;
}
