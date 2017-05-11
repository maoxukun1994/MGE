#include "mge_gluniformmanager.h"

namespace MGE_CORE
{

MGE_GLUniformManager::MGE_GLUniformManager()
{
}

void MGE_GLUniformManager::framePassUpdate()
{

}

void MGE_GLUniformManager::fixedTimeUpdate()
{

}

MGE_GLUniformManager::~MGE_GLUniformManager()
{

}

MGE_GLUniformManager * MGE_GLUniformManager::getInstance()
{
    static MGE_GLUniformManager m_instance;
    return &m_instance;
}

void MGE_GLUniformManager::registerHostUniform(std::string name, MGE_UniformType type, void * pointer)
{
    m_uniforms_write_lock.lock();

    auto item = m_uniforms.find(name);
    if(item != m_uniforms.end())
    {
        //already has same key.Check values.
        if(item->second.type == type && item->second.dataPointer == pointer)
        {
            //if content are the same,then this uniform already been registered.
            MGE_GlobalFunction::getInstance()->mgeWarnMessage("Uniform already registered.");
        }
        else
        {
            MGE_GlobalFunction::getInstance()->mgeErrorMessage("Uniform name conflict detected.Not registering.");
        }
    }
    else
    {
        //item not found.Register a new one.
        m_uniforms.insert(std::make_pair(name,MGE_HostUniform(type,pointer)));
    }

    m_uniforms_write_lock.unlock();
    return;
}

void MGE_GLUniformManager::unregisterHostUniform(std::string name)
{
    m_uniforms_write_lock.lock();

    auto item = m_uniforms.find(name);
    if(item != m_uniforms.end())
    {
        //found
        //check reference count
        if(item->second.refCount != 0)
        {
            MGE_GlobalFunction::getInstance()->mgeWarnMessage("Can not unregister uniform that holds references.");
        }
        else
        {
            m_uniforms.erase(item);
        }
    }
    else
    {
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("Uniform not found.Can not unregister.");
    }

    m_uniforms_write_lock.unlock();
}

bool MGE_GLUniformManager::bindShaderUniform(MGE_ShaderUniform &uniform)
{
    m_uniforms_write_lock.lock();

    bool ret = false;

    auto item = m_uniforms.find(uniform.name);
    if(item != m_uniforms.end())
    {
        //found
        //link pointer
        uniform.data = item->second.dataPointer;
        //increase reference count
        ++(item->second.refCount);
        ret = true;
    }
    else
    {
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("Uniform not found in uniformManager.Can not bind.");
    }

    m_uniforms_write_lock.unlock();

    return ret;
}

void MGE_GLUniformManager::unbindShaderUniform(MGE_ShaderUniform &uniform)
{
    m_uniforms_write_lock.lock();

    auto item = m_uniforms.find(uniform.name);
    if(item != m_uniforms.end())
    {
        //found
        //unlink pointer
        uniform.data = nullptr;
        //remove reference count
        --(item->second.refCount);
    }
    else
    {
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("Uniform not found in uniformManager.Can not unbind.");
    }

    m_uniforms_write_lock.unlock();
}

MGE_HostUniform MGE_GLUniformManager::getUniform(std::string name)
{
    m_uniforms_write_lock.lock();

    MGE_HostUniform ret;
    auto item = m_uniforms.find(name);
    if(item != m_uniforms.end())
    {
        //found
        ret = item->second;
    }
    else
    {
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("Can not get uniform.Uniform not found.");
    }

    m_uniforms_write_lock.unlock();

    return ret;
}



}
//namespace MGE_CORE
