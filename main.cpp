#include <mgdl.h>

#include "Star.h"
#include "Flower.h"
#include "Ribbon.h"

static struct ProceduralMesh star;
static struct ProceduralMesh starBorder;
static struct ProceduralMesh flower;
static struct ProceduralMesh flowerCenter;
static struct ProceduralMesh ribbon;
static float* splinePoints;

//---------------------------------------------------------------------
void init()
{
    glViewport(0, 0, gdl::GetScreenWidth(), gdl::GetScreenHeight());
    // gdl::InitOrthoProjection();
    gdl::InitPerspectiveProjection(90.0f, 0.1f, 100.0f);
    vec3 cp = V3f_Create(0.0f, 0.0f, 1.0f);
    vec3 ct = V3f_Create(0.0f, 0.0f, 0.0f);
    vec3 cup = V3f_Create(0.0f, 1.0f, 0.0f);
    gdl::InitCamera(cp, ct, cup);

    glClearColor((float)0xbd/255.0f, (float)0xb2/255.0f, (float)0xff/255.0f, 0.0f);

    vec3 color = V3f_Create(0.6f, 0.5f, 0.9f);

    float flowerCenterRatio = 0.7f;
    flower = CreateFlowerMesh(10.0f, flowerCenterRatio);
    star = CreateStarMesh(10.0f, 0.5f);
    starBorder = CreateStarMeshBorder(10.0f, 0.5f, 1.0f);
    flowerCenter = CreateFlowerCenterMesh(80.0f, flowerCenterRatio);


    splinePoints = (float*)malloc(sizeof(float)*12);
    splinePoints[0] = -10.0f;
    splinePoints[1] = 0.0f;
    splinePoints[2] = 0.0f;

    splinePoints[3] = 0.0f;
    splinePoints[4] = 0.0f + 10.0f;
    splinePoints[5] = 0.0f;

    splinePoints[6] = 10.0f;
    splinePoints[7] = 0.0f;
    splinePoints[8] = 0.0f;

    splinePoints[9] = 14.0f;
    splinePoints[10] = -10.0f;
    splinePoints[11] = 0.0f;

    ribbon = CreateRibbonMesh(splinePoints, 4, 8);

    SetPositionProceduralMesh(&flower, 0.8f, 0.0f, -1.0f);
    SetScaleProceduralMesh(&flower, 0.1f);

    SetPositionProceduralMesh(&star, -0.8f, -0.8f, -1.0f);
    SetScaleProceduralMesh(&star, 0.1f);

    SetPositionProceduralMesh(&starBorder, -0.8f, 0.8f, -1.0f);
    SetScaleProceduralMesh(&starBorder, 0.1f);
}
// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    gdl::cross_glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    float rot = gdl::GetElapsedSeconds() * 20.0f;

    /*
    vec3 color;
    V3f_HexToColor(0xfa, 0xd1, 0xfa, color);
    */

    int scrW = gdl::GetScreenWidth();
    int scrH = gdl::GetScreenHeight();

    DrawProceduralMesh(&flower);

    float fourth = 0.25f;
    // Stars
    DrawProceduralMesh(&star);
    DrawProceduralMesh(&starBorder);
    /*

    float scale = 1.0f;
    DrawMesh(fourth * 2.0f, gdl::GetScreenHeight()/2, rot,  scale, &c, &starBorder);


    // Flower center
    c.r = 1.0f;
    c.g = 1.0f;
    c.b = 1.0f;
    DrawMesh(fourth, gdl::GetScreenHeight()/2, rot, 1.0f,  &c, &flowerCenter);
    */

    SetPositionProceduralMesh(&ribbon, 0.0f, 0.0f, -1.0f);
    SetScaleProceduralMesh(&ribbon, 0.1f);
    DrawProceduralMesh(&ribbon);


    /*
    float centerX = 0.0f;
    float centerY = 0.0f;
    float rotationRad = 0.0f;
    float scale = 1.0f;

	glPushMatrix();
	glTranslatef(centerX, centerY, 0.0f);
	glRotatef(rotationRad, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);
	//glBegin(GL_LINE_LOOP);
	//glBegin(GL_TRIANGLES);
	glBegin(GL_LINES);

	glColor3f(color.r, color.g, color.b);

    glVertex3f(0.0f, 0.0f, -1.0f);
    glVertex3f(scrW, scrH, -1.0f);

	glEnd();
	glPopMatrix();
	*/
}


// Called before render()
void update()
{
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonHome))
    {
        gdl::Platform& plat = gdl::Platform::GetPlatform();
        plat.DoProgramExit();
    }
}

int main()
{
    gdl::InitSystem(MGDL_PLATFORM,
        gdl::ScreenAspect::Screen4x3,
        init,
        update,  // std::function callbacks
        render
    );
}
