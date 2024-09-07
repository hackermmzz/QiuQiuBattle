#ifndef AI_H
#define AI_H
#include"role.h"
#include"Packaged/timer.h"
#include <utility>
#include"controller.h"
using namespace std;
struct Ai
{
    Role*role;
    bool avoidWhenEnemyCome;
    bool shootWhenAbleToEatEnemy;
    bool NeedUpdateDir;
    vec2 dir;
    uint time;
    //////////////////////
    Ai(Role&role);
    void move();
};

#endif // AI_H
