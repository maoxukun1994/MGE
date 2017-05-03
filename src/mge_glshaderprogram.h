#ifndef MGE_GLSHADERPROGRAM_H
#define MGE_GLSHADERPROGRAM_H

#include "mge_globalfunction.h"

//using namespace MGE_CORE
namespace MGE_CORE
{


class MGE_GLShaderProgram
{

private:

    bool m_isProgramLinked;

    //shader handle
    GLuint m_shaderProgram;
    GLuint m_vertexShader;
    GLuint m_fragmentShader;
    GLuint m_computeShader;

    //record error infomation
    GLchar m_infoLog[MGE_SHADER_INFO_BUFFERSIZE];

private:

    void deleteShaders();

public:

    MGE_GLShaderProgram();
    ~MGE_GLShaderProgram();

    void addShaderSource(GLenum shaderType,const char * shaderSource);
    void addShaderSourceFile(GLenum shaderType,const char * shaderFileName);
    void linkProgram();
    void use();

    //this will delete all shaders and destroy this program,put the class's status into original
    void resetShaderProgram();

    GLuint getProgramID();
};
//class MGE_GLShaderProgram


}
//namespace MGE_CORE



#endif // MGE_GLSHADERPROGRAM_H

