#include "VertexBufferObj.h"

VertexBufferObj::VertexBufferObj(GLuint size,void*data)
{
    openglAPIPointer->glGenBuffers(1,&id);
    bind();
    openglAPIPointer->glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

}

VertexBufferObj::~VertexBufferObj()
{
    if(id)openglAPIPointer->glDeleteBuffers(1,&id);
}

void VertexBufferObj::bind()
{
openglAPIPointer->glBindBuffer(GL_ARRAY_BUFFER,id);
}

void *VertexBufferObj::map(GLuint offset, GLuint size,GLenum mask){
    void *ptr =openglAPIPointer->glMapBufferRange(GL_ARRAY_BUFFER, offset, size, mask);
    return ptr;
}

void VertexBufferObj::unmap()
{
    openglAPIPointer->glUnmapBuffer(GL_ARRAY_BUFFER);
}

void VertexBufferObj::setdata(GLuint offset, GLuint size, void *data)
{
    openglAPIPointer->glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBufferObj::setpoint(GLuint location, GLenum value_type, GLuint num, GLuint step_size, GLuint offset)
{
    openglAPIPointer->glEnableVertexAttribArray(location);
    openglAPIPointer->glVertexAttribPointer(location, num, value_type, GL_FALSE, step_size,(void*)(offset));
}

void VertexBufferObj::setpoint(GLuint location, GLuint divisor)
{
    openglAPIPointer->glEnableVertexAttribArray(location);
    openglAPIPointer->glVertexAttribDivisor(location, divisor);
}
