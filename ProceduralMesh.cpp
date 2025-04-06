
#include "ProceduralMesh.h"
#include <stdio.h>  // for error handling
#include "Vec3Macros.h"


void InitProceduralMesh(
    struct ProceduralMesh* mesh,
    const char* name,
    int max_vertices,
    int max_indices
) {
    mesh->name = name;
    mesh->index_count = 0;
    mesh->vertex_count = 0;
    mesh->max_indices = max_indices;
    mesh->max_vertices = max_vertices;
    // Allocate memory and check if malloc succeeds
    mesh->vertices = (vec3*)malloc(sizeof(vec3) * max_vertices);
    if (mesh->vertices == NULL) {
        fprintf(stderr, "Error: Memory allocation for vertices failed. Item: %s\n", mesh->name);
        return;
    }

    mesh->colours = (vec3*)malloc(sizeof(vec3) * max_vertices);
    if (mesh->colours == NULL) {
        fprintf(stderr, "Error: Memory allocation for colours failed. Item: %s\n", mesh->name);
        free(mesh->vertices);  // Free previously allocated memory before returning
        return;
    }

    mesh->indecies = (uint16_t*)malloc(sizeof(uint16_t) * max_indices);
    if (mesh->indecies == NULL) {
        fprintf(stderr, "Error: Memory allocation for indecies failed. Item: %s\n", mesh->name);
        free(mesh->vertices);
        free(mesh->colours);
        return;
    }

    mesh->position_x = mesh->position_y = mesh->position_z = 0.0f;
    mesh->pitch = mesh->yaw = mesh->roll = 0.0f;
    mesh->scale_x = mesh->scale_y = mesh->scale_z = 1.0f;
#ifndef N64
    printf("Item %s built:\n", mesh->name);
    printf("\tIndex count %i\n", mesh->index_count);
    printf("\tMax indecies %i\n", mesh->max_indices);
    printf("\tMax vertices %i\n", mesh->max_vertices);
    printf("\tVertex Data Address %p\n", (void*)mesh->vertices);
    printf("\tColour Data Address %p\n", (void*)mesh->colours);
    printf("\tIndex Data Address %p\n\n", (void*)mesh->indecies);
#endif
}

void FreeProceduralMesh(struct ProceduralMesh* mesh) {
    if (mesh->vertices) free(mesh->vertices);
    if (mesh->colours) free(mesh->colours);
    if (mesh->indecies) free(mesh->indecies);
}

int PushVertexProceduralMesh(struct ProceduralMesh* mesh, vec3 position, vec3 colour) {
    if (mesh->vertex_count >= mesh->max_vertices) {
        fprintf(stderr, "Error: Exceeded maximum number of vertices. Item: %s\n", mesh->name);
        return -1;  // Error code
    }

    int i = mesh->vertex_count;
    V3f_Copy(position, mesh->vertices[i]);
    V3f_Copy(colour, mesh->colours[i]);
    return mesh->vertex_count++;
}

void PushTriangleProceduralMesh(struct ProceduralMesh* mesh, uint16_t a, uint16_t b, uint16_t c) {
#ifndef N64
    if (mesh->index_count + 3 > mesh->max_indices) {
        fprintf(stderr, "Error: Exceeded maximum number of indices %d >= %d. Item: %s\n", mesh->index_count+3, mesh->max_indices, mesh->name);
        return;
    }
#endif

    printf("index %i = (%i;%i;%i);", mesh->index_count, a, b, c);
    mesh->indecies[mesh->index_count++] = a;
    mesh->indecies[mesh->index_count++] = b;
    mesh->indecies[mesh->index_count++] = c;

}

void DrawProceduralMesh(struct ProceduralMesh* mesh) {

#ifndef N64
    if (mesh->vertices == NULL) {
        fprintf(stderr, "Error: Memory corruption at runtime for vertices failed. Item: %s\n", mesh->name);
        free(mesh->vertices);
        free(mesh->colours);
        free(mesh->indecies);
        return;
    }

    if (mesh->colours == NULL) {
        fprintf(stderr, "Error: Memory corruption at runtime for colours failed. Item: %s\n", mesh->name);
        free(mesh->vertices);
        free(mesh->colours);
        free(mesh->indecies);
        return;
    }

    if (mesh->indecies == NULL) {
        fprintf(stderr, "Error: Memory corruption at runtime for indecies failed. Item: %s\n", mesh->name);
        free(mesh->vertices);
        free(mesh->colours);
        free(mesh->indecies);
        return;
    }
    /*Zprintf("Item to draw %s:\n", mesh->name);
    printf("\tIndex count %i\n", mesh->index_count);
    printf("\tMax indecies %i\n", mesh->max_indices);
    printf("\tMax vertices %i\n", mesh->max_vertices);
    printf("\tVertex Data Address %i\n", mesh->vertices);
    printf("\tColour Data Address %i\n", mesh->colours);
    printf("\tIndex Data Address %i\n\n", mesh->indecies);*/
#endif

    // Enable and configure depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);

    // Enable lighting and normalization
    //glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    mat4 model;
    glPushMatrix();
    glTranslatef(mesh->position_x, mesh->position_y, mesh->position_z);
    glRotatef(mesh->pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(mesh->yaw,   0.0f, 1.0f, 0.0f);
    glRotatef(mesh->roll,  0.0f, 0.0f, 1.0f);
    glScalef(mesh->scale_x, mesh->scale_y, mesh->scale_z);

    // Enable vertex and color arrays
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Point OpenGL to your data
    glVertexPointer(3, GL_FLOAT, 0, mesh->vertices);
    glColorPointer(3, GL_FLOAT, 0, mesh->colours);

    // Draw the indexed triangles
    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_SHORT, mesh->indecies);

    // Clean up
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

/*
    glBegin(GL_LINES);
    for (int i = 0; i < mesh->vertex_count; i++) {
        glColor3f(mesh->colours[i][0], mesh->colours[i][1], mesh->colours[idx][2]);
        glVertex3f(mesh->vertices[i][0], mesh->vertices[i][1], mesh->vertices[i][2]);
        //printf("%i(%f;%f;%f);", idx, mesh->vertices[idx][0], mesh->vertices[idx][1], mesh->vertices[idx][2]);
    }
    glEnd();*/
    //printf("\n\n\n\n");
    glPopMatrix();
}

void SetPositionProceduralMesh(struct ProceduralMesh* mesh, float x, float y, float z)
{
    mesh->position_x = x;
    mesh->position_y = y;
    mesh->position_z = z;
}
void SetScaleProceduralMesh(struct ProceduralMesh* mesh, float scale)
{
    mesh->scale_x = scale;
    mesh->scale_y = scale;
    mesh->scale_z = scale;
}

void SetRotationsProceduralMesh(struct ProceduralMesh* mesh, float pitch, float yaw, float roll)
{
    mesh->pitch = pitch;
    mesh->yaw = yaw;
    mesh->roll = roll;
}
