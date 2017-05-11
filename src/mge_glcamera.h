#ifndef MGE_GLCAMERA_H
#define MGE_GLCAMERA_H

#include "mge_globalfunction.h"
#include "mge_math.h"

namespace MGE_CORE
{


class MGE_GLCamera
{

protected:

    Vec3 m_position;
    Vec3 m_up;
    Vec3 m_forward;

public:

    MGE_GLCamera();

    MGE_GLCamera(Vec3 pos,Vec3 up,Vec3 forward);

    virtual ~MGE_GLCamera();

    void setPosition(Vec3 pos);
    void setForward(Vec3 forward);
    void setUp(Vec3 up);

    Vec3 getPosition();
    Vec3 getForward();
    Vec3 getUp();

    virtual void update();

};


class MGE_GLFpsCamera : public MGE_GLCamera
{

public:

    Mat4 m_vMatrix;
    Mat4 m_pMatrix;

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
