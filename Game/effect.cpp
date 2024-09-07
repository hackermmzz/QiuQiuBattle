#include "effect.h"
///////////////////////////
Texture2D* Effect::RandomSampler=0;
///////////////////////////
Effect::Effect()
{
    data=0;
    if(!RandomSampler)
    {
        static uint w=1024,h=1024;
        struct Color{
            byte r,g,b,a;
        };
        vector<Color>data(w*h);
        for(uint i=0;i<w*h;++i)
        {
            data[i].r=random(0,255);
        }
        RandomSampler=new Texture2D(w,h,data.data());

    }
}


void Effect::Render(void *ball)
{

}

void *Effect::Copy()
{
    return 0;
}

Effect::~Effect()
{
    if(data)delete data;
}
