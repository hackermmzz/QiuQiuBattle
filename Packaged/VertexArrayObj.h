#ifndef VAO_H
#define VAO_H
#include"include.h"
class VertexArrayObj
{
private:
       GLuint id;
public:
    VertexArrayObj();
    ~VertexArrayObj();
    void bind();
    void unbind();
};

#endif // VAO_H
