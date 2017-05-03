#include "mge_glbatchbase.h"

using namespace MGE_CORE;

MGE_GLBatchBase::MGE_GLBatchBase()
{
    //class won't construct when opengl functions is uninitialized
    if(!MGE_GlobalFunction::getInstance()->isGLInitialized())
    {
        MGE_GlobalFunction::getInstance()->mgeQuitApp(MGE_ERRORCODE::GLUnInitialized);
    }

    m_vao = 0;

    m_vbo_pos = 0;
    m_vbo_normal = 0;
    m_vbo_color = 0;

    for(unsigned int i=0;i<MGE_MAX_TUV_SET_PER_BATCH;i++)
    {
        m_vbo_tuv[i] = 0;
    }

    m_vertex_count = 0;

    //status as 0(none)
    m_vertex_attr_status = 0;
    //all expected status as 0(none)
    m_vertex_attr_expected_status = 0;

    //create vao for this batch
    glGenVertexArrays(1,&m_vao);
}

MGE_GLBatchBase::~MGE_GLBatchBase()
{
    //delete vbos
    for(int i = MGE_VATTRIB_POSITION;i!=MGE_VATTRIB_LAST;i++)
    {
        disableBatchVertexAttrib((MGE_VATTRIBS)i,true);
    }

    //finally release vao,vao must be the last one to release
    if(m_vao != 0)
    {
        //delete vao
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&m_vao);
    }
}

void MGE_GLBatchBase::resetBatch()
{
    //delete vbos
    for(int i = MGE_VATTRIB_POSITION;i!=MGE_VATTRIB_LAST;i++)
    {
        disableBatchVertexAttrib((MGE_VATTRIBS)i,true);
    }

    //finally release vao,vao must be the last one to release
    if(m_vao != 0)
    {
        //delete vao
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&m_vao);
    }
}

void MGE_GLBatchBase::enableBatchVertexAttrib(MGE_VATTRIBS attribute)
{
    if(attribute == MGE_VATTRIB_LAST)
    {
        //disable all attributes
        for(int i = MGE_VATTRIB_POSITION;i!=MGE_VATTRIB_LAST;i++)
        {
            disableBatchVertexAttrib((MGE_VATTRIBS)i,false);
        }
        return;
    }

    setAttrExpectStatusBit(attribute);

    updateVAO();
}

void MGE_GLBatchBase::disableBatchVertexAttrib(MGE_VATTRIBS attribute, bool unsetData)
{
    if(attribute == MGE_VATTRIB_LAST)
    {
        //invalid,MGE_VATTRIB_LAST cannot be disabled
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("MGE_VATTRIB_LAST cannot be disabled.");
        return;
    }

    //unset expected status
    unsetAttrExpectStatusBit(attribute);

    //if to unset data
    if(unsetData)
    {
        //then unset the disabled vbo
        switch(attribute)
        {
        case MGE_VATTRIB_NORMAL:
            unsetVertexNormalData();
            break;
        case MGE_VATTRIB_COLOR:
            unsetVertexColorData();
            break;
        case MGE_VATTRIB_TUV_0:
            unsetVertexTUVData(0);
            break;
        case MGE_VATTRIB_TUV_1:
            unsetVertexTUVData(1);
            break;
        case MGE_VATTRIB_TUV_2:
            unsetVertexTUVData(2);
            break;
        case MGE_VATTRIB_TUV_3:
            unsetVertexTUVData(3);
            break;
        default:
            break;
        }
    }

    //do not forget to update VAO
    updateVAO();
}

void MGE_GLBatchBase::setVertexPositionData(GLfloat * data, GLuint vertexCount)
{
    if(!isAttrEnabled(MGE_VATTRIB_POSITION))
    {
        //invalid,MGE_VATTRIB_POSITION is not enabled
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("MGE_VATTRIB_POSITION is disabled, cannot set vertex data.");
        return;
    }

    //unset vbo data
    unsetVertexPositionData();

    //set vbo data
    glGenBuffers(1,&m_vbo_pos);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo_pos);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * vertexCount * 3,data,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    //set vertex count
    if(m_vertex_count == 0)
    {
        m_vertex_count = vertexCount;
    }
    else
    {
        if(m_vertex_count > vertexCount)
        {
            m_vertex_count = vertexCount;
        }
    }

    //set m_vertex_attr_status
    setAttrStatusBit(MGE_VATTRIB_POSITION);


    //don not forget to update VAO
    updateVAO();

    return;
}

void MGE_GLBatchBase::unsetVertexPositionData()
{
    if(m_vbo_pos != 0)
    {
        if(m_vao != 0)
        {
            //unbind vao
            glBindVertexArray(0);

            //unbind buffer
            glBindBuffer(GL_ARRAY_BUFFER,0);

            //delete buffer
            glDeleteBuffers(1,&m_vbo_pos);

            m_vbo_pos = 0;

            //unset m_vertex_attr_status
            unsetAttrStatusBit(MGE_VATTRIB_POSITION);

            m_vertex_count = 0;

        }
    }
    return;
}

void MGE_GLBatchBase::setVertexNormalData(GLfloat *data, GLuint vertexCount)
{
    if(!isAttrEnabled(MGE_VATTRIB_NORMAL))
    {
        //invalid,MGE_VATTRIB_NORMAL is not enabled
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("MGE_VATTRIB_NORMAL is disabled, cannot set vertex data.");
        return;
    }

    //unset vbo data
    unsetVertexNormalData();

    //set vbo data
    glGenBuffers(1,&m_vbo_normal);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo_normal);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * vertexCount * 3,data,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    //set vertex count
    if(m_vertex_count == 0)
    {
        m_vertex_count = vertexCount;
    }
    else
    {
        if(m_vertex_count > vertexCount)
        {
            m_vertex_count = vertexCount;
        }
    }

    //set m_vertex_attr_status
    setAttrStatusBit(MGE_VATTRIB_NORMAL);

    //don not forget to update VAO
    updateVAO();

    return;
}

void MGE_GLBatchBase::unsetVertexNormalData()
{
    if(m_vbo_normal != 0)
    {
        if(m_vao != 0)
        {
            //unbind vao
            glBindVertexArray(0);

            //unbind buffer
            glBindBuffer(GL_ARRAY_BUFFER,0);

            //delete buffer
            glDeleteBuffers(1,&m_vbo_normal);

            m_vbo_normal = 0;

            //unset m_vertex_attr_status
            unsetAttrStatusBit(MGE_VATTRIB_NORMAL);
        }
    }
    return;
}

void MGE_GLBatchBase::setVertexColorData(GLfloat * data,GLuint vertexCount)
{
    if(!isAttrEnabled(MGE_VATTRIB_COLOR))
    {
        //invalid,MGE_VATTRIB_COLOR is not enabled
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("MGE_VATTRIB_COLOR is disabled, cannot set vertex data.");
        return;
    }

    //unset vbo data
    unsetVertexColorData();


    //set vbo data
    glGenBuffers(1,&m_vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo_color);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * vertexCount * 4,data,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    //set vertex count
    if(m_vertex_count == 0)
    {
        m_vertex_count = vertexCount;
    }
    else
    {
        if(m_vertex_count > vertexCount)
        {
            m_vertex_count = vertexCount;
        }
    }


    //set m_vertex_attr_status
    setAttrStatusBit(MGE_VATTRIB_COLOR);

    //don not forget to update VAO
    updateVAO();

    return;
}

void MGE_GLBatchBase::unsetVertexColorData()
{
    if(m_vbo_color != 0)
    {
        if(m_vao != 0)
        {
            //unbind vao
            glBindVertexArray(0);

            //unbind buffer
            glBindBuffer(GL_ARRAY_BUFFER,0);

            //delete buffer
            glDeleteBuffers(1,&m_vbo_color);

            m_vbo_color = 0;

            //unset m_vertex_attr_status
            unsetAttrStatusBit(MGE_VATTRIB_COLOR);
        }
    }
    return;
}

void MGE_GLBatchBase::setVertexTUVData(GLfloat * data,GLuint vertexCount,GLuint textureChannal)
{
    //check channal number
    if(textureChannal >= MGE_MAX_TUV_SET_PER_BATCH)
    {
        //invalid channal
        //invalid texture channal
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("Invalid texture channal.");
        return;
    }

    if(!isAttrEnabled((MGE_VATTRIBS)((int)MGE_VATTRIB_TUV_0+textureChannal)))
    {
        //invalid,MGE_VATTRIB_TUV_N is not enabled
        MGE_GlobalFunction::getInstance()->mgeWarnMessage("MGE_VATTRIB_TUV_X is disabled, cannot set vertex data.");
        return;
    }

    //unset vbo data
    unsetVertexTUVData(textureChannal);


    //set vbo data
    glGenBuffers(1,&m_vbo_tuv[textureChannal]);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo_tuv[textureChannal]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat) * vertexCount * 2,data,GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    //set vertex count
    if(m_vertex_count == 0)
    {
        m_vertex_count = vertexCount;
    }
    else
    {
        if(m_vertex_count > vertexCount)
        {
            m_vertex_count = vertexCount;
        }
    }


    //set m_vertex_attr_status
    setAttrStatusBit((MGE_VATTRIBS)((int)MGE_VATTRIB_TUV_0+textureChannal));

    //don not forget to update VAO
    updateVAO();

    return;
}

void MGE_GLBatchBase::unsetVertexTUVData(GLuint textureChannal)
{
    //check channal number
    if(textureChannal >= MGE_MAX_TUV_SET_PER_BATCH)
    {
        //invalid channal
        return;
    }

    if(m_vbo_tuv[textureChannal]!= 0)
    {
        if(m_vao != 0)
        {
            //unbind vao
            glBindVertexArray(0);

            //unbind buffer
            glBindBuffer(GL_ARRAY_BUFFER,0);

            //delete buffer
            glDeleteBuffers(1,&m_vbo_tuv[textureChannal]);

            m_vbo_tuv[textureChannal] = 0;

            //unset m_vertex_attr_status
            unsetAttrStatusBit((MGE_VATTRIBS)((int)MGE_VATTRIB_TUV_0+textureChannal));

        }
    }
    return;
}

void MGE_GLBatchBase::draw()
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertex_count);
    glBindVertexArray(0);
}

bool MGE_GLBatchBase::isAttrEnabled(MGE_VATTRIBS attribute)
{
    unsigned int result = 0;
    result = result | (1<<attribute);

    if(result & m_vertex_attr_expected_status)
    {
        return true;
    }
    return false;
}

bool MGE_GLBatchBase::isAttrDataSet(MGE_VATTRIBS attribute)
{
    unsigned int result = 0;
    result = result | (1<<attribute);

    if(result & m_vertex_attr_status)
    {
        return true;
    }
    return false;
}

void MGE_GLBatchBase::updateVAO()
{
    //delete if vao exists
    if( m_vao !=0 )
    {
        glBindVertexArray(0);
        glDeleteVertexArrays(1,&m_vao);
    }

    //create new vao
    glGenVertexArrays(1,&m_vao);
    glBindVertexArray(m_vao);

    //starting collect vertex attribute infomation

    //bind vao in order to record vertex attributes infomation
    glBindVertexArray(m_vao);

    for(int i = MGE_VATTRIB_POSITION;i!=MGE_VATTRIB_LAST;i++)
    {
        //skip unabled attributes and skip attributes which haven't been set data
        if(!isAttrEnabled((MGE_VATTRIBS)i) || !isAttrDataSet((MGE_VATTRIBS)i)) continue;

        switch (i)
        {
        case (int)MGE_VATTRIB_POSITION:
            glBindBuffer(GL_ARRAY_BUFFER,m_vbo_pos);
            glVertexAttribPointer(MGE_VATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(MGE_VATTRIB_POSITION);
            break;
        case (int)MGE_VATTRIB_NORMAL:
            glBindBuffer(GL_ARRAY_BUFFER,m_vbo_normal);
            glVertexAttribPointer(MGE_VATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(MGE_VATTRIB_NORMAL);
            break;
        case (int)MGE_VATTRIB_COLOR:
            glBindBuffer(GL_ARRAY_BUFFER,m_vbo_color);
            glVertexAttribPointer(MGE_VATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(MGE_VATTRIB_COLOR);
            break;
        case (int)MGE_VATTRIB_TUV_0:
            glBindBuffer(GL_ARRAY_BUFFER,m_vbo_tuv[0]);
            glVertexAttribPointer(MGE_VATTRIB_TUV_0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(MGE_VATTRIB_TUV_0);
            break;
        case (int)MGE_VATTRIB_TUV_1:
            glBindBuffer(GL_ARRAY_BUFFER,m_vbo_tuv[1]);
            glVertexAttribPointer(MGE_VATTRIB_TUV_1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(MGE_VATTRIB_TUV_1);
            break;
        case (int)MGE_VATTRIB_TUV_2:
            glBindBuffer(GL_ARRAY_BUFFER,m_vbo_tuv[2]);
            glVertexAttribPointer(MGE_VATTRIB_TUV_2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(MGE_VATTRIB_TUV_2);
            break;
        case (int)MGE_VATTRIB_TUV_3:
            glBindBuffer(GL_ARRAY_BUFFER,m_vbo_tuv[3]);
            glVertexAttribPointer(MGE_VATTRIB_TUV_3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(MGE_VATTRIB_TUV_3);
            break;
        default:
            break;
        }
    }

    //unbind vao
    glBindVertexArray(0);

    //unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER,0);

    return;
}


void MGE_GLBatchBase::setAttrStatusBit(MGE_VATTRIBS attribute)
{

    //*****************just on big edian arch
    //bit operation,may cause cross-platform issues

    m_vertex_attr_status = m_vertex_attr_status | (1<<attribute);

    //*****************just on big edian arch
}

void MGE_GLBatchBase::unsetAttrStatusBit(MGE_VATTRIBS attribute)
{
    //*****************just on big edian arch
    //bit operation,may cause cross-platform issues

    unsigned int temp = ~(1<<attribute);
    m_vertex_attr_status = m_vertex_attr_status & temp;

    //*****************just on big edian arch
}


void MGE_GLBatchBase::setAttrExpectStatusBit(MGE_VATTRIBS attribute)
{
    //*****************just on big edian arch
    //bit operation,may cause cross-platform issues

    m_vertex_attr_expected_status = m_vertex_attr_expected_status | (1<<attribute);

    //*****************just on big edian arch
}

void MGE_GLBatchBase::unsetAttrExpectStatusBit(MGE_VATTRIBS attribute)
{
    //*****************just on big edian arch
    //bit operation,may cause cross-platform issues

    unsigned int temp = ~(1<<attribute);
    m_vertex_attr_expected_status = m_vertex_attr_expected_status & temp;

    //*****************just on big edian arch
}
