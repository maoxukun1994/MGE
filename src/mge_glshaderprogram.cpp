#include "mge_glshaderprogram.h"

namespace MGE_CORE
{

MGE_GLShaderProgram::MGE_GLShaderProgram()
{
    //class won't construct when opengl functions is uninitialized
    if(!MGE_GlobalFunction::getInstance()->isGLInitialized())
    {
        MGE_GlobalFunction::getInstance()->mgeQuitApp(MGE_ERRORCODE::GLUnInitialized);
    }

    //initialize
    m_isProgramLinked = false;
    m_shaderProgram = 0;
    m_vertexShader = 0;
    m_fragmentShader = 0;
    m_computeShader = 0;

    resetShaderProgram();
}

MGE_GLShaderProgram::~MGE_GLShaderProgram()
{
    if(m_shaderProgram)
    {
        glUseProgram(0);
        glDeleteProgram(m_shaderProgram);
    }
    deleteShaders();
}

void MGE_GLShaderProgram::addShaderSource(GLenum shaderType,const char * shaderSource)
{
    GLint success = 0;

    //make sure program has been created and has not been linked
    if(m_isProgramLinked || !m_shaderProgram) return;

    switch(shaderType)
    {
    case GL_VERTEX_SHADER:
        //check if created and delete
        if(m_vertexShader)
        {
            glDeleteShader(m_vertexShader);
        }
        //create
        m_vertexShader = glCreateShader(shaderType);
        glShaderSource(m_vertexShader,1,&shaderSource,NULL);
        //compile
        glCompileShader(m_vertexShader);
        //check
        glGetShaderiv(m_vertexShader,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            m_vertexShader = 0;
            glGetShaderInfoLog(m_vertexShader,MGE_SHADER_INFO_BUFFERSIZE,NULL,m_infoLog);
            MGE_GlobalFunction::getInstance()->mgeInfoMessage((char *)m_infoLog);
            MGE_GlobalFunction::getInstance()->mgeQuitApp(ShaderCompileVertex);
            return;
        }
        break;
    case GL_FRAGMENT_SHADER:
        //check if created and delete
        if(m_fragmentShader)
        {
            glDeleteShader(m_fragmentShader);
        }
        //create
        m_fragmentShader = glCreateShader(shaderType);
        glShaderSource(m_fragmentShader,1,&shaderSource,NULL);
        //compile
        glCompileShader(m_fragmentShader);
        //check
        glGetShaderiv(m_fragmentShader,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            glGetShaderInfoLog(m_fragmentShader,MGE_SHADER_INFO_BUFFERSIZE,NULL,m_infoLog);
            MGE_GlobalFunction::getInstance()->mgeInfoMessage(m_infoLog);
            MGE_GlobalFunction::getInstance()->mgeQuitApp(ShaderCompileFragment);
        }
        break;
    case GL_COMPUTE_SHADER:
        //***if to check the compute shader extension?***//

        //check if created and delete
        if(m_computeShader)
        {
            glDeleteShader(m_computeShader);
        }
        //create
        m_computeShader = glCreateShader(shaderType);
        glShaderSource(m_computeShader,1,&shaderSource,NULL);
        //compile
        glCompileShader(m_computeShader);
        //check
        glGetShaderiv(m_computeShader,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            glGetShaderInfoLog(m_computeShader,MGE_SHADER_INFO_BUFFERSIZE,NULL,m_infoLog);
            MGE_GlobalFunction::getInstance()->mgeInfoMessage(m_infoLog);
            MGE_GlobalFunction::getInstance()->mgeQuitApp(ShaderCompileCompute);
        }
        break;
    default:
        return;
        break;
    }

}

void MGE_GLShaderProgram::addShaderSourceFile(GLenum shaderType,const char * shaderFileName)
{
    //Retrieve the source code from filePath
    std::string Code;
    std::ifstream ShaderFile;
    std::stringstream ShaderStream;

    // ensures ifstream objects can throw exceptions:
    ShaderFile.exceptions(std::ifstream::badbit);

    try
    {
        // Open files
        ShaderFile.open(shaderFileName);
        // Read file's buffer contents into streams
        ShaderStream << ShaderFile.rdbuf();
        // close file handlers
        ShaderFile.close();
        // Convert stream into GLchar array
        Code = ShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        MGE_GlobalFunction::getInstance()->mgeQuitApp(ShaderFileRead);
    }

    const GLchar * ShaderCode = Code.c_str();

    addShaderSource(shaderType,ShaderCode);
}

void MGE_GLShaderProgram::linkProgram()
{
    GLint success = 0;
    GLchar m_infoLog[MGE_SHADER_INFO_BUFFERSIZE];


    //***attach shaders to program***

    //attach vertex shader to program
    glAttachShader(m_shaderProgram,m_vertexShader);
    //attach fragment shader to program
    glAttachShader(m_shaderProgram,m_fragmentShader);
    //attach compute shader to program
    glAttachShader(m_shaderProgram,m_computeShader);

    //TO BE FILLED

    //***attach shaders to program***

    glLinkProgram(m_shaderProgram);
    glGetProgramiv(m_shaderProgram,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(m_shaderProgram,MGE_SHADER_INFO_BUFFERSIZE,NULL,m_infoLog);
        MGE_GlobalFunction::getInstance()->mgeInfoMessage(m_infoLog);
        MGE_GlobalFunction::getInstance()->mgeQuitApp(ShaderLink);
    }

    //***delete shaders***
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
    glDeleteShader(m_computeShader);
    //TO BE FILLED
    //***delete shaders***

    //set linked flag to true
    m_isProgramLinked = true;
}

void MGE_GLShaderProgram::use()
{
    glUseProgram(m_shaderProgram);
}

void MGE_GLShaderProgram::resetShaderProgram()
{
    //first delete program
    if(m_shaderProgram != 0)
    {
        glDeleteProgram(m_shaderProgram);
    }

    //shader program
    m_isProgramLinked = false;
    m_shaderProgram = 0;

    //start init..........
    m_shaderProgram = glCreateProgram();
}

GLuint MGE_GLShaderProgram::getProgramID()
{
    return m_shaderProgram;
}

void MGE_GLShaderProgram::deleteShaders()
{
    //individal shaders
    //***delete shaders***
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
    glDeleteShader(m_computeShader);
    //TO BE FILLED
    //***delete shaders***
    m_vertexShader = 0;
    m_fragmentShader = 0;
    m_computeShader = 0;
    //TO BE FILLED
}


}
//namespace MGE_CORE
