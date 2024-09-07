#ifndef SSBO_H
#define SSBO_H
#include"include.h"
class ShaderStorageBufferObj
{
private:
    GLuint id;
public:
    ShaderStorageBufferObj(GLuint size);
    void unmap();
    void *map(GLuint offset, GLuint size,GLbitfield mask=GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    void bind();
    void bindBase(GLuint idx);
    ~ShaderStorageBufferObj();
};

#endif // SSBO_H
