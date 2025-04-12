#include "SkyCylinder.h"
#include "Vec3Macros.h"

/* Modifies the vertices of a mesh so that it is projected on an upwards cylinder
 * inner surface. The mesh is expected to be mostly on XY plane and in model coordinates
 * so that mesh center is (0,0,0)
 * Parameters:
 * @ mesh : The mesh to convert
 * @ cylinder : The cylinder to map it to
 * @ y : this is added to all the vertices of the mesh
 * @ facingAngle : To which direction the mesh is projected, in radians
 *
 * Make sure that the cylinder circumfere is waaay wider than the mesh on X-axis
 * otherwise it will be warped as the vertices loop around the cylinder
 *
 */
void ProjectMeshToCylinder(struct ProceduralMesh* mesh, struct SkyCylinder* cylinder, float y, float facingAngle)
{

	// Calculate circumfere
	// Calculate how many radians is 1 unit on X
	// Take mesh
	// Take each vertex
	// move vertex away from cylinder center on Z axis
	// replace the vertex with that vertex rotated from X:0 to new position
	// around Y using the radiansToX calculated before + facingAngle
	// Adjust Y

	float circumfere = cylinder->radius * 2.0f * M_PI;
	float radiansPerX = (M_PI * 2.0f) / circumfere;
	printf("Circ %.2f radPerX %.2f\n", circumfere, radiansPerX);

	vec3 v;
	vec3 rotated;
	float rotateFromX;
	for (int i = 0; i < mesh->vertex_count; i++)
	{
		printf("Convert :");
		V3f_FromVec3Array(mesh->vertices, i, v);
		V3f_Print(v);
		//
		V3f_Z(v) = cylinder->radius;

		rotateFromX = V3f_X(v) * radiansPerX;
		// Set x to zero, we only want to rotate a Z vector
		V3f_X(v) = 0.0f;
		V3f_RotateY(v, rotateFromX + facingAngle, rotated);

		V3f_Y(rotated) += y;
		V3f_Print(rotated);

		V3f_Copy(rotated, mesh->vertices[i]);
	}
}
