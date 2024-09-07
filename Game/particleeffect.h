#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H
#include"effect.h"
#include"ball.h"
struct ParticleEffect:Effect
{
    /////////////////////////
    ParticleEffect();
    ///////////////////////////////////////////////////
    struct Particle{
        vec2 pos;
        vec2 tarPos;
        vec3 color;
        float ShootTime;
        float TotalTime;
        vec2 initPos;
    };
    // Effect interface
public:
    void Render(void*ball);

    // Effect interface
public:
    void *Copy();
};

#endif // PARTICLEEFFECT_H
