#include "mesh.h"

// OpenGL
#include <mgdl.h>

#ifdef __cplusplus
extern "C"
{
#endif

void PushVertex(struct Mesh* mesh, vec3 v)
{
	int va = mesh->vertexAmount;
	mesh->vertices[va*3 + 0] = v.x;
	mesh->vertices[va*3 + 1] = v.y;
	mesh->vertices[va*3 + 2] = v.z;
	mesh->vertexAmount++;
}

vec3 rotatez(vec3 p, float angle) {
	float xt = p.x*cos(angle) - p.y*sin(angle);
	float yt = p.x*sin(angle) + p.y*cos(angle);
	float zt = p.z;
	return vec3New(xt, yt, zt);
}

void DrawMesh(float centerX, float centerY, float rotationRad, float scale, struct Color* color, struct Mesh* mesh)
{
	glPushMatrix();
	glTranslatef(centerX, centerY, 0.0f);
	glRotatef(rotationRad, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);
	//glBegin(GL_LINE_LOOP);
	glBegin(GL_TRIANGLES);

	glColor3f(color->r, color->g, color->b);

	for (int i = 0; i < mesh->vertexAmount * 3; i += 3)
	{
		glVertex2f(mesh->vertices[i+0], mesh->vertices[i+1]);//, mesh->vertices[vi+2]);
	}

	glEnd();
	glPopMatrix();
}

#ifdef __cplusplus
}
#endif
