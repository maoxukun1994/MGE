#ifndef MGE_IMAGE_H
#define MGE_IMAGE_H

#include "mge_globalfunction.h"

namespace MGE_UTILITY
{


//DEPENDS ON SFML
//Image class wrapper,for now use sfml::image for image loading
class MGE_Image
{

private:

    sf::Image * m_imageInstance;

public:

    MGE_Image();

    MGE_Image(const char * filename);

    ~MGE_Image();

    bool load(const char * filename);

    int getWidth();

    int getHeight();

    void flipHorizontally();

    void flipVertically();

    //format in GL_RGBA , GL_UNSIGNED_BYTE
    const void * getPixelsPtr();

};




}//namespace MGE_UTILITY






#endif
