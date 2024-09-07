#include "controller.h"
Controller::Controller(const vec2 &initPos, GLuint ballApp,GLuint sporeApp,GLuint effectIdx):role(ballApp,sporeApp,effectIdx,initPos)
{
    CrossHairPos=initPos;
}

void Controller::SetViewArea()
{
    auto&&ret=GetRoleViewSizeAndViewCenter(role);
    ViewAreaSize=ret.first;
    ViewCenter=ret.second;
}

pair<vec2, vec2> Controller::GetRoleViewSizeAndViewCenter(Role &role)
{
    static const double minView=50;
    static const double k=MapSize/(WeightWin-minView);
    static const double fac=1.5;
    float xl=INT_MAX,xr=INT_MIN,yt=INT_MIN,yb=INT_MAX;
    float x_sum=0,y_sum=0,w_sum=0;
    for(auto&ball:role.balls)
    {
        auto&p=ball.pos;
        auto r=ball.GetRadian();
        xl=min(xl,p.x-r);
        xr=max(xr,p.x+r);
        yt=max(yt,p.y+r);
        yb=min(yb,p.y-r);
        x_sum+=p.x*ball.weight;
        y_sum+=p.y*ball.weight;
        w_sum+=ball.weight;
    }
    float dis=max(xr-xl,yt-yb)*fac;
    dis=fmax(dis,k*w_sum+minView);
    auto ViewAreaSize=vec2{dis,dis};
    auto ViewCenter=vec2{x_sum/w_sum,y_sum/w_sum};
    return pair<vec2, vec2>{ViewAreaSize,ViewCenter};
}

void Controller::SetCrossHairPos(ActionBar &bar)
{
    const static float maxExtent=sqrt(2.0);
    CrossHairPos=bar.dir*(bar.extent*maxExtent);
}

void Controller::Move(ActionBar&bar)
{
    function<void(Ball&)>fun=[&](Ball&ball)->void{
         vec2 tarPos=ball.pos+ball.moveDir*(ball.GetMaxSpeed()*RenderTime/1000*bar.extent);
         if(CheckIsOverEdge(tarPos)==0)ball.pos=tarPos;
    };
    for(auto&ball:role.balls)
    {
        ball.Move(fun);
    }
}

void Controller::RenderCrossHair()
{
    static ShaderProgram*p=0;
    static VertexArrayObj*vao;
    if(p==0)
    {
        p=new ShaderProgram("src/Shader/CrossHair.vert","src/Shader/CrossHair.frag");
        vao=new VertexArrayObj();
        const GLfloat scale=0.05;
        GLfloat vertex[][2]={{-scale,0},{scale,0},{0,scale},{0,-scale}};
        VertexBufferObj*vbo=new VertexBufferObj(sizeof(vertex),vertex);
        vbo->setpoint(0,GL_FLOAT,2,sizeof(vec2),0);
        vao->unbind();
    }
    p->bind();
    p->setvec2("offset",CrossHairPos);
    vao->bind();
    openglAPIPointer->glDrawArrays(GL_LINES,0,4);
}

Controller::~Controller()
{

}
