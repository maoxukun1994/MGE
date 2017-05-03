#include "mge_global.h"
#include "mge_globalfunction.h"
#include "mge_glshaderprogram.h"
#include "mge_glbatchbase.h"
#include "mge_gltexture2d.h"
#include "mge_glprogrammanager.h"

int main()
{
    //detect and list avaliable video modes
    auto modes = sf::VideoMode::getFullscreenModes();
    for(auto p : modes)
    {
        std::cout<<"Video: "<<p.width<<"x"<<p.height<<" @"<<p.bitsPerPixel<<"bit per pixel"<<std::endl;
    }

    // Create the main window
    sf::Window window(modes.front(),"MGE",sf::Style::Default,sf::ContextSettings(24,8,4,4,5,sf::ContextSettings::Core));

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
    int frameCount =0;
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

    const char * vs = "#version 450 core\nlayout (location = 0) in vec3 position;\nlayout (location = 2) in vec4 color;\nout vec4 vcolor;\nvoid main()\n{\ngl_Position = vec4(position.x,position.y,position.z,1.0);\nvcolor=color;\n}\n";
    const char * fs = "#version 450 core\nin vec4 vcolor;\nout vec4 fragColor;\nvoid main()\n{\nfragColor = vcolor;\n}\n";
    const char * vst = "#version 450 core\nlayout (location = 0) in vec3 position;\nlayout (location = 3) in vec2 diffuseTextureUV;\nout vec2 tuv;\nuniform float rad;\nvoid main()\n{\nvec3 npos;\nnpos.x=position.x*cos(rad)-position.y*sin(rad);\nnpos.y=position.x*sin(rad)+position.y*cos(rad);\nnpos.z=0.0f;\ngl_Position = vec4(npos,1.0f);\ntuv = diffuseTextureUV;\n}\n" ;
    const char * fst = "#version 450 core\nuniform sampler2D tex;\nin vec2 tuv;\nout vec4 color;\nvoid main()\n{\ncolor = vec4(texture(tex,tuv));\n}\n";



    auto tex = new MGE_CORE::MGE_GLTexture2D();
    tex->setMinFilterMethod(MGE_CORE::TextureFilterMethod::LINEAR_MIPMAP_LINEAR);
    tex->setMagFilterMethod(MGE_CORE::TextureFilterMethod::LINEAR);
    tex->loadImage("/home/maomao/Develop/Projects/Builds/mge-build/test.bmp");
    tex->bind();

    auto programMgr = MGE_CORE::MGE_GLProgramManager::getInstance();
    auto program = new MGE_CORE::MGE_GLShaderProgram();
    program->addShaderSource(GL_VERTEX_SHADER,vst);
    program->addShaderSource(GL_FRAGMENT_SHADER,fst);
    program->linkProgram();
    program->use();
    programMgr->addProgram(program);
    GLuint uniformlocation = glGetUniformLocation(program->getProgramID(),"tex");
    GLuint uniformlocation_1 = glGetUniformLocation(program->getProgramID(),"rad");
    float rad = 0.0f;
    glUniform1i(uniformlocation,0);

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


        MGE_CORE::MGE_GLProgramManager::getInstance()->getProgram(0)->use();
        glUniform1f(uniformlocation_1,rad);
        tex->bind();
        batch->draw();
        rad += 0.02f;

        // Finally, display the rendered frame on screen
        window.display();

        ++frameCount;
        if(frameCount > 300)
        {
            std::cout<<"Average fps in last 600 frames:"<<(float)frameCount/timer.getElapsedTime().asSeconds()<<std::endl;
            frameCount = 0;
            timer.restart();
        }
    }

    MGE_CORE::MGE_GlobalFunction::getInstance()->mgeQuitApp(MGE_CORE::MGE_ERRORCODE::NoError);
    return 0;
}
