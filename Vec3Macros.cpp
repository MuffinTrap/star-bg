#include "Vec3Macros.h"


#ifdef CC_VECTOR
vec3 V3f_RotateZFunc(vec3 p, float angle) {
	float xt = p.x*cos(angle) - p.y*sin(angle);
	float yt = p.x*sin(angle) + p.y*cos(angle);
	float zt = p.z;
	return vec3New(xt, yt, zt);
}

vec V3f_HexToColorFunc(int rx, int gx, int bx)
{
	float r = (float)rx/255.0f;
	float g = (float)gx/255.0f;
	float b = (float)bx/255.0f;
	return V3f_Create(r,g,b);
}

float V3f_BezierFuncF( float s, float P0, float C0, float C1, float P1)
{
	return (P0*(1-s)^3 + 3*C0*s*(1-s)^2 + 3*C1*s^2*(1-s) + P1*s^3);
}

vec3 V3f_BezierFuncV3(float s, vec3 P0, vec3 C0, vec3 C1, vec3 P1)
{
	float currentX = V3f_BezierFuncF(s, V3f_X(P0), V3f_X(C0), V3f_X(C1), V3f_X(P1));
	float currentY = V3f_BezierFuncF(s, V3f_Y(P0), V3f_Y(C0), V3f_Y(C1), V3f_Y(P1));
	float currentZ = V3f_BezierFuncF(s, V3f_Z(P0), V3f_Z(C0), V3f_Z(C1), V3f_Z(P1));

	vec3 current = V3f_Create(currentX, currentY, currentZ);
	return current;
}
#else

void V3f_RotateZFunc(vec3 p, float angle, vec3 out)
{
    out[0] = p[0]*cosf(angle) - p[1]*sinf(angle);
    out[1] = p[0]*sinf(angle) + p[1]*cosf(angle);
    out[2] = p[2];
}

void V3f_HexToColorFunc(int rx, int gx, int bx, vec3 color_out)
{
	float r = (float)rx/255.0f;
	float g = (float)gx/255.0f;
	float b = (float)bx/255.0f;
	vec3 c =  V3f_Create(r,g,b);
	V3f_Copy(c, color_out);
}

float V3f_BezierFuncF( float s, float P0, float C0, float C1, float P1)
{
	return glm_bezier(s, P0, C0, C1, P1);
}

void V3f_BezierFuncV3(float s, vec3 P0, vec3 C0, vec3 C1, vec3 P1, vec3 out_point)
{
	V3f_X(out_point)  = V3f_BezierFuncF(s, V3f_X(P0), V3f_X(C0), V3f_X(C1), V3f_X(P1));
	V3f_Y(out_point) = V3f_BezierFuncF(s, V3f_Y(P0), V3f_Y(C0), V3f_Y(C1), V3f_Y(P1));
	V3f_Z(out_point) = V3f_BezierFuncF(s, V3f_Z(P0), V3f_Z(C0), V3f_Z(C1), V3f_Z(P1));
}

#endif
