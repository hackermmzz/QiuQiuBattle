#include "ai.h"

Ai::Ai(Role &role_)
{
    role=&role_;
    time=0;
    NeedUpdateDir=avoidWhenEnemyCome=shootWhenAbleToEatEnemy=0;
}

void Ai::move()
{
    static const uint AvoidWeight=3;
    static const uint minTime=3*1000;
    static const uint maxTime=10*1000;
    if(time==0||NeedUpdateDir){
        avoidWhenEnemyCome=random(1,AvoidWeight)==1;
        NeedUpdateDir=0;
        //生成朝一个方向运动的时间
        time=random(minTime,maxTime);
        //生成方向
        dir=vec2{randomF(-1.0,1.0),randomF(-1.0,1.0)};
        while(dir.x==0.0&&dir.y==0.0)dir=vec2{randomF(-1.0,1.0),randomF(-1.0,1.0)};
        dir.normalize();

    }
    time-=RenderTime;
    auto&&ret=Controller::GetRoleViewSizeAndViewCenter(*role);
    vec2 viewSize=ret.first;
    vec2 viewCenter=ret.second;
    //更新主角的移动方向
    auto CrossHairWorldPos=dir*ViewAreaSize*0.5+viewCenter;
    for(auto&ball:role->balls)
    {
        ball.moveDir=(CrossHairWorldPos-ball.pos).normalize();
    }
    function<void(Ball&)>MoveFun=[&](Ball&ball)->void{
        vec2 tarPos=ball.pos+ball.moveDir*(ball.GetMaxSpeed()*RenderTime/1000.0);
        if(CheckIsOverEdge(tarPos)==0)ball.pos=tarPos;
        else NeedUpdateDir=1;
    };
    for(auto&ball:role->balls)ball.Move(MoveFun);
    if(avoidWhenEnemyCome)
    {
        //这里假设敌人只有一个球
        static const float bonusRad=10.0;//勘测该额外范围下是否有可以吃了自己球的敌人
        static const float BallChangeDirFac=1.1;
        Ball&mainBall=role->balls.front();
        auto r=mainBall.GetRadian()+bonusRad;
        auto r_pow=r*r;
        auto weightThreold=mainBall.weight*BallCanEatBallWeightTime;
        float xl=mainBall.pos.x-r,xr=mainBall.pos.x+r;
        float yt=mainBall.pos.y+r,yb=mainBall.pos.y-r;
        auto&&ret=GetTreeTwoNodes(AllBalls,xl,xr,yt,yb);
        static const float mb=AllBalls.MinBlockSize.x;
        static const float mh=mb/2;
        float xb=ret.first->pos.x+mh,xe=ret.second->pos.x+mh;
        yt=ret.first->pos.y-mh,yb=ret.second->pos.y-mh;
        vec2 tarDir=dir;
        bool flag=0;
        for(auto x=xb;x<=xe;x+=mb)
        {
            for(auto y=yt;y>=yb;y-=mb)
            {
                auto&BallV=*(vector<Ball*>*)AllBalls.GetNode(vec2{x,y})->data;
                for(auto*ball:BallV)
                {
                    vec2 off=ball->pos-mainBall.pos;
                    if(off.x*off.x+off.y*off.y>r_pow)continue;
                    if(ball->weight>=weightThreold)
                    {
                        tarDir=tarDir+(mainBall.pos-ball->pos)*BallChangeDirFac;
                        flag=1;
                    }
                }
            }
        }
        if(flag){
            dir=tarDir;
            dir=dir.normalize();
            time=0;
            NeedUpdateDir=0;
            avoidWhenEnemyCome=random(1,AvoidWeight)==1;
        }
    }
}

