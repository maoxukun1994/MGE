#include "mge_glprogrammanager.h"

namespace MGE_CORE
{

MGE_GLProgramManager::MGE_GLProgramManager()
{
    m_programs.reserve(MGE_SHADER_CONTAINER_INITSIZE);
}


MGE_GLProgramManager::~MGE_GLProgramManager()
{
    for(auto p : m_programs)
    {
        if(p != nullptr)
        {
            delete p;
        }
    }
}

//use static varible
MGE_GLProgramManager * MGE_GLProgramManager::m_instance = new MGE_GLProgramManager();
MGE_GLProgramManager * MGE_GLProgramManager::getInstance()
{
    return m_instance;
}

//MGE_GLProgramManager * MGE_GLProgramManager::getInstance()
//{
//    return &m_instance;
//}

MGE_GLShaderProgram * MGE_GLProgramManager::getProgram(unsigned int id)
{
    if(id < m_programs.size())
    {
        return m_programs.at(id);
    }
    return nullptr;
}

int MGE_GLProgramManager::addProgram(MGE_GLShaderProgram * program)
{
    int index = -1;
    if(program != nullptr)
    {
        m_program_vector_lock.lock();
        m_programs.push_back(program);
        index = m_programs.size()-1;
        m_program_vector_lock.unlock();
    }
    else
    {
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("Not adding null shader program to program manager.");
    }
    return index;
}


}
//namespace MGE_CORE
