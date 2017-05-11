#include "mge_app.h"

namespace MGE_CORE
{

MGE_App::MGE_App()
{
    m_mainWindow = nullptr;
    m_isRunning = false;

#ifdef __gnu_linux__
    //only on linux
    XInitThreads();
#endif
}

MGE_App::~MGE_App()
{
    if(m_mainWindow != nullptr)
    {
        m_mainWindow->close();
        delete m_mainWindow;
    }

    //temp
    delete batch;
    delete tex;
    delete cam;
}

bool MGE_App::createWindow(sf::ContextSettings glContextSettings)
{
    if(m_mainWindow != nullptr) return false;

    //detect and list avaliable video modes
    auto modes = sf::VideoMode::getFullscreenModes();
    for(auto p : modes)
    {
        std::cout<<"Video: "<<p.width<<"x"<<p.height<<" @"<<p.bitsPerPixel<<"bit per pixel"<<std::endl;
    }

    // Create the main window
    m_mainWindow = new sf::Window(modes.front(),"MGEAPP",sf::Style::Default,glContextSettings);

    //load opengl functions
    if(!gladLoadGL() && m_mainWindow!=nullptr)
    {
        MGE_CORE::MGE_GlobalFunction::getInstance()->mgeQuitApp(MGE_CORE::MGE_ERRORCODE::GLLoadError);
    }
    else
    {
        MGE_CORE::MGE_GlobalFunction::getInstance()->setGLInitialized(true);
    }

    // Make it the active window for OpenGL calls
    m_mainWindow->setActive(true);
    //enable vsync
    m_mainWindow->setVerticalSyncEnabled(true);

    //gl parameters
    glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    //temp init

    glClearColor(0.2f,0.5f,0.8f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //for measuring fps
    frameCount =1;
    timer.restart();

    //initialize camera
    cam = new MGE_CORE::MGE_GLFpsCamera();
    cam->setPosition(glm::vec3(0.0f,0.0f,2.0f));
    cam->setForward(glm::vec3(0.0f,1.0f,-1.0f));
    cam->setUp(glm::vec3(0.0f,0.0f,1.0f));
    cam->perspective(45.0f,1.799f,0.01f,1000.0f);
    cam->update();
    MGE_CORE::MGE_GLUniformManager::getInstance()->registerHostUniform("vMatrix",MGE_CORE::MGE_UniformType::MAT4,cam->getVMatrixPtr());
    MGE_CORE::MGE_GLUniformManager::getInstance()->registerHostUniform("pMatrix",MGE_CORE::MGE_UniformType::MAT4,cam->getPMatrixPtr());
    mMatrix = matTranslate(mMatrix,glm::vec3(0.0f,2.0f,0.0f));
    MGE_CORE::MGE_GLUniformManager::getInstance()->registerHostUniform("mMatrix",MGE_CORE::MGE_UniformType::MAT4,glm::value_ptr(mMatrix));


    //initialize resources
    GLfloat verts[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    GLfloat tuvs[] = {
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,0.0f, 1.0f,0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,0.0f, 1.0f,0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,0.0f, 0.0f,1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,0.0f, 0.0f,1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f,0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f, 0.0f,0.0f, 1.0f
    };
    batch = new MGE_CORE::MGE_GLBatchBase();
    batch->enableBatchVertexAttrib(MGE_CORE::MGE_VATTRIB_POSITION);
    batch->enableBatchVertexAttrib(MGE_CORE::MGE_VATTRIB_TUV_0);
    batch->setVertexPositionData(verts,36);
    batch->setVertexTUVData(tuvs,36,0);

    tex = new MGE_CORE::MGE_GLTexture2D();
    tex->setMinFilterMethod(MGE_CORE::MGE_TextureFilterMethod::LINEAR_MIPMAP_LINEAR);
    tex->setMagFilterMethod(MGE_CORE::MGE_TextureFilterMethod::LINEAR);
    tex->loadImage("/home/maomao/Develop/Projects/Builds/mge-build/test.png");
    tex->bind();

    sampler = 0;
    MGE_CORE::MGE_GLUniformManager::getInstance()->registerHostUniform("tex",MGE_CORE::MGE_UniformType::INT,&sampler);

    program = new MGE_CORE::MGE_GLShaderProgram();
    program->addShaderSourceFile(GL_VERTEX_SHADER,"basic.vert");
    program->addShaderSourceFile(GL_FRAGMENT_SHADER,"basic.frag");
    program->setShaderConfigFile("basic.config");
    program->linkProgram();
    program->use();
    MGE_CORE::MGE_GLProgramManager::getInstance()->addProgram(program);

    return true;
}


int MGE_App::exec()
{
    if(m_mainWindow == nullptr)
    {
        return MGE_ERRORCODE::WindowUnInitialized;
    }

    // Start the game loop
    m_isRunning = true;

    //deactivate the window to move it to another thread
    m_mainWindow->setActive(false);

    //launch the rendering thread
    std::thread renderThread(&MGE_App::renderExec,this);

    // Process events

    sf::Event event;
    while(m_isRunning)
    {
        if(m_mainWindow->waitEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                m_isRunning = false;

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                m_isRunning = false;

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::U))
                batch->disableBatchVertexAttrib(MGE_CORE::MGE_VATTRIB_TUV_0,false);

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::T))
                batch->enableBatchVertexAttrib(MGE_CORE::MGE_VATTRIB_TUV_0);
        }
    }

    renderThread.join();

    //set active in main thread for correct resource cleaning
    m_mainWindow->setActive(true);

    return MGE_ERRORCODE::NoError;
}


void MGE_App::renderExec()
{
    while(m_isRunning)
    {

        // Clear the color and depth buffers
        glClearColor(0.2f,0.5f,0.8f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        mMatrix = glm::rotate(mMatrix,0.02f,glm::vec3(0.0f,0.0f,1.0f));
        tex->bind();
        MGE_CORE::MGE_GLProgramManager::getInstance()->getProgram(0)->use();
        cam->update();
        batch->draw();

        //Finally, display the rendered frame on screen
        m_mainWindow->display();

        ++frameCount;
        if(frameCount > 300)
        {
            std::cout<<"Average fps in last 600 frames:"<<(float)frameCount/timer.getElapsedTime().asSeconds()<<std::endl;
            frameCount = 0;
            timer.restart();
        }
    }

    //deactivated it to be cleaned by main thread
    m_mainWindow->setActive(false);
}

MGE_App * MGE_App::getInstance()
{
    static MGE_App m_instance;
    return &m_instance;
}

}
//namespace MGE_CORE
