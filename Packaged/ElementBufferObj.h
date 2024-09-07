#ifndef EBO_H
#define EBO_H
#include"include.h"
class ElementBufferObj
{
private:
    GLuint id;
public:
    ElementBufferObj(GLuint size,void*data);
    void bind();
    ~ElementBufferObj();
};

#endif // EBO_H
