#include "mge_glcamera.h"


namespace MGE_CORE
{

MGE_GLCamera::MGE_GLCamera()
{
    m_position = Vec3(0.0f,0.0f,0.0f);
    m_up = Vec3(0.0f,0.0f,1.0f);
    m_forward = Vec3(0.0f,1.0f,0.0f);
}

MGE_GLCamera::MGE_GLCamera(Vec3 pos,Vec3 up,Vec3 forward)
{
    m_position = pos;
    m_up = up;
    m_forward = forward;
}

MGE_GLCamera::~MGE_GLCamera()
{

}

void MGE_GLCamera::setPosition(Vec3 pos)
{
    m_position = pos;
}

void MGE_GLCamera::setForward(Vec3 forward)
{
    m_forward = forward;
}

void MGE_GLCamera::setUp(Vec3 up)
{
    m_up = up;
}

Vec3 MGE_GLCamera::getPosition()
{
    return m_position;
}

Vec3 MGE_GLCamera::getForward()
{
    return m_forward;
}

Vec3 MGE_GLCamera::getUp()
{
    return m_up;
}

void MGE_GLCamera::update()
{
}


MGE_GLFpsCamera::MGE_GLFpsCamera():MGE_GLCamera()
{

}

MGE_GLFpsCamera::~MGE_GLFpsCamera()
{

}

void * MGE_GLFpsCamera::getVMatrixPtr()
{
    return (void *)glm::value_ptr(m_vMatrix);
}

void * MGE_GLFpsCamera::getPMatrixPtr()
{
    return (void *)glm::value_ptr(m_pMatrix);
}

void MGE_GLFpsCamera::update()
{
    m_vMatrix = glm::lookAt(m_position,m_position+m_forward,m_up);
}

void MGE_GLFpsCamera::perspective(float fovy,float aspect,float nearz,float farz)
{
    m_pMatrix = glm::perspective(fovy,aspect,nearz,farz);
}















}
//namespace MGE_CORE
