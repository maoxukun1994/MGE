#ifndef MGE_GLTEXTURE2D_H
#define MGE_GLTEXTURE2D_H

#include "mge_globalfunction.h"

//using namespace MGE_CORE
namespace MGE_CORE
{


enum TextureFilterType
{
    MIN_FILTER = 0,
    MAG_FILTER
};

enum TextureFilterMethod
{
    NEAREST = 0,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_NEAREST,
    LINEAR_MIPMAP_LINEAR
};

//currently using soil as image loader.
class MGE_GLTexture2D
{

protected:

    GLuint m_texId;
    TextureFilterMethod m_minFilterMethod;
    TextureFilterMethod m_magFilterMethod;

public:

    MGE_GLTexture2D();

    GLuint loadImage(const char * imagefile);

    GLuint textureId();

    void setMinFilterMethod(TextureFilterMethod method);
    void setMagFilterMethod(TextureFilterMethod method);

    void bind();
};
//class MGE_GLTexture2D

}
//namespace MGE_CORE



#endif // MGE_GLTEXTURELOADER_H
