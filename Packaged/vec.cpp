#include "vec.h"


vec2 vec2::operator+(const vec2 &v) const{
    return vec2{x+v.x,y+v.y};
}

vec2 vec2::operator -(const vec2 &v) const{
    return vec2{x-v.x,y-v.y};
}

vec2 vec2::operator *(GLfloat v) const
{
    return vec2{x*v,y*v};
}

vec2 vec2::operator /(const vec2 &v) const
{
    return vec2{x/v.x,y/v.y};
}

vec2 vec2::operator *(const vec2 &v) const
{
    return vec2{x*v.x,y*v.y};
}

vec2 vec2::mix(const vec2 &v, float weight) const
{
    return vec2{x*(1.0-weight)+v.x*weight,y*(1.0-weight)+v.y*weight};
}

vec2 vec2::normalize() const
{
    float dis=sqrt(x*x+y*y);
    if(dis==0)return vec2{0,0};
    return vec2{x/dis,y/dis};
}
