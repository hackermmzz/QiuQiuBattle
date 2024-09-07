#include "ShaderStorageBufferObj.h"

ShaderStorageBufferObj::ShaderStorageBufferObj(GLuint size)
{
    openglAPIPointer->glGenBuffers(1,&id);
    bind();
    openglAPIPointer->glBufferData(GL_SHADER_STORAGE_BUFFER,size,0,GL_STATIC_DRAW);
}

void *ShaderStorageBufferObj::map(GLuint offset, GLuint size, GLbitfield mask){
    void *ptr =openglAPIPointer->glMapBufferRange(GL_SHADER_STORAGE_BUFFER, offset, size, mask);
    return ptr;
}

void ShaderStorageBufferObj::unmap()
{
    openglAPIPointer->glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void ShaderStorageBufferObj::bind()
{
openglAPIPointer->glBindBuffer(GL_SHADER_STORAGE_BUFFER,id);
}

void ShaderStorageBufferObj::bindBase(GLuint idx)
{
 openglAPIPointer->glBindBufferBase(GL_SHADER_STORAGE_BUFFER,idx,id);
}
