#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include"include.h"
#include"Shaderprogram.h"
#include"VertexArrayObj.h"
#include"VertexBufferObj.h"
struct ParticleSystem{
    VertexArrayObj*vao[2];
    VertexBufferObj*vbo[2];
    GLuint tfbo[2];
    GLuint laucherCnt;
    bool isInit;
    uint idx;
    ///////////////////////////
    ParticleSystem(VertexArrayObj*vao[2],VertexBufferObj*vbo[2],GLuint laucherCnt);
    void Update(GLenum mode);
    void Draw(GLenum mode);
    VertexBufferObj*GetRenderVbo();
    ~ParticleSystem();
};

#endif // PARTICLESYSTEM_H
