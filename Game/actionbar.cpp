#include "actionbar.h"

ActionBar::ActionBar()
{
   offset=0.1;
   scale0=0.2;
   scale1=0.1;
   barOffset0=vec2{scale0-1.0+offset,scale0-1.0+offset};
   barOffset1=barOffset0;
   isclicked=0;
   extent=0;
   dir=vec2{0,0};
}

void ActionBar::Render()
{
    static ShaderProgram*p=0;
    static VertexArrayObj*vao;
    static const GLuint vertexCnt=40;
    if(p==0)
    {
        p=new ShaderProgram("src/Shader/ActionBar.vert","src/Shader/ActionBar.frag");
        vao=new VertexArrayObj;
        ////////////////////////////生成圆的顶点
        vector<vec2>vertex(vertexCnt);
        vector<tuple<GLuint,GLuint,GLuint>>indices(vertexCnt-2);
        for(int i=0;i<vertexCnt;++i)vertex[i]=vec2{cos(i*2*PI/vertexCnt),sin(i*2*PI/vertexCnt)};
        for(int i=0;i<vertexCnt-2;++i)indices[i]=tuple<GLuint,GLuint,GLuint>{0,i+1,i+2};
        VertexBufferObj*vbo=new VertexBufferObj(vertex.size()*sizeof(vec2),vertex.data());
        vbo->setpoint(0,GL_FLOAT,2,sizeof(vec2),0);
        ElementBufferObj*ebo=new ElementBufferObj(indices.size()*sizeof(GLuint)*3,indices.data());
        vao->unbind();
    }
    /////////////////////////////
    vec3 OffsetAndScale0=vec3{barOffset0.x,barOffset0.y,scale0};
    vec3 OffsetAndScale1=vec3{barOffset1.x,barOffset1.y,scale1};
    const static GLfloat color0[4]={1.0,1.0,1.0,0.2};
    const static GLfloat color1[4]={1.0,1.0,1.0,1.0};
    openglAPIPointer->glEnable(GL_BLEND);
    openglAPIPointer->glBlendEquation(GL_FUNC_ADD);
    openglAPIPointer->glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    p->bind();
    p->setvec3("OffsetAndScale",OffsetAndScale0);
    p->setvec4("color",color0);
    vao->bind();
    openglAPIPointer->glDrawElements(GL_TRIANGLES,(vertexCnt-2)*3,GL_UNSIGNED_INT,0);
    openglAPIPointer->glDisable(GL_BLEND);
    p->setvec3("OffsetAndScale",OffsetAndScale1);
    p->setvec4("color",color1);
    openglAPIPointer->glDrawElements(GL_TRIANGLES,(vertexCnt-2)*3,GL_UNSIGNED_INT,0);
}

void ActionBar::SetClicked(GLfloat x, GLfloat y)
{
    if(pow(x-barOffset0.x,2)+pow(y-barOffset0.y,2)<=scale0*scale0)isclicked=1;
}

void ActionBar::MoveBar(GLfloat x, GLfloat y)
{
    if(isclicked)
    {
        float xo=x-barOffset0.x,yo=y-barOffset0.y;
        dir=vec2{xo,yo}.normalize();
        float dis=sqrt(xo*xo+yo*yo);
        extent=min(dis/scale0,1.0f);
        barOffset1=barOffset0+dir*scale0*extent;
    }
}

void ActionBar::ResetBarPos()
{
    barOffset1=barOffset0;
    isclicked=0;
    extent=0;
}

ActionBar::~ActionBar()
{

}
