#ifndef MGE_GLCAMERA_H
#define MGE_GLCAMERA_H

#include "mge_globalfunction.h"
#include "mge_math.h"

namespace MGE_CORE
{


class MGE_GLCamera
{

protected:

    glm::vec3 m_position;
    glm::vec3 m_up;
    glm::vec3 m_forward;

public:

    MGE_GLCamera();

    MGE_GLCamera(glm::vec3 pos,glm::vec3 up,glm::vec3 forward);

    ~MGE_GLCamera();

    void setPosition(glm::vec3 pos);
    void setForward(glm::vec3 forward);
    void setUp(glm::vec3 up);

    glm::vec3 getPosition();
    glm::vec3 getForward();
    glm::vec3 getUp();

    virtual void update();

};


class MGE_GLFpsCamera : public MGE_GLCamera
{

public:

    glm::mat4x4 m_vMatrix;
    glm::mat4x4 m_pMatrix;

public:

    MGE_GLFpsCamera();
    ~MGE_GLFpsCamera();

    void * getVMatrixPtr();
    void * getPMatrixPtr();

    void update() override;
    void perspective(float fovy,float aspect,float nearz,float farz);
};

}
//namespace MGE_CORE







#endif //MGE_GLCAMERA_H
