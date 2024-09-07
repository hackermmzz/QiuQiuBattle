#ifndef VEC_H
#define VEC_H
#include"include.h"
struct vec3
{
    GLfloat x,y,z;

};
struct vec2{
    GLfloat x,y;
    vec2 operator+(const vec2&v)const;
    vec2 operator -(const vec2&v)const;
    vec2 operator *(GLfloat v)const;
    vec2 operator /(const vec2&v)const;
    vec2 operator *(const vec2&v)const;
    vec2 mix(const vec2&v,float weight)const;
    vec2 normalize()const;
};
#endif // VEC_H
