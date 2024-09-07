#ifndef BOOM_H
#define BOOM_H
#include"globalsource.h"
#include"ball.h"
struct Boom:Ball
{
    vec2 ShootPos;
    ////////////////////
    Boom(const vec2&pos,GLuint weight);
    void AddToTree();
    void Shoot();
    Boom Spit(const vec2&dir);
};

#endif // BOOM_H
