#ifndef VORTEXEFFECT_H
#define VORTEXEFFECT_H
#include"effect.h"
#include"ball.h"
struct VortexEffect:Effect
{
    struct Particle{
        vec2 pos;
        vec3 color;
        float r;
        float degree;
    };
    ///////////////////////////
    VortexEffect();

    // Effect interface
public:
    void Render(void *ball);
    void *Copy();
};

#endif // VORTEXEFFECT_H
