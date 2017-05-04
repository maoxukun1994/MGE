#ifndef MGE_GLTEXTURE2D_H
#define MGE_GLTEXTURE2D_H

#include "mge_globalfunction.h"
#include "mge_image.h"

//using namespace MGE_CORE
namespace MGE_CORE
{

//currently using soil as image loader.
class MGE_GLTexture2D
{

protected:

    GLuint m_texId;
    MGE_TextureFilterMethod m_minFilterMethod;
    MGE_TextureFilterMethod m_magFilterMethod;

public:

    MGE_GLTexture2D();

    ~MGE_GLTexture2D();

    GLuint loadImage(const char * imagefile);

    GLuint textureId();

    void setMinFilterMethod(MGE_TextureFilterMethod method);
    void setMagFilterMethod(MGE_TextureFilterMethod method);

    void bind();
};
//class MGE_GLTexture2D

}
//namespace MGE_CORE



#endif // MGE_GLTEXTURELOADER_H
