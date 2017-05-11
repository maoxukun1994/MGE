#ifndef MGE_GLPROGRAMMANAGER_H
#define MGE_GLPROGRAMMANAGER_H

#include "mge_globalfunction.h"
#include "mge_glshaderprogram.h"

//namespace MGE_CORE
namespace MGE_CORE
{

//class mge_GLProgramManager
class MGE_GLProgramManager
{

private:

    std::vector<MGE_GLShaderProgram *> m_programs;

    std::mutex m_program_vector_lock;

private:

    MGE_GLProgramManager();

public:

    ~MGE_GLProgramManager();

    static MGE_GLProgramManager * getInstance();

    MGE_GLShaderProgram * getProgram(unsigned int id);

    //this class will takeover ownship of any MGE_GLShaderProgram instances whose pointer has been passed
    //into this function as parameter,return the id (index) of the program in vector
    int addProgram(MGE_GLShaderProgram * program);

};
//class MGE_GLProgramManager



}
//namespace MGE_CORE

#endif // MGE_GLPROGRAMMANAGER_H
