#ifndef ACTIONBAR_H
#define ACTIONBAR_H
#include"globalsource.h"
struct ActionBar
{
    ////////////////////////////
    GLfloat offset;
    GLfloat scale0,scale1;
    vec2 barOffset0,barOffset1;
    bool isclicked;
    vec2 dir;
    GLfloat extent;
    ////////////////////////////
    ActionBar();
    void Render();
    void SetClicked(GLfloat x,GLfloat y);
    void MoveBar(GLfloat x,GLfloat y);
    void ResetBarPos();
    ~ActionBar();
};

#endif // ACTIONBAR_H
