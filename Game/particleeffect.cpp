#include "particleeffect.h"

ParticleEffect::ParticleEffect():Effect()
{

}

void ParticleEffect::Render(void*ball)
{
    static ShaderProgram*renderProgram=0;
    static ShaderProgram*transformProgram=0;
    if(!renderProgram)
    {
        renderProgram=new ShaderProgram("src/Shader/ParticleRender.vert","src/Shader/ParticleRender.frag");
        transformProgram=new ShaderProgram("src/Shader/ParticleTransform.vert","src/Shader/ParticleTransform.geom","",{"pos","tarPos","color","ShootTime","TotalTime","initPos"});
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

void *ParticleEffect::Copy()
{
    const static uint ParticleCnt=100;
    auto*ret=new ParticleEffect();
    ParticleSystem*particleSystem;
    VertexArrayObj* vao[2];
    VertexBufferObj* vbo[2];
    /////////////////////////////////////////初始化数据
    vector<Particle>particle(ParticleCnt);
    for(int i=0;i<ParticleCnt;++i)
    {
        Particle&p=particle[i];
        p.tarPos=p.pos=p.initPos=vec2{randomF(-1.0,1.0),randomF(-1.0,1.0)};
        p.color=vec3{randomF(),randomF(),randomF()};
        p.ShootTime=p.TotalTime=8.0;
    }
    for(int i=0;i<2;++i)
    {
        vao[i]=new VertexArrayObj;
        vbo[i]=new VertexBufferObj(sizeof(Particle)*ParticleCnt,i?0:particle.data());
        vbo[i]->setpoint(0,GL_FLOAT,4,sizeof(Particle),0);
        vbo[i]->setpoint(1,GL_FLOAT,3,sizeof(Particle),offsetof(Particle,Particle::color));
        vbo[i]->setpoint(2,GL_FLOAT,4,sizeof(Particle),offsetof(Particle,Particle::ShootTime));
        vao[i]->unbind();
    }
    particleSystem=new ParticleSystem(vao,vbo,ParticleCnt);
    ret->data=particleSystem;
    ///////////////////////////////////////////////////
    return ret;
}
