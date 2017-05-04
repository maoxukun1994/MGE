#include "mge_image.h"

namespace MGE_UTILITY
{

MGE_Image::MGE_Image()
{
    m_imageInstance = nullptr;
}

MGE_Image::MGE_Image(const char * filename)
{
    m_imageInstance = nullptr;
    this->load(filename);
}

MGE_Image::~MGE_Image()
{
    if(m_imageInstance != nullptr)
    {
        delete m_imageInstance;
    }
}

bool MGE_Image::load(const char * filename)
{
    //try to load image
    if(m_imageInstance != nullptr)
    {
        delete m_imageInstance;
        m_imageInstance = nullptr;
    }

    m_imageInstance = new sf::Image();

    if(!m_imageInstance->loadFromFile(filename))
    {
        //load failed.
        MGE_CORE::MGE_GlobalFunction::getInstance()->mgeWarnMessage("Texture file load failed.Using default texture.");
        m_imageInstance->create(16,16,sf::Color::Red);
        return false;
    }
    return true;
}

int MGE_Image::getWidth()
{
    if(m_imageInstance != nullptr)
    {
        return m_imageInstance->getSize().x;
    }
    return 0;
}

int MGE_Image::getHeight()
{
    if(m_imageInstance != nullptr)
    {
        return m_imageInstance->getSize().y;
    }
    return 0;
}

void MGE_Image::flipHorizontally()
{
    if(m_imageInstance != nullptr)
    {
        return m_imageInstance->flipHorizontally();
    }
}

void MGE_Image::flipVertically()
{
    if(m_imageInstance != nullptr)
    {
        return m_imageInstance->flipVertically();
    }
}

//format in GL_RGBA , GL_UNSIGNED_BYTE
const void * MGE_Image::getPixelsPtr()
{
    if(m_imageInstance != nullptr)
    {
        return (const void *)m_imageInstance->getPixelsPtr();
    }
    return nullptr;
}



}
//namespace MGE_UTILITY
