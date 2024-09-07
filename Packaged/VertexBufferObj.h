#ifndef VBO_H
#define VBO_H
#include"include.h"
struct  VertexBufferObj
{
    GLuint id;
    ////////////////////////
    VertexBufferObj(GLuint size,void*data);
    ~VertexBufferObj();
    void bind();
    void *map(GLuint offset,GLuint size,GLenum mask=GL_MAP_WRITE_BIT|GL_MAP_READ_BIT);
    void unmap();
    void setdata(GLuint offset,GLuint size,void*data);
    void setpoint(GLuint location, GLenum value_type, GLuint num, GLuint step_size, GLuint offset);
    void setpoint(GLuint location, GLuint divisor);
};

#endif // VBO_H
