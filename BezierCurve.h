#ifndef BEZIER_CURVE_H
#define BEZIER_CURVE_H

#include "Vec3Macros.h"

#ifdef __cplusplus
extern "C"
{
	#endif
	struct BezierCurve
	{
		vec3* vertices;
		int vertexAmount;
		int vertexCounter;
	};
	int InitBezierCurve( struct BezierCurve* curve, int vertices );
	void FreeBezierCurve(struct BezierCurve* curve);
	int PushVertexBezierCurveVec3(struct BezierCurve* curve, vec3 position);
	int PushVertexBezierCurveXYZ(struct BezierCurve* curve, float x, float y, float z);

	// Returns the size of the array in floats
	int ConvertBezierCurveToFloatArray(struct BezierCurve* curve, float** destArray);

	#ifdef __cplusplus
}
#endif
#endif
