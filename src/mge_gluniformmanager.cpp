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

MGE_GLUniformManager * MGE_GLUniformManager::m_instance = new MGE_GLUniformManager();

MGE_GLUniformManager * MGE_GLUniformManager::getInstance()
{
    return m_instance;
}

void MGE_GLUniformManager::registerLocalUniform(std::string name, MGE_UniformType type, void * pointer)
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
        m_uniforms.insert(std::make_pair(name,MGE_SingalUniform(type,pointer)));
    }

    m_uniforms_write_lock.unlock();
    return;
}

void MGE_GLUniformManager::unregisterLocalUniform(std::string name)
{
    m_uniforms_write_lock.lock();

    auto item = m_uniforms.find(name);
    if(item != m_uniforms.end())
    {
        //found
        m_uniforms.erase(item);
    }
    else
    {
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("Uniform not found.Can not unregister.");
    }

    m_uniforms_write_lock.unlock();
}

MGE_SingalUniform & MGE_GLUniformManager::getUniform(std::string name)
{

}



}
//namespace MGE_CORE
