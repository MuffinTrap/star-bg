#include <mgdl.h>

#include "Star.h"
#include "Flower.h"
#include "Ribbon.h"
#include "BezierCurve.h"
#include "Cloud.h"
#include "SkyCylinder.h"


static struct ProceduralMesh star;
static struct ProceduralMesh starBorder;
static struct ProceduralMesh flower;
static struct ProceduralMesh flowerCenter;
static struct ProceduralMesh clouds[10];
static struct Ribbon ribbon;
static struct SkyCylinder cylinder;

static int ribbonPoints;
static int ribbonPointsPerSegments;
static int ribbonDrawStart = 0;
static int ribbonDrawEnd = 100;

static int cloudsAmount = 10;

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
    int c = 0;
    int r = 0;
    cylinder.radius = 200.0f; // This needs to be huge compared to clouds!
    float tau = M_PI * 2.0f;
    float angleStep = tau/(float)cloudsAmount;
    for (int i = 0; i < cloudsAmount; i++)
    {
        clouds[i] = CreateCloudMesh(12.0f, 4, 0.0f);
        float y = -12.5 + r * 5.5f;
        float angle = i * angleStep;
        ProjectMeshToCylinder(&clouds[i], &cylinder, y, angle);

        //SetPositionProceduralMesh(&clouds[i], -2.5f + c * 4.5, -2.5 + r * 1.5f, -2.0f);
        SetPositionProceduralMesh(&clouds[i], 0.0f, 0.0f, -2.0f);
        SetScaleProceduralMesh(&clouds[i], 0.05f);

        c++;
        if (c >= 2) { c= 0; r++;}
    }

    printf("Rotation test\n");



    /* RIBBON
     */

    // Positions

    SetPositionProceduralMesh(&flower, 0.8f, 0.0f, -1.0f);
    SetScaleProceduralMesh(&flower, 0.1f);

    SetPositionProceduralMesh(&star, -0.8f, -0.8f, -1.0f);
    SetScaleProceduralMesh(&star, 0.1f);

    SetPositionProceduralMesh(&starBorder, -0.8f, 0.8f, -1.0f);
    SetScaleProceduralMesh(&starBorder, 0.1f);


}


void CreateRibbon()
{
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
}



// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    gdl::cross_glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    gdl::InitPerspectiveProjection(90.0f, 0.1f, 100.0f);
    vec3 cp = V3f_Create(0.0f, 0.0f, 0.0f);
    vec3 ct = V3f_Create(0.0f, 0.0f, -1.0f);
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

    // Ribbon
    /*
    SetPositionProceduralMesh(ribbon.mesh, 0.0f, 0.0f, -1.0f);
    SetRotationsProceduralMesh(ribbon.mesh, rot*2.0f, rot, rot*0.5f);
    SetScaleProceduralMesh(ribbon.mesh, 0.1f);
    // DrawProceduralMesh(ribbon.mesh);
    DrawRibbonPartially(&ribbon, ribbonDrawStart, ribbonDrawEnd);
    */


    // Cloud
    for (int i = 0; i < cloudsAmount; i++)
    {
        SetRotationsProceduralMesh(&clouds[i], 0.0f, rot, 0.0f);
        DrawProceduralMesh(&clouds[i]);
    }


    gdl::InitOrthoProjection();
    debugFont->Printf(gdl::Colors::White, 10, scrH, 16, "Ribbon Draw %d -> %d", ribbonDrawStart, ribbonDrawEnd);
    int rotDeg = ((int)rot) % 360;
    debugFont->Printf(gdl::Colors::White, 10, scrH-16, 16, "Cylinder rotation %d", rotDeg);
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
