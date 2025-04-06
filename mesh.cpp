#include "mesh.h"

// OpenGL
#include "OpenGLMacros.h"

#ifdef __cplusplus
extern "C"
{
#endif

void PushVertex(struct Mesh* mesh, vec3 v)
{
	int va = mesh->vertexAmount;
	mesh->vertices[va*3 + 0] = V3f_X(v);
	mesh->vertices[va*3 + 1] = V3f_Y(v);
	mesh->vertices[va*3 + 2] = V3f_Z(v);
	mesh->vertexAmount++;
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
