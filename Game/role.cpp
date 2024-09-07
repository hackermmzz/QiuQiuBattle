#include "role.h"

Role::Role(GLuint ballApp, GLuint sporeApp, GLuint effectIdx, const vec2 &initPos)
{
    ballApperance=ballApp;
    sporeApperance=sporeApp;
    EffectIdx=effectIdx;
    auto ball=Ball(ballApperance,sporeApperance,EffectIdx,initPos);
    ball.weight=BallInitWeight;
    ball.master=this;
    balls.push_front(ball);
    data=0;
}

bool Role::Spit()
{
    bool flag=0;
    for(auto&ball:balls)
    {
        auto&&spore=ball.Spit();
        if(spore.appearance==-1)continue;
        auto*node=AllMoveSpores.GetNode(spore.CurPos);
        auto*data=(list<Spore>*)(node->data);
        data->push_back(spore);
        flag=1;
    }
    return flag;
}

bool Role::Shoot()
{
    bool flag=0;
    static vector<Ball*>AllBalls;
    AllBalls.clear();
    for(auto&ball:balls)AllBalls.emplace_back(&ball);
    sort(AllBalls.begin(),AllBalls.end(),[&](Ball*a,Ball*b)
    {
       return a->weight>b->weight;
    });
    int size=AllBalls.size();
    int shootCnt=min(RoleMaxBalls-size,uint(size));
    for(int i=0;i<shootCnt;++i)
    {
        auto&ball=*(AllBalls[i]);
        auto&&shoot=ball.Shoot();
        if(shoot.ballAppearance!=-1)
        {
            balls.push_back(shoot);
            flag=1;
        }
    }
    return flag;
}

void Role::AddToTree()
{
    static const float MapHalfSize=MapSize/2;
    static const auto esp=std::numeric_limits<float>::epsilon();
    static const auto BlockMinSize=MapSize/pow(2,BallAndSporeTreeDeep-1);
    static const auto HalfBlockSize=BlockMinSize/2;
    for(auto&ball:balls)
    {
        auto&p=ball.pos;
        auto r=ball.GetRadian();
        vec2 lt=vec2{p.x-r,p.y+r},rb=vec2{p.x+r,p.y-r};
        lt.x=min(MapHalfSize-esp,max(-MapHalfSize+esp,lt.x));
        lt.y=min(MapHalfSize-esp,max(-MapHalfSize+esp,lt.y));
        rb.x=min(MapHalfSize-esp,max(-MapHalfSize+esp,rb.x));
        rb.y=min(MapHalfSize-esp,max(-MapHalfSize+esp,rb.y));
        auto*node0=AllBalls.GetNode(lt),*node1=AllBalls.GetNode(rb);
        float xb=node0->pos.x+HalfBlockSize,xe=node1->pos.x+HalfBlockSize;
        float yb=node0->pos.y-HalfBlockSize,ye=node1->pos.y-HalfBlockSize;
        for(;xb<=xe;xb+=BlockMinSize)
        {
            for(float y=yb;y>=ye;y-=BlockMinSize)
            {
                auto*node=AllBalls.GetNode(vec2{xb,y});
                auto&v=*((vector<Ball*>*)(node->data));
                v.push_back(&ball);
            }
        }
    }
}

Role::~Role()
{

}
