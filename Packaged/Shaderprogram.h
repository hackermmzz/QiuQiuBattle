#ifndef PROGRAM_H
#define PROGRAM_H
#include"string"
#include"include.h"
#include"fstream"
#include"qdir.h"
#include"qfile.h"
#include"qdatetime.h"
#include"vec.h"
using namespace std;
class ShaderProgram
{
private:
    GLuint id;
public:
    ShaderProgram(const string&vertexShader,const string&fragmentShader);
    ShaderProgram(const string&vertexShader,const string&geomryShader,const string&fragmentShader,const vector<string>&varyings);
    int GetLocation(const char*name);
    void setuint(const char*id,uint val);
    void setfloat(const char*id,float val);
    void setvec2(const char*name,const vec2&val);
    void setvec3(const char*name,const vec3&val);
    void setvec4(const char*name,const GLfloat*val);
    void setSsbo(const char*id,uint idx);
    void bind();
    ~ShaderProgram();
};

#endif // PROGRAM_H
