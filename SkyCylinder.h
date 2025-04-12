#ifndef SKY_CYLINDER_H
#define SKY_CYLINDER_H

#include "ProceduralMesh.h"

#ifdef __cplusplus
extern "C"
{
#endif

	struct SkyCylinder
	{
		float radius;
	};

	void ProjectMeshToCylinder(struct ProceduralMesh* mesh, struct SkyCylinder* cylinder, float y, float facingAngle);


#ifdef __cplusplus
}
#endif
#endif
