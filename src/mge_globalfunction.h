#ifndef MGE_GLOBALFUNCTION_H
#define MGE_GLOBALFUNCTION_H

#include "mge_global.h"

namespace MGE_CORE
{

class MGE_GlobalFunction
{

private:

    bool m_GLFunctionsInitialized;

private:

    MGE_GlobalFunction();

public:

    static MGE_GlobalFunction * getInstance();

    void setGLInitialized(bool flag);

    bool isGLInitialized();

    //quit with errorCode
    void mgeQuitApp(MGE_CORE::MGE_ERRORCODE errorCode);

    //print message to stdout
    void mgeErrorMessage(const char * msg);

    //print warning message
    void mgeWarnMessage(const char * msg);

    //print info message
    void mgeInfoMessage(const char * msg);

};



}

//namespace MGE_CORE


#endif
