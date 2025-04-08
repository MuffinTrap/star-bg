#include <mgdl.h>

#include "Star.h"
#include "Flower.h"
#include "Ribbon.h"
#include "BezierCurve.h"


static struct ProceduralMesh star;
static struct ProceduralMesh starBorder;
static struct ProceduralMesh flower;
static struct ProceduralMesh flowerCenter;
static struct Ribbon ribbon;

static int ribbonPoints;
static int ribbonPointsPerSegments;
static int ribbonDrawStart = 0;
static int ribbonDrawEnd = 100;

static gdl::Font* debugFont;

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

    debugFont = gdl::GetDebugFont();

    vec3 color = V3f_Create(0.6f, 0.5f, 0.9f);

    float flowerCenterRatio = 0.7f;
    flower = CreateFlowerMesh(10.0f, flowerCenterRatio);
    star = CreateStarMesh(10.0f, 0.5f);
    starBorder = CreateStarMeshBorder(10.0f, 0.5f, 1.0f);
    flowerCenter = CreateFlowerCenterMesh(80.0f, flowerCenterRatio);

    ribbonPoints = 7;
    ribbonPointsPerSegments = 14;
    struct BezierCurve curve;
    InitBezierCurve(&curve, ribbonPoints);

    PushVertexBezierCurveXYZ(&curve, -9.0f , -9.0f,  -9.0f);
    PushVertexBezierCurveXYZ(&curve, -4.0f ,   -4.0f  , -3.0f);
    PushVertexBezierCurveXYZ(&curve, 14.0f  ,  5.0f  , -3.0f);
    PushVertexBezierCurveXYZ(&curve, 0.0f  ,  10.0f  , 8.0f);
    PushVertexBezierCurveXYZ(&curve, -2.0f ,   5.0f  , -8.0f);
    PushVertexBezierCurveXYZ(&curve, 16.0f  ,  -5.0f  , 0.0f);
    PushVertexBezierCurveXYZ(&curve, -8.0f , -10.0f,  10.0f);

    float* splinePoints = NULL;
    ConvertBezierCurveToFloatArray(&curve, &splinePoints);


    ribbon = CreateRibbonMesh(splinePoints, curve.vertexAmount, ribbonPointsPerSegments, 5, 3.0f, 0xfa, 0x0d, 0x54);

    FreeBezierCurve(&curve);
    free(splinePoints);

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
    gdl::InitPerspectiveProjection(90.0f, 0.1f, 100.0f);
    vec3 cp = V3f_Create(0.0f, 0.0f, 1.0f);
    vec3 ct = V3f_Create(0.0f, 0.0f, 0.0f);
    vec3 cup = V3f_Create(0.0f, 1.0f, 0.0f);
    gdl::InitCamera(cp, ct, cup);

    float rot = gdl::GetElapsedSeconds() * 40.0f;

    /*
    vec3 color;
    V3f_HexToColor(0xfa, 0xd1, 0xfa, color);
    */

    int scrW = gdl::GetScreenWidth();
    int scrH = gdl::GetScreenHeight();

    //DrawProceduralMesh(&flower);

    float fourth = 0.25f;
    // Stars
    //DrawProceduralMesh(&star);
    //DrawProceduralMesh(&starBorder);
    /*

    float scale = 1.0f;
    DrawMesh(fourth * 2.0f, gdl::GetScreenHeight()/2, rot,  scale, &c, &starBorder);


    // Flower center
    c.r = 1.0f;
    c.g = 1.0f;
    c.b = 1.0f;
    DrawMesh(fourth, gdl::GetScreenHeight()/2, rot, 1.0f,  &c, &flowerCenter);
    */

    SetPositionProceduralMesh(ribbon.mesh, 0.0f, 0.0f, -1.0f);
    SetRotationsProceduralMesh(ribbon.mesh, rot*2.0f, rot, rot*0.5f);
    SetScaleProceduralMesh(ribbon.mesh, 0.1f);
    // DrawProceduralMesh(ribbon.mesh);
    DrawRibbonPartially(&ribbon, ribbonDrawStart, ribbonDrawEnd);


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
    gdl::InitOrthoProjection();
    debugFont->Printf(gdl::Colors::White, 10, scrH, 16, "Ribbon Draw %d -> %d", ribbonDrawStart, ribbonDrawEnd);

}


// Called before render()
void update()
{
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonHome))
    {
        gdl::Platform& plat = gdl::Platform::GetPlatform();
        plat.DoProgramExit();
    }
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonUp))
    {
        ribbonDrawStart += 1;
    }
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonDown))
    {
        ribbonDrawStart -= 1;
    }
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonLeft))
    {
        ribbonDrawEnd -= 1;
    }
    if (gdl::GetController(0).ButtonPress(gdl::WiiButtons::ButtonRight))
    {
        ribbonDrawEnd += 1;
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
