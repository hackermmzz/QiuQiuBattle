#ifndef SPORE_H
#define SPORE_H
#include"globalsource.h"
enum {
  SPORE_STOP,
  SPORE_SHOOT,
};
struct Spore
{
    vec2 CurPos;
    vec2 TarPos;
    GLuint appearance;
    uint shootTime;
    uint state;
    //////////////////////////
    Spore(const vec2&p,const vec2&dir,GLuint appearanceType);
    void Shoot();
    ~Spore();
};

#endif // SPORE_H
