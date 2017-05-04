#include "mge_gltexture2d.h"

namespace MGE_CORE
{

MGE_GLTexture2D::MGE_GLTexture2D()
{
    //class won't construct when opengl functions is uninitialized
    if(!MGE_GlobalFunction::getInstance()->isGLInitialized())
    {
        MGE_GlobalFunction::getInstance()->mgeQuitApp(MGE_ERRORCODE::GLUnInitialized);
    }

    m_texId = 0;
    m_minFilterMethod = TextureFilterMethod::NEAREST;
    m_magFilterMethod = TextureFilterMethod::NEAREST;
}

MGE_GLTexture2D::~MGE_GLTexture2D()
{
    //delete the underlying opengl texture and free resource
    if(m_texId)
    {
        glDeleteTextures(1,&m_texId);
    }
}

GLuint MGE_GLTexture2D::loadImage(const char * imagefile)
{
    //check if texture is already created
    if(m_texId)
    {
        //unbind current texture in case current texture is this one
        glBindTexture(GL_TEXTURE_2D,0);
        glDeleteTextures(1,&m_texId);
        m_texId = 0;
    }
    //generate opengl texture
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D,texture);

    //set texture filter method
    switch(m_minFilterMethod)
    {
    case TextureFilterMethod::NEAREST:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        break;
    case TextureFilterMethod::LINEAR:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        break;
    case TextureFilterMethod::LINEAR_MIPMAP_NEAREST:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
        break;
    case TextureFilterMethod::LINEAR_MIPMAP_LINEAR:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        break;
    default:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        break;
    }

    switch(m_magFilterMethod)
    {
    case TextureFilterMethod::NEAREST:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        break;
    case TextureFilterMethod::LINEAR:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        break;
    default:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        break;
    }

    //try load image
    MGE_UTILITY::MGE_Image image(imagefile);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image.getWidth(),image.getHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);

    //set id
    m_texId = texture;
    //return id
    return m_texId;
}

GLuint MGE_GLTexture2D::textureId()
{
    return m_texId;
}

void MGE_GLTexture2D::setMinFilterMethod(TextureFilterMethod method)
{
    m_minFilterMethod = method;
}

void MGE_GLTexture2D::setMagFilterMethod(TextureFilterMethod method)
{
    m_magFilterMethod = method;
}

void MGE_GLTexture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D,m_texId);
}

}
//namespace MGE_CORE
