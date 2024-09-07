#include "particlesystem.h"

ParticleSystem::ParticleSystem(VertexArrayObj *vao_[], VertexBufferObj *vbo_[], GLuint laucherCnt_)
{
    laucherCnt=laucherCnt_;
    isInit=0;
    idx=0;
    for(int i=0;i<2;++i)
    {
        vao[i]=vao_[i];
        vbo[i]=vbo_[i];
        openglAPIPointer->glGenTransformFeedbacks(1,&tfbo[i]);
        openglAPIPointer->glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,tfbo[i]);
        openglAPIPointer->glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,vbo_[(i+1)&1]->id);
    }
}

void ParticleSystem::Update(GLenum mode)
{
    uint tfbo_id=tfbo[idx];
    openglAPIPointer->glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,tfbo_id);
    openglAPIPointer->glEnable(GL_RASTERIZER_DISCARD);
    openglAPIPointer->glBeginTransformFeedback(mode);
    vao[idx]->bind();
    if (!isInit)
    {
        isInit = 1;
        openglAPIPointer->glDrawArrays(mode, 0, laucherCnt);
    }
    else
       Draw(mode);
    idx = (idx + 1) & 1;
    openglAPIPointer->glEndTransformFeedback();
    openglAPIPointer->glDisable(GL_RASTERIZER_DISCARD);
}

void ParticleSystem::Draw(GLenum mode)
{
    openglAPIPointer->glDrawTransformFeedback(mode, tfbo[(idx+1)&1]);
}

VertexBufferObj *ParticleSystem::GetRenderVbo()
{
    return vbo[idx];
}

ParticleSystem::~ParticleSystem()
{
    for(auto*v:vao)delete v;
    for(auto*v:vbo)delete v;
   openglAPIPointer->glDeleteTransformFeedbacks(2, tfbo);
}
