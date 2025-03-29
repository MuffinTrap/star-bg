#include <mgdl.h>

#include "starbg.h"

static struct StarMesh mesh;
static struct StarMesh meshBorder;
static struct StarParameters params;

//---------------------------------------------------------------------
void init()
{
    glViewport(0, 0, gdl::GetScreenWidth(), gdl::GetScreenHeight());
    gdl::InitOrthoProjection();

    glClearColor(247.0f/255.0f, 1.0f, 174.0f/255.0f, 0.0f);

    mesh = CreateStarMesh();
    meshBorder = CreateStarMeshBorder(0.1f);
    SetStarParameters(&params, 100.0f, 0.5f, 0.6f, 0.2f, 0.2);
}
// Rendering callback. glFlush etc.. is done automatically after it
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float rot = gdl::GetElapsedSeconds() * 20.0f;

    params.r=0.7f;
    params.radius = 100.0f;
    DrawStar2D(gdl::GetScreenWidth()/2, gdl::GetScreenHeight()/2, rot, &params, &mesh);

    for (int i = 0; i < 10; i++)
    {
        params.r= 1.0f - (float)i * 0.1f;
        params.radius = 100 + 40 * i;
        rot += 0.1f;
        DrawStar2D(gdl::GetScreenWidth()/2, gdl::GetScreenHeight()/2, rot, &params, &meshBorder);
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
