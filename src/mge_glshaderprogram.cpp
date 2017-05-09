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

    m_shaderProgram = 0;
    m_vertexShader = 0;
    m_fragmentShader = 0;
    m_computeShader = 0;
    m_isProgramLinked = false;
    //initialize
    resetShaderProgram();
}

MGE_GLShaderProgram::~MGE_GLShaderProgram()
{
    if(m_shaderProgram)
    {
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

void MGE_GLShaderProgram::setShaderConfigFile(std::string filename)
{
    m_configFileName = filename;
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

    //setup unifroms through config file
    setupUniforms();
}

void MGE_GLShaderProgram::use()
{
    glUseProgram(m_shaderProgram);

    update_frame();
}

void MGE_GLShaderProgram::update_frame()
{
    for(auto p : m_frameUpdateUniforms)
    {
        updateUniform(p);
    }
}

void MGE_GLShaderProgram::update_fixed()
{
    for(auto p : m_fixedTimeUpdateUniforms)
    {
        updateUniform(p);
    }
}

void MGE_GLShaderProgram::update_once()
{
    for(auto p : m_updateWhenUseUniforms)
    {
        updateUniform(p);
    }
}

void MGE_GLShaderProgram::resetShaderProgram()
{
    //first delete program
    if(m_shaderProgram != 0)
    {
        glDeleteProgram(m_shaderProgram);
    }

    //delete shaders
    deleteShaders();
    //clear config file name
    m_configFileName.clear();


    //clear uniforms
    m_frameUpdateUniforms.clear();
    m_fixedTimeUpdateUniforms.clear();
    m_updateWhenUseUniforms.clear();

    //initialize
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

void MGE_GLShaderProgram::setupUniforms()
{
    if(!m_isProgramLinked) return;

    //use current program
    glUseProgram(m_shaderProgram);

    //check if file has been set
    if(m_configFileName.empty()) return;

    //Retrieve the source code from filePath
    std::ifstream ConfigFile;

    // ensures ifstream objects can throw exceptions:
    ConfigFile.exceptions(std::ifstream::badbit);

    try
    {
        // Open files
        ConfigFile.open(m_configFileName);

        while(ConfigFile.getline(m_fileLineBuffer,MGE_SHADER_CONFIG_LINE_BUFFERSIZE))
        {
            std::string line(m_fileLineBuffer);

            if(!line.empty() && line.front()!='#')
            {
                std::string name;
                std::string type;
                std::string updateMethod;

                unsigned int start = 0;
                unsigned int end = 0;
                for(start=0;start<line.length() && (line.at(start)==' ' || line.at(start)=='\t');++start);
                for(end=start;end<line.length() && line.at(end)!=' ' && line.at(end)!='\t' && line.at(end)!='\n';++end);
                if(start < line.length()) name = line.substr(start,end-start);
                for(start=end+1;start<line.length() && (line.at(start)==' ' || line.at(start)=='\t');++start);
                for(end=start;end<line.length() && line.at(end)!=' ' && line.at(end)!='\t' && line.at(end)!='\n';++end);
                if(start < line.length()) type = line.substr(start,end-start);
                for(start=end+1;start<line.length() && (line.at(start)==' ' || line.at(start)=='\t');++start);
                for(end=start;end<line.length() && line.at(end)!=' ' && line.at(end)!='\t' && line.at(end)!='\n';++end);
                if(start < line.length()) updateMethod = line.substr(start,end-start);

                if(name.empty() || type.empty() || updateMethod.empty())
                {
                    //ignore line
                    continue;
                }
                else
                {
                    MGE_ShaderUniform newUniform;
                    //name
                    newUniform.name = name;
                    //type
                    switch (type.at(0))
                    {
                    case '1':
                        newUniform.type = MGE_UniformType::INT;
                        break;
                    case '2':
                        newUniform.type = MGE_UniformType::FLOAT;
                        break;
                    case '3':
                        newUniform.type = MGE_UniformType::DOUBLE;
                        break;
                    case '4':
                        newUniform.type = MGE_UniformType::VEC2;
                        break;
                    case '5':
                        newUniform.type = MGE_UniformType::VEC3;
                        break;
                    case '6':
                        newUniform.type = MGE_UniformType::VEC4;
                        break;
                    case '7':
                        newUniform.type = MGE_UniformType::MAT3;
                        break;
                    case '8':
                        newUniform.type = MGE_UniformType::MAT4;
                        break;
                    default:
                        //unknown type
                        MGE_GlobalFunction::getInstance()->mgeWarnMessage("Unknown uniform type in config file.");
                        continue;
                    }
                    //locations
                    newUniform.location = glGetUniformLocation(m_shaderProgram,name.c_str());
                    if(newUniform.location == -1)
                    {
                        MGE_GlobalFunction::getInstance()->mgeWarnMessage("Can not get uniform location in linked program.Skipping line.");
                        continue;
                    }

                    //bind host uniforms
                    if(! MGE_GLUniformManager::getInstance()->bindShaderUniform(newUniform))
                    {
                        continue;
                    }

                    //update type
                    switch (updateMethod.at(0))
                    {
                    case '0':
                        m_frameUpdateUniforms.push_back(newUniform);
                        break;
                    case '1':
                        m_fixedTimeUpdateUniforms.push_back(newUniform);
                        break;
                    case '2':
                        m_updateWhenUseUniforms.push_back(newUniform);
                        break;
                    default:
                        //unknown update ype
                        MGE_GlobalFunction::getInstance()->mgeWarnMessage("Unknown uniform update type in config file.");
                        continue;
                    }
                }
            }
        }


        // close file handlers
        ConfigFile.close();
    }
    catch(std::ifstream::failure e)
    {
        MGE_GlobalFunction::getInstance()->mgeQuitApp(ShaderConfigRead);
    }
}

void MGE_GLShaderProgram::updateUniform(MGE_ShaderUniform &target)
{
    switch(target.type)
    {
    case MGE_UniformType::INT:
        glUniform1iv(target.location,1,((GLint *)target.data));
        break;
    case MGE_UniformType::FLOAT:
        glUniform1fv(target.location,1,((GLfloat *)target.data));
        break;
    case MGE_UniformType::DOUBLE:
        glUniform1dv(target.location,1,((GLdouble *)target.data));
        break;
    case MGE_UniformType::VEC2:
        glUniform2fv(target.location,1,((GLfloat *)target.data));
        break;
    case MGE_UniformType::VEC3:
        glUniform3fv(target.location,1,((GLfloat *)target.data));
        break;
    case MGE_UniformType::VEC4:
        glUniform4fv(target.location,1,((GLfloat *)target.data));
        break;
    case MGE_UniformType::MAT3:
        glUniformMatrix3fv(target.location,1,GL_FALSE,((GLfloat *)target.data));
        break;
    case MGE_UniformType::MAT4:
        glUniformMatrix4fv(target.location,1,GL_FALSE,((GLfloat *)target.data));
        break;
    default:
        break;
    }
}




}
//namespace MGE_CORE
