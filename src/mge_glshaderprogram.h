#ifndef MGE_GLSHADERPROGRAM_H
#define MGE_GLSHADERPROGRAM_H

#include "mge_globalfunction.h"
#include "mge_gluniformmanager.h"

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

    //shader config file name
    std::string m_configFileName;

    //record error infomation
    GLchar m_infoLog[MGE_SHADER_INFO_BUFFERSIZE];

    //used in config file read
    char m_fileLineBuffer[MGE_SHADER_CONFIG_LINE_BUFFERSIZE];

    //uniforms
    std::list<MGE_ShaderUniform> m_frameUpdateUniforms;
    std::list<MGE_ShaderUniform> m_fixedTimeUpdateUniforms;
    std::list<MGE_ShaderUniform> m_updateWhenUseUniforms;

public:

    MGE_GLShaderProgram();
    ~MGE_GLShaderProgram();

    void addShaderSource(GLenum shaderType,const char * shaderSource);
    void addShaderSourceFile(GLenum shaderType,const char * shaderFileName);
    void setShaderConfigFile(std::string filename);
    void linkProgram();
    void use();

    void update_frame();
    void update_fixed();

    //will only be called in linkProgram() by itself
    void update_once();

    //this will delete all shaders and destroy this program,put the class's status into original
    //use carefully
    void resetShaderProgram();

    GLuint getProgramID();

protected:

    void deleteShaders();
    void setupUniforms();
    void updateUniform(MGE_ShaderUniform &target);
};
//class MGE_GLShaderProgram


}
//namespace MGE_CORE



#endif // MGE_GLSHADERPROGRAM_H

