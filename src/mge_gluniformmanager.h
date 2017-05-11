#ifndef MGE_GLUNIFORMMANAGER_H
#define MGE_GLUNIFORMMANAGER_H

#include "mge_globalfunction.h"

namespace MGE_CORE
{

//the uniform manager needs to be thread-safe
//caution:
//the design propose of this class is to provide a manageable uniform set
//but not to assure the status of uniform data pointers.
//the class will NOT take over ownership of any dataPointer of MGE_HostUniforms
//so it's other classes' responsibility to assure they don't change the pointer,or
//called proper functions(such as unregister..etc.)to inform the class that the uniform pointer
//has changed.
class MGE_GLUniformManager
{

private:

    std::unordered_map<std::string,MGE_HostUniform> m_uniforms;

    std::mutex m_uniforms_write_lock;

public:

private:

    MGE_GLUniformManager();

    void framePassUpdate();

    void fixedTimeUpdate();

public:

    ~MGE_GLUniformManager();

    static MGE_GLUniformManager * getInstance();

    void registerHostUniform(std::string name,MGE_UniformType type,void * pointer);

    void unregisterHostUniform(std::string name);

    bool bindShaderUniform(MGE_ShaderUniform &uniform);

    void unbindShaderUniform(MGE_ShaderUniform &uniform);

    MGE_HostUniform getUniform(std::string name);
};




}
//namespace MGE_CORE

#endif // MGE_GLUNIFORMMANAGER_H
