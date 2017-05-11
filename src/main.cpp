#include "mge_app.h"

int main()
{
    auto app = MGE_CORE::MGE_App::getInstance();

    if( !app->createWindow())
    {
        return MGE_CORE::MGE_ERRORCODE::WindowUnInitialized;
    }
    app->exec();

    return 0;
}

