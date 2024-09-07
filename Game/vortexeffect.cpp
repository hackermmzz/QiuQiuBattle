#include "vortexeffect.h"
VortexEffect::VortexEffect()
{

}

void VortexEffect::Render(void *ball)
{
    static ShaderProgram*renderProgram=0;
    static ShaderProgram*transformProgram=0;
    if(!renderProgram)
    {
        renderProgram=new ShaderProgram("src/Shader/VortexRender.vert","src/Shader/VortexRender.frag");
        transformProgram=new ShaderProgram("src/Shader/VortexTransform.vert","src/Shader/VortexTransform.geom","",{"pos","color","r","degree"});
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
    vbo->setpoint(0,GL_FLOAT,2,sizeof(Particle),0);
    vbo->setpoint(1,GL_FLOAT,3,sizeof(Particle),offsetof(Particle,Particle::color));
    renderProgram->bind();
    renderProgram->setvec2("ViewAreaSize",ViewAreaSize);
    renderProgram->setvec2("ViewCenter",ViewCenter);
    renderProgram->setvec2("BallPos",((Ball*)ball)->pos);
    renderProgram->setfloat("BallRadian",((Ball*)ball)->GetRadian());
    system->Draw(GL_POINTS);
}

void *VortexEffect::Copy()
{
    const static uint particleCnt=200;
    VortexEffect*ret=new VortexEffect;
    ParticleSystem*particleSystem;
    VertexArrayObj* vao[2];
    VertexBufferObj* vbo[2];
    /////////////////////////////////////////初始化数据
    vector<Particle>particle(particleCnt);
    for(int i=0;i<particleCnt;++i)
    {
        Particle&p=particle[i];
        p.r=randomF(0.5,1.0);
        p.degree=randomF(0,2*PI);
    }
    for(int i=0;i<2;++i)
    {
        vao[i]=new VertexArrayObj;
        vbo[i]=new VertexBufferObj(sizeof(Particle)*particleCnt,i?0:particle.data());
        vbo[i]->setpoint(0,GL_FLOAT,3,sizeof(Particle),offsetof(Particle,Particle::color));
        vbo[i]->setpoint(1,GL_FLOAT,2,sizeof(Particle),offsetof(Particle,Particle::r));
        vao[i]->unbind();
    }
    particleSystem=new ParticleSystem(vao,vbo,particleCnt);
    ret->data=particleSystem;
    ///////////////////////////////////////////////////
    return ret;
}
