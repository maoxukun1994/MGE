#ifndef MGE_APP_H
#define MGE_APP_H

#include "mge_globalfunction.h"

//temp
#include "mge_glcamera.h"
#include "mge_glbatchbase.h"
#include "mge_gltexture2d.h"
#include "mge_glshaderprogram.h"
#include "mge_glprogrammanager.h"
#include "mge_gluniformmanager.h"

namespace MGE_CORE
{

class MGE_App
{

private:

    sf::Window * m_mainWindow;

    volatile bool m_isRunning;

private:

    MGE_App();

protected:

    void renderExec();

public:

    ~MGE_App();

    //must be called in main thread
    bool createWindow(sf::ContextSettings glContextSettings = sf::ContextSettings(24,8,4,4,5,sf::ContextSettings::Core));

    int exec();

    static MGE_App * getInstance();


public://temp,move to scenemanager later

    //for measuring fps
    int frameCount;
    sf::Clock timer;
    MGE_GLFpsCamera * cam;
    Mat4 mMatrix;
    MGE_GLBatchBase * batch;
    MGE_GLTexture2D * tex;
    int sampler;
    MGE_GLShaderProgram * program;
};

}
//namaespace MGE_CORE

#endif //MGE_APP_H
