#include "boom.h"

Boom::Boom(const vec2 &pos_, GLuint weight_):Ball(BallTypesRealCnt-1,-1,0,pos_)
{
    weight=weight_;
}

void Boom::AddToTree()
{
    auto r=GetRadian();
    float xl=pos.x-r,xr=pos.x+r,yt=pos.y+r,yb=pos.y-r;
    auto&&ret=GetTreeTwoNodes(BoomsTree,xl,xr,yt,yb);
    static const auto mb=BoomsTree.MinBlockSize.x;
    static const auto mh=mb/2;
    float xb=ret.first->pos.x+mh,xe=ret.second->pos.x+mh;
    yt=ret.first->pos.y-mh,yb=ret.second->pos.y-mh;
    for(auto x=xb;x<=xe;x+=mb)
    {
        for(auto y=yt;y>=yb;y-=mb)
        {
            auto*node=(vector<Boom*>*)BoomsTree.GetNode(vec2{x,y})->data;
            node->push_back(this);
        }
    }
}

void Boom::Shoot()
{
    if(ballState==BALL_SHOOT)
    {
        shootTime+=RenderTime;
        shootTime=min(shootTime,BoomShootTime);
        vec2 tarPos=pos.mix(ShootPos,shootTime*1.0/BoomShootTime);
        if(CheckIsOverEdge(tarPos)==1)ballState=BALL_STOP;
        else pos=tarPos;
        if(shootTime==BoomShootTime)ballState=BALL_STOP;
    }
}

Boom Boom::Spit(const vec2&dir)
{
    static const GLfloat minusDis=1;
    weight/=2;
    float dis=GetRadian()*2+minusDis;
    vec2 tarpos=pos+dir*(GetRadian()*2+minusDis);
    if(CheckIsOverEdge(tarpos))tarpos=GetValidPos(tarpos,dir,dis);
    Boom ret(pos,weight/2);
    ret.ballState=BALL_SHOOT;
    ret.ShootPos=tarpos;
    return ret;
}
