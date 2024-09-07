#ifndef EFFECTS_H
#define EFFECTS_H
#include"globalsource.h"
#include"memory"
using namespace std;
struct Effect
{
    void*data;//data必须是new出来的
    static Texture2D*RandomSampler;
    ////////////////////
    Effect();
    virtual void Render(void*ball);
    virtual void*Copy();
    ~Effect();
};

#endif //
