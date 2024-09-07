#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include"include.h"
#include"string"
#include"qimage.h"
#include"Shaderprogram.h"
#include"VertexArrayObj.h"
#include"VertexBufferObj.h"
using namespace std;
class Texture2D
{
private:
    GLuint id;
    GLuint width,height;
public:
    Texture2D(const string&file);
    Texture2D(GLuint w,GLuint h,void*data);
    void bind(GLuint idx);
    void draw();
    ~Texture2D();
};

#endif // TEXTURE2D_H
