#ifndef BALL_H
#define BALL_H
#include"globalsource.h"
#include"spore.h"
#include"functional"
#include"effect.h"
enum{
  BALL_STOP,
  BALL_SHOOT,
};
struct Ball
{
    GLuint weight;
    vec2 pos;
    vec2 ShootDir;
    float ShootDis;
    vec2 moveDir;
    GLuint ballAppearance;
    GLuint sporeAppearance;
    GLuint ballState;
    GLuint shootTime;
    GLuint aliveTime;
    bool IsEffectInit;
    GLuint EffectIdx;
    void*effect;
    void*master;
    /////////////////////////////
    Ball(GLuint,GLuint,GLuint,const vec2&);
    void Move(function<void(Ball&)>&fun);
    void Render();
    Ball Shoot();
    Ball Shoot(GLuint w,const vec2&dir);
    Spore Spit();
    float GetRadian();
    float GetMaxSpeed();
    uint CheckBallState();
    uint GetMergeTime();
    uint LoseWeight();
    ~Ball();
};

#endif // BALL_H
