#ifndef MGE_GLBATCHBASE_H
#define MGE_GLBATCHBASE_H

#include "mge_globalfunction.h"

//using namespace MGE_CORE
namespace MGE_CORE
{

//base class for glbatch.A batch is a class that holds all vertex data,like positions,colors,normals etc.
class MGE_GLBatchBase
{

protected:

    GLuint m_vao;
    GLuint m_vbo_pos;
    GLuint m_vbo_normal;
    GLuint m_vbo_color;
    GLuint m_vbo_tuv[MGE_MAX_TUV_SET_PER_BATCH];
    GLuint m_vertex_count;
    unsigned int m_vertex_attr_status;
    unsigned int m_vertex_attr_expected_status;

public:

    MGE_GLBatchBase();
    ~MGE_GLBatchBase();

    void resetBatch();
    void setVertexPositionData(GLfloat * data,GLuint vertexCount);
    void setVertexNormalData(GLfloat * data,GLuint vertexCount);
    void setVertexColorData(GLfloat * data,GLuint vertexCount);
    void setVertexTUVData(GLfloat * data,GLuint vertexCount,GLuint textureChannal);

    //if attribute set to MGE_VATTRIB_LAST,will disable all attributes
    void enableBatchVertexAttrib(MGE_VATTRIBS attribute);
    void disableBatchVertexAttrib(MGE_VATTRIBS attribute,bool unsetData);

    virtual void draw();

protected:

    bool isAttrEnabled(MGE_VATTRIBS attribute);
    bool isAttrDataSet(MGE_VATTRIBS attribute);

    void unsetVertexPositionData();
    void unsetVertexNormalData();
    void unsetVertexColorData();
    void unsetVertexTUVData(GLuint textureChannal);

    void setAttrStatusBit(MGE_VATTRIBS attribute);
    void unsetAttrStatusBit(MGE_VATTRIBS attribute);

    void setAttrExpectStatusBit(MGE_VATTRIBS attribute);
    void unsetAttrExpectStatusBit(MGE_VATTRIBS attribute);

    void updateVAO();
};
//class MGE_GLBatchBase


}
//namespace MGE_CORE





#endif // MGE_GLBATCHBASE_H
