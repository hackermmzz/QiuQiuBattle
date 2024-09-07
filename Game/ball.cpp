#include "ball.h"

Ball::Ball(GLuint ballApp, GLuint sporeApp,GLuint effectIdx,const vec2 &initPos)
{
    ballAppearance=ballApp;
    sporeAppearance=sporeApp;
    EffectIdx=effectIdx;
    pos=initPos;
    ballState=BALL_STOP;
    aliveTime=0;
    shootTime=0;
    IsEffectInit=0;
}

void Ball::Move(function<void(Ball&)>&fun)
{
    ////////用于判断是否合并
    aliveTime+=RenderTime;
    if(ballState==BALL_SHOOT)
    {
        shootTime+=RenderTime;
        shootTime=min(shootTime,BallShootTime);
        vec2 tarPos=pos+ShootDir*(ShootDis*RenderTime/BallShootTime);
        if(CheckIsOverEdge(tarPos)==0)pos=tarPos;
        else ballState=BALL_STOP;
        if(shootTime==BallShootTime)
            ballState=BALL_STOP;
    }
    fun(*this);
}

void Ball::Render()
{
    static VertexArrayObj*vao=0;
    static const GLuint vertex_cnt=30;
    static const GLuint element_cnt=(vertex_cnt-2)*3;
    if(!vao)
    {
        //////////////////////////////生成圆的顶点坐标
        vector<vec2>vertex(vertex_cnt);
        vector<tuple<GLuint,GLuint,GLuint>>indices((vertex_cnt-2));
        for(int i=0;i<vertex_cnt;++i)vertex[i]=vec2{cos(i*2*PI/vertex_cnt),sin(i*2*PI/vertex_cnt)};
        for(int i=0;i<vertex_cnt-2;++i)indices[i]=tuple<GLuint,GLuint,GLuint>{0,i+1,i+2};
        //////////////////////////////
        vao=new VertexArrayObj;
        VertexBufferObj*vbo=new VertexBufferObj(vertex.size()*sizeof(vec2),vertex.data());
        vbo->setpoint(0,GL_FLOAT,2,sizeof(vec2),0);
        ElementBufferObj*ebo=new ElementBufferObj(indices.size()*sizeof(GLuint)*3,indices.data());
        vao->unbind();
    }
    /////////////////绘制表面
    struct vec4{
      float x,y,z,w;
    };
    BallSurFaceRenderProgram->bind();
    vao->bind();
    BallSurFaceRenderProgram->setuint("sampler",0);
    BallTypes->bind(0);
    vec4 data{pos.x,pos.y,GetRadian(),ballAppearance};
    BallSurFaceRenderProgram->setvec4("data",(float*)&data);
    openglAPIPointer->glDrawElements(GL_TRIANGLES,element_cnt,GL_UNSIGNED_INT,0);
    //////////////////绘制特效
    if(sporeAppearance!=-1&&weight>=BallCanShowEffectMinWeight){
        if(IsEffectInit==0){
            effect=((Effect*)AllEffects[EffectIdx])->Copy();
            IsEffectInit=1;
        }
        ((Effect*)effect)->Render(this);
    }
}

Ball Ball::Shoot()
{
    //如果分身出的球的质量小于10那么没法分身
    if(weight/2<BallMinWeight){
        Ball ret(0,0,0,pos);
        ret.ballAppearance=-1;
        return ret;
    }
    vec2 dir=moveDir;
    while(dir.x==0.0&&dir.y==0.0)dir=vec2{randomF(-1.0,1.0),randomF(-1.0,1.0)}.normalize();
    return Shoot(weight/2,dir);
}

Ball Ball::Shoot(GLuint w, const vec2 &dir)
{
    weight-=w;
    aliveTime=0;//重置存活时间
    Ball ret(ballAppearance,sporeAppearance,EffectIdx,pos);
    ret.master=master;
    ret.weight=w;
    ret.ballState=BALL_SHOOT;
    ret.ShootDir=dir;
    //特判一下,如果w==weight那么不需要获取俩次半径
    if((w<<1)==weight)ret.ShootDis=GetRadian()+BallShootDistance;
    else ret.ShootDis=GetRadian()+ret.GetRadian()+BallShootDistance;
    ret.shootTime=0;
    return ret;
}

Spore Ball::Spit()
{
    static const auto off=0.5;
    static const auto SpitThreold=SporeSingleWeight<<1;
    if(weight<=SpitThreold)return Spore(vec2{},vec2{},-1);
    weight-=SporeSingleWeight;
    auto dir=moveDir;
    while(dir.x==0.0&&dir.y==0.0)dir=vec2{randomF()*2.0-1.0,randomF()*2.0-1.0}.normalize();
    return Spore(dir*(GetRadian()+off)+pos,dir,sporeAppearance);
}

float Ball::GetRadian()
{
   return GetTargetWeightRadian(weight);
}

float Ball::GetMaxSpeed()
{
    static const double k=-1.0*(BallMaxSpeed-BallMinSpeed)/WeightWin;
    float res=k*weight+BallMaxSpeed;
    res=fmax(res,BallMinSpeed);
    return res;
}

uint Ball::CheckBallState()
{
    //0 当前位置未碰撞任何球
    return 0;
}

uint Ball::GetMergeTime()
{
    static const uint minTime=6*1000;
    static const auto fac=1.0/9;
    float rate=weight*1.0/BallInitWeight;
    return minTime+fmax(0.0,(fac*rate*rate-1)*minTime);
}

uint Ball::LoseWeight()
{
    static const float fac=0.01;
   return weight*fac;
}

Ball::~Ball()
{
    if(IsEffectInit)delete effect;
}


