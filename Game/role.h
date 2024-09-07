#ifndef ROLE_H
#define ROLE_H
#include"ball.h"
#include"list"
struct Role
{
    list<Ball>balls;
    uint ballApperance;
    uint sporeApperance;
    GLuint EffectIdx;
    void*data;//用于存储数据指针
    ///////////////////////////
    Role(GLuint,GLuint,GLuint,const vec2&);
    bool Spit();
    bool Shoot();
    void AddToTree();
    ~Role();
};
#endif // ROLE_H
