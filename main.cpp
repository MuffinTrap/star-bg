#include <mgdl.h>

#include "starbg.h"

static struct StarMesh mesh;
static struct StarMesh meshBorder;

//---------------------------------------------------------------------
void init()
{
    glViewport(0, 0, gdl::GetScreenWidth(), gdl::GetScreenHeight());
    gdl::InitOrthoProjection();

    glClearColor((float)0xbd/255.0f, (float)0xb2/255.0f, (float)0xff/255.0f, 0.0f);

    mesh = CreateStarMesh(80.0f, 0.5f);
    meshBorder = CreateStarMeshBorder(100.0f, 0.5f, 10.0f);
}
// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float rot = gdl::GetElapsedSeconds() * 20.0f;

    struct StarColor c;
    c.r = (float)0xfa / 255.0f;
    c.g = (float)0xd1 / 255.0f;
    c.b = (float)0xfa / 255.0f;
    DrawStar2D(gdl::GetScreenWidth()/2, gdl::GetScreenHeight()/2, rot, 1.0f,  &c, &mesh);

    for (int i = 0; i < 5; i++)
    {
        float scale = 1.0f + i * 0.4f;
        DrawStar2D(gdl::GetScreenWidth()/2, gdl::GetScreenHeight()/2, rot,  scale, &c, &meshBorder);
    }
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
