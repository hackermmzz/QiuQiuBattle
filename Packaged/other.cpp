#include "other.h"
int random(int a, int b)
{
    static std::uniform_int_distribution<int> randomInt(0,INT_MAX); // 随机浮点数，范围0.0 - 1.0
    static std::default_random_engine generator(time(0));
    auto f=randomInt(generator);
    return f%(b-a+1)+a;
}

float randomF()
{
    static std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0
    static std::default_random_engine generator(time(0));
    auto f=randomFloats(generator);
    return f;
}

float randomF(float a, float b)
{
    static std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // 随机浮点数，范围0.0 - 1.0
    static std::default_random_engine generator(time(0));
    auto f=randomFloats(generator);
    return a*f+b*(1.0-f);
}
