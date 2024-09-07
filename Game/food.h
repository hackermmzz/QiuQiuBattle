#ifndef FOOD_H
#define FOOD_H
#include"globalsource.h"
#include"Packaged/vec.h"
#include"Packaged/other.h"
///////////////////
struct Food
{
    vec2 pos;
    GLfloat rotate;
    vec3 color;
    int resume;
};

#endif // FOOD_H
