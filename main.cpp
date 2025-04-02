#include <mgdl.h>

#include "starbg.h"
#include "flower.h"

static struct Mesh star;
static struct Mesh starBorder;
static struct Mesh flower;
static struct Mesh flowerCenter;

//---------------------------------------------------------------------
void init()
{
    glViewport(0, 0, gdl::GetScreenWidth(), gdl::GetScreenHeight());
    gdl::InitOrthoProjection();

    glClearColor((float)0xbd/255.0f, (float)0xb2/255.0f, (float)0xff/255.0f, 0.0f);

    star = CreateStarMesh(80.0f, 0.5f);
    starBorder = CreateStarMeshBorder(100.0f, 0.5f, 10.0f);
    float flowerCenterRatio = 0.7f;
    flower = CreateFlowerMesh(80.0f, flowerCenterRatio);
    flowerCenter = CreateFlowerCenterMesh(80.0f, flowerCenterRatio);
}
// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float rot = gdl::GetElapsedSeconds() * 20.0f;

    struct Color c;
    c.r = (float)0xfa / 255.0f;
    c.g = (float)0xd1 / 255.0f;
    c.b = (float)0xfa / 255.0f;

    float fourth = gdl::GetScreenWidth() / 4.0f;
    // Stars
    DrawMesh(fourth * 3.0f, gdl::GetScreenHeight()/2, rot, 1.0f,  &c, &star);

    float scale = 1.0f;
    DrawMesh(fourth * 2.0f, gdl::GetScreenHeight()/2, rot,  scale, &c, &starBorder);

    // flower
    DrawMesh(fourth, gdl::GetScreenHeight()/2, rot, 1.0f,  &c, &flower);

    // Flower center
    c.r = 1.0f;
    c.g = 1.0f;
    c.b = 1.0f;
    DrawMesh(fourth, gdl::GetScreenHeight()/2, rot, 1.0f,  &c, &flowerCenter);
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
