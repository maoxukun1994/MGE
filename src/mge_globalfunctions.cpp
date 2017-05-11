#include "mge_globalfunction.h"

namespace MGE_CORE
{

MGE_GlobalFunction::MGE_GlobalFunction()
{
    m_GLFunctionsInitialized = false;
}

MGE_GlobalFunction * MGE_GlobalFunction::getInstance()
{
    static MGE_GlobalFunction m_func;
    return &m_func;
}

void MGE_GlobalFunction::setGLInitialized(bool flag)
{
    m_GLFunctionsInitialized = flag;
}

bool MGE_GlobalFunction::isGLInitialized()
{
    return m_GLFunctionsInitialized;
}

//quit with errorCode
void MGE_GlobalFunction::mgeQuitApp(MGE_CORE::MGE_ERRORCODE errorCode)
{
    switch(errorCode)
    {
    case MGE_CORE::MGE_ERRORCODE::NoError:
        exit(0);
        break;
    case MGE_CORE::MGE_ERRORCODE::GLLoadError:
        std::cout<<"Application quit with error : GLLoadError"<<std::endl;
        break;
    case MGE_CORE::MGE_ERRORCODE::GLUnInitialized:
        std::cout<<"Application quit with error : GLUnInitialized"<<std::endl;
        break;
    case MGE_CORE::MGE_ERRORCODE::WindowUnInitialized:
        std::cout<<"Application quit with error : WindowUnInitialized"<<std::endl;
        break;
    case MGE_CORE::MGE_ERRORCODE::ShaderCompileVertex:
        std::cout<<"Application quit with error : ShaderCompileVertex"<<std::endl;
        break;
    case MGE_CORE::MGE_ERRORCODE::ShaderCompileFragment:
        std::cout<<"Application quit with error : ShaderCompileFragment"<<std::endl;
        break;
    case MGE_CORE::MGE_ERRORCODE::ShaderCompileCompute:
        std::cout<<"Application quit with error : ShaderCompileCompute"<<std::endl;
        break;
    case MGE_CORE::MGE_ERRORCODE::ShaderLink:
        std::cout<<"Application quit with error : ShaderLink"<<std::endl;
        break;
    case MGE_CORE::MGE_ERRORCODE::ShaderFileRead:
        std::cout<<"Application quit with error : ShaderFileRead"<<std::endl;
        break;
    case MGE_CORE::MGE_ERRORCODE::ShaderConfigRead:
        std::cout<<"Application quit with error : ShaderConfigRead"<<std::endl;
        break;
    case MGE_CORE::MGE_ERRORCODE::TextureLoadFileRead:
        std::cout<<"Application quit with error : TextureLoadFileRead"<<std::endl;
        break;
    default :
        std::cout<<"Unknown error occured."<<std::endl;
        break;
    }
    exit(1);
}
//mgeQuitApp

//print message to stdout
void MGE_GlobalFunction::mgeErrorMessage(const char * msg)
{
    std::cout<<"ERROR: "<<msg<<std::endl;
}

//print warning message
void MGE_GlobalFunction::mgeWarnMessage(const char * msg)
{
    std::cout<<"WARN: "<<msg<<std::endl;
}

//print info message
void MGE_GlobalFunction::mgeInfoMessage(const char * msg)
{
    std::cout<<"INFO: "<<msg<<std::endl;
}


}
//namespace MGE_CORE
