#include "ElementBufferObj.h"

ElementBufferObj::ElementBufferObj(GLuint size,void*data)
{
    openglAPIPointer->glGenBuffers(1,&id);
        bind();
    openglAPIPointer->glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,data,GL_STATIC_DRAW);

}

void ElementBufferObj::bind()
{
openglAPIPointer->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
}

ElementBufferObj::~ElementBufferObj()
{
    if(id)openglAPIPointer->glDeleteBuffers(1,&id);
}
