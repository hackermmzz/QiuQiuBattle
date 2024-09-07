#ifndef INCLUDE_H
#define INCLUDE_H
/////
#include<qopenglwidget.h>
#include<qopenglfunctions_4_5_core.h>
#include"QDebug"
//////////////
#define OPENGLAPIVERSION QOpenGLFunctions_4_5_Core
extern OPENGLAPIVERSION*openglAPIPointer;
void InitOpenGLAPIPointer(OPENGLAPIVERSION* api_);
#define PI 3.1415926
#endif // INCLUDE_H
