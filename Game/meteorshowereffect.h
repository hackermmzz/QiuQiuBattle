#ifndef METEORSHOWEREFFECT_H
#define METEORSHOWEREFFECT_H
#include"ball.h"
#include"effect.h"
struct MeteorShowerEffect:Effect
{
    struct Meteor{
        vec2 pos;
        float type;
        float size;
    };
public:
    MeteorShowerEffect();

    // Effect interface
public:
    void Render(void *ball);
    void *Copy();
};

#endif // METEORSHOWEREFFECT_H
