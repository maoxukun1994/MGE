#include "mge_global.h"
#include "mge_globalfunction.h"
#include "mge_glshaderprogram.h"
#include "mge_glbatchbase.h"
#include "mge_gltexture2d.h"
#include "mge_glprogrammanager.h"
#include "mge_gluniformmanager.h"

int main()
{
    //detect and list avaliable video modes
    auto modes = sf::VideoMode::getFullscreenModes();
    for(auto p : modes)
    {
        std::cout<<"Video: "<<p.width<<"x"<<p.height<<" @"<<p.bitsPerPixel<<"bit per pixel"<<std::endl;
    }

    // Create the main window
    sf::Window window(modes.front(),"MGE",sf::Style::Fullscreen,sf::ContextSettings(24,8,4,4,5,sf::ContextSettings::Core));

    //load opengl functions
    if(!gladLoadGL())
    {
        MGE_CORE::MGE_GlobalFunction::getInstance()->mgeQuitApp(MGE_CORE::MGE_ERRORCODE::GLLoadError);
    }
    else
    {
        MGE_CORE::MGE_GlobalFunction::getInstance()->setGLInitialized(true);
    }

    //for measuring fps
    int frameCount =1;
    sf::Clock timer;
    timer.restart();

    // Make it the active window for OpenGL calls
    window.setActive();
    window.setVerticalSyncEnabled(true);

    //initialize resources
    GLfloat verts[] = {-1.0f,-1.0f,0.0f,1.0f,-1.0f,0.0f,0.0f,1.0f,0.0f};
    GLfloat colors[] = {1.0f,0.0f,0.0f,1.0f,0.0f,1.0f,0.0f,1.0f,0.0f,0.0f,1.0f,1.0f};
    GLfloat tuvs[] = {0.0f,0.0f,1.0,0.0,0.5f,1.0f};
    auto batch = new MGE_CORE::MGE_GLBatchBase();
    batch->enableBatchVertexAttrib(MGE_CORE::MGE_VATTRIB_POSITION);
    batch->enableBatchVertexAttrib(MGE_CORE::MGE_VATTRIB_COLOR);
    batch->enableBatchVertexAttrib(MGE_CORE::MGE_VATTRIB_TUV_0);
    batch->setVertexPositionData(verts,3);
    batch->setVertexColorData(colors,3);
    batch->setVertexTUVData(tuvs,3,0);

    auto tex = new MGE_CORE::MGE_GLTexture2D();
    tex->setMinFilterMethod(MGE_CORE::MGE_TextureFilterMethod::LINEAR_MIPMAP_LINEAR);
    tex->setMagFilterMethod(MGE_CORE::MGE_TextureFilterMethod::LINEAR);
    tex->loadImage("/home/maomao/Develop/Projects/Builds/mge-build/test.png");
    tex->bind();

    int sampler = 0;
    float rad = 0.0f;
    MGE_CORE::MGE_GLUniformManager::getInstance()->registerHostUniform("rad",MGE_CORE::MGE_UniformType::FLOAT,&rad);
    MGE_CORE::MGE_GLUniformManager::getInstance()->registerHostUniform("tex",MGE_CORE::MGE_UniformType::INT,&sampler);

    auto program = new MGE_CORE::MGE_GLShaderProgram();
    program->addShaderSourceFile(GL_VERTEX_SHADER,"test.vert");
    program->addShaderSourceFile(GL_FRAGMENT_SHADER,"test.frag");
    program->setShaderConfigFile("test.config");
    program->linkProgram();
    program->use();
    MGE_CORE::MGE_GLProgramManager::getInstance()->addProgram(program);


    //gl parameters
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::C))
                batch->disableBatchVertexAttrib(MGE_CORE::MGE_VATTRIB_COLOR,false);

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::V))
                batch->enableBatchVertexAttrib(MGE_CORE::MGE_VATTRIB_COLOR);
        }

        // Clear the color and depth buffers
        glClearColor(0.2f,0.5f,0.8f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        tex->bind();
        MGE_CORE::MGE_GLProgramManager::getInstance()->getProgram(0)->use();
        batch->draw();
        rad += 0.02f;

        // Finally, display the rendered frame on screen
        window.display();

        //fuck

        ++frameCount;
        if(frameCount > 300)
        {
            std::cout<<"Average fps in last 600 frames:"<<(float)frameCount/timer.getElapsedTime().asSeconds()<<std::endl;
            frameCount = 0;
            timer.restart();
        }
    }

    window.setActive(false);

    delete batch;
    delete tex;

    //MGE_CORE::MGE_GlobalFunction::getInstance()->mgeQuitApp(MGE_CORE::MGE_ERRORCODE::NoError);
    return 0;
}
