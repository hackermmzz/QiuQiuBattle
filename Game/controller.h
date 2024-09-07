#ifndef CONTROLLER_H
#define CONTROLLER_H
#include"globalsource.h"
#include"role.h"
#include"actionbar.h"
using namespace std;
struct Controller
{
    vec2 CrossHairPos;
    Role role;
    ///////////////////////////////
    Controller(const vec2&initPos,GLuint,GLuint,GLuint);
    void SetViewArea();
    static pair<vec2,vec2>GetRoleViewSizeAndViewCenter(Role&role);
    void SetCrossHairPos(ActionBar&bar);
    void Move(ActionBar&bar);
    void RenderCrossHair();
    ~Controller();
};

#endif // CONTROLLER_H
