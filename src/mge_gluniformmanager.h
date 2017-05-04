#ifndef MGE_GLUNIFORMMANAGER_H
#define MGE_GLUNIFORMMANAGER_H

#include "mge_globalfunction.h"

namespace MGE_CORE
{

typedef struct mgeuniform
{
    MGE_UniformType type;
    void * dataPointer;
    //constructor
    mgeuniform(MGE_UniformType utype = NOTYPE,void * udataPointer = nullptr):type(utype),dataPointer(udataPointer){}
}
MGE_SingalUniform;

//the uniform manager needs to be thread-safe
//caution:
//the design propose of this class is to provide a manageable uniform set
class MGE_GLUniformManager
{

private:

    static MGE_GLUniformManager * m_instance;

    std::unordered_map<std::string,MGE_SingalUniform> m_uniforms;

    std::mutex m_uniforms_write_lock;

public:

private:

    MGE_GLUniformManager();

    void framePassUpdate();

    void fixedTimeUpdate();

public:

    ~MGE_GLUniformManager();

    MGE_GLUniformManager * getInstance();

    void registerLocalUniform(std::string name,MGE_UniformType type,void * pointer);

    void unregisterLocalUniform(std::string name);

    MGE_SingalUniform & getUniform(std::string name);
};




}
//namespace MGE_CORE

#endif // MGE_GLUNIFORMMANAGER_H
