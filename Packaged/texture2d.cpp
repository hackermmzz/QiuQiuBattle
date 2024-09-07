#include "texture2d.h"


Texture2D::Texture2D(const string &file)
{
    openglAPIPointer->glGenTextures(1,&id);
    openglAPIPointer->glBindTexture(GL_TEXTURE_2D,id);
    auto img=QImage(file.c_str()).convertToFormat(QImage::Format_RGBA8888);
    width=img.width(),height=img.height();
    openglAPIPointer->glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,img.bits());
    openglAPIPointer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    openglAPIPointer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    openglAPIPointer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    openglAPIPointer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    openglAPIPointer->glGenerateMipmap(GL_TEXTURE_2D);
}

Texture2D::Texture2D(GLuint w, GLuint h, void *data)
{
    openglAPIPointer->glGenTextures(1,&id);
    openglAPIPointer->glBindTexture(GL_TEXTURE_2D,id);
    width=w,height=h;
    openglAPIPointer->glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    openglAPIPointer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    openglAPIPointer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    openglAPIPointer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    openglAPIPointer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    openglAPIPointer->glGenerateMipmap(GL_TEXTURE_2D);

}

void Texture2D::bind(GLuint idx)
{
    openglAPIPointer->glActiveTexture(GL_TEXTURE0+idx);
    openglAPIPointer->glBindTexture(GL_TEXTURE_2D,id);
}

void Texture2D::draw()
{
    static ShaderProgram*p=0;
    static VertexArrayObj*vao;
    if(!p)
    {
        p=new ShaderProgram("src/Shader/TextureDraw.vert","src/Shader/TextureDraw.frag");
        vao=new VertexArrayObj();
        GLfloat indices[]={0,1,2,0,2,3};
        auto*vbo=new VertexBufferObj(sizeof(indices),indices);
        vbo->setpoint(0,GL_FLOAT,1,sizeof(float),0);
        vao->unbind();
    }
    vao->bind();
    p->bind();
    p->setuint("sampler",0);
    bind(0);
    openglAPIPointer->glDrawArrays(GL_TRIANGLES,0,6);;
}

Texture2D::~Texture2D()
{
    openglAPIPointer->glDeleteTextures(1,&id);
}
