#include "meteorshowereffect.h"

MeteorShowerEffect::MeteorShowerEffect()
{

}

void MeteorShowerEffect::Render(void *ball)
{
    static ShaderProgram*renderProgram=0;
    static ShaderProgram*transformProgram=0;
    if(!renderProgram)
    {
        renderProgram=new ShaderProgram("src/Shader/MeteorShowerRender.vert","src/Shader/MeteorShowerRender.frag");
        transformProgram=new ShaderProgram("src/Shader/MeteorShowerTransform.vert","src/Shader/MeteorShowerTransform.geom","",{"pos","type","size"});
    }
    //////////////////////////////////开始绘制
    //更新点精灵
    ParticleSystem*system=(ParticleSystem*)data;
    transformProgram->bind();
    transformProgram->setuint("random",0);
    Effect::RandomSampler->bind(0);
    transformProgram->setfloat("Time",RenderTime);
    transformProgram->setfloat("fac",randomF());
    system->Update(GL_POINTS);
    //绘制
    auto*vbo=system->GetRenderVbo();
    openglAPIPointer->glBindVertexArray(0);
    vbo->bind();
    vbo->setpoint(0,GL_FLOAT,2,sizeof(Meteor),0);
    vbo->setpoint(1,GL_FLOAT,1,sizeof(Meteor),sizeof(vec3));
    renderProgram->bind();
    renderProgram->setvec2("ViewAreaSize",ViewAreaSize);
    renderProgram->setvec2("ViewCenter",ViewCenter);
    renderProgram->setvec2("BallPos",((Ball*)ball)->pos);
    renderProgram->setfloat("BallRadian",((Ball*)ball)->GetRadian());
    system->Draw(GL_POINTS);
}

void *MeteorShowerEffect::Copy()
{
    const static uint MeteorLauncherCnt=6;
    const static uint MeteorMaxCnt=1000;
    MeteorShowerEffect*ret=new MeteorShowerEffect;
    //////////////////////////////
    ParticleSystem*particleSystem;
    VertexArrayObj* vao[2];
    VertexBufferObj* vbo[2];
    /////////////////////////////////////////初始化数据
    vector<Meteor>meteorLauncher(MeteorLauncherCnt);
    for(int i=0;i<MeteorLauncherCnt;++i)
    {
        auto&d=meteorLauncher[i];
        d.pos=vec2{MapSize*100.0,MapSize*100.0};
        d.type=0;//0表示流星产生器
        d.size=0.0;//立刻开始产生流星雨
    }
    for(int i=0;i<2;++i)
    {
        vao[i]=new VertexArrayObj;
        vbo[i]=new VertexBufferObj(sizeof(Meteor)*(MeteorMaxCnt+MeteorLauncherCnt),0);
        if(i==0)vbo[i]->setdata(0,sizeof(Meteor)*MeteorLauncherCnt,meteorLauncher.data());
        vbo[i]->setpoint(0,GL_FLOAT,4,sizeof(Meteor),0);
        vao[i]->unbind();
    }
    particleSystem=new ParticleSystem(vao,vbo,MeteorLauncherCnt);
    ret->data=particleSystem;
    /////////////////////////////////
    return ret;
}
