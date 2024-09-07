#include "VertexArrayObj.h"
VertexArrayObj::VertexArrayObj()
{
    openglAPIPointer->glGenVertexArrays(1,&id);
    bind();
}

void VertexArrayObj::bind()
{
    openglAPIPointer->glBindVertexArray(id);
}

void VertexArrayObj::unbind()
{
    openglAPIPointer->glBindVertexArray(0);;
}
VertexArrayObj::~VertexArrayObj()
{
    if(id)
        openglAPIPointer->glDeleteVertexArrays(1,&id);
}
