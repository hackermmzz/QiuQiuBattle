#include "Shaderprogram.h"

ShaderProgram::ShaderProgram(const string &vertexShader, const string &fragmentShader):ShaderProgram(vertexShader,"",fragmentShader,{})
{


}

ShaderProgram::ShaderProgram(const string &vertexShader, const string &geomryShader, const string &fragmentShader, const vector<string>&varyings)
{
    auto read=[&](const string&file_)->string{
        QFile file(file_.c_str());
        file.open(QIODevice::ReadOnly);
        string res;
        res.resize(file.size());
        file.read(&res[0],res.size());
        return res;
    };
    string data0;
    if(vertexShader!="")
    data0=read(vertexShader);
    string data1;
    if(geomryShader!="")data1=read(geomryShader);
    string data2;
    if(fragmentShader!="")
        data2=read(fragmentShader);
    ///////////////编译报错
    auto GetShaderError=[&](GLuint id,const string&file)->void{
            int success;
            openglAPIPointer->glGetShaderiv(id, GL_COMPILE_STATUS, &success);
            if (success)
                return;
            QString SaveToFilePath=QDir::currentPath()+QString("/ShaderCompileError.txt");
             QFile f(SaveToFilePath);
             f.open(QIODevice::Append);
             char*info=new char[1024*1024*2+1];//2MB
            openglAPIPointer->glGetShaderInfoLog(id, 1024*1024*2, 0, info);
            strcat(info,file.c_str());
            strcat(info, "\n--------------------------------------\n");
            char*date=QDateTime::currentDateTime().toString("hh:mm:ss").toUtf8().data();
            f.write(date,strlen(date));
            f.write("\n",1);
           f.write(info,strlen(info));
            delete[]info;
    };
    ///////////////编译shader
    auto CompileShader=[&](GLenum type,const string&code,const string&file)->GLuint{
        auto id=openglAPIPointer->glCreateShader(type);
        auto*data=code.data();
        openglAPIPointer->glShaderSource(id,1,&data,0);
        openglAPIPointer->glCompileShader(id);
        GetShaderError(id,file);
        return id;
    };
    GLuint id0=-1,id1=-1,id2=-1;
    if(vertexShader!="")
        id0=CompileShader(GL_VERTEX_SHADER,data0,vertexShader);
    if(geomryShader!="")
        id1=CompileShader(GL_GEOMETRY_SHADER,data1,geomryShader);
    if(fragmentShader!="")
        id2=CompileShader(GL_FRAGMENT_SHADER,data2,fragmentShader);
    ///////////////生成着色器程序
    auto Geterror=[&](GLuint id){
        int status = 0;
        openglAPIPointer->glGetProgramiv(id, GL_LINK_STATUS, &status);
         if (status == GL_FALSE) {
          QString SaveToFilePath=QDir::currentPath()+QString("\\ShaderCompileError.txt");
           QFile f(SaveToFilePath);
          f.open(QIODevice::Append);
           char *info=new char[1024*1024*2+1];
           openglAPIPointer->glGetProgramInfoLog(id, 1024*1024, 0, info);
           strcat(info, "Program");
           strcat(info, "\n--------------------------------------\n");
           char*date=QDateTime::currentDateTime().toString("hh:mm:ss").toUtf8().data();
           f.write(date,strlen(date));
           f.write("\n",1);
           f.write(info,strlen(info));
           f.close();
           delete []info;
         }
    };
    id= openglAPIPointer->glCreateProgram();
    if(id0!=-1)
    openglAPIPointer->glAttachShader(id, id0);
    if(id1!=-1)
    openglAPIPointer->glAttachShader(id, id1);
    if(id2!=-1)
    openglAPIPointer->glAttachShader(id,id2);
    if(varyings.size()){
        vector<const char*>v;
        for(auto&s:varyings)v.push_back(s.c_str());
        openglAPIPointer->glTransformFeedbackVaryings(id, varyings.size(),v.data(), GL_INTERLEAVED_ATTRIBS);
    }
    openglAPIPointer->glLinkProgram(id);
    Geterror(id);
    if(id0!=-1)
    openglAPIPointer->glDeleteShader(id0);
    if(id1!=-1)
    openglAPIPointer->glDeleteShader(id1);
    if(id2!=-1)
    openglAPIPointer->glDeleteShader(id2);
    ///
    bind();
    ////////////////
}

int ShaderProgram::GetLocation(const char *name)
{
    int location = openglAPIPointer->glGetUniformLocation(id, name);
    return location;
}

void ShaderProgram::setuint(const char *id, uint val)
{
    int location = GetLocation(id);
    if (location < 0){
        qDebug()<<id<<" not found!";
      return;
    }
    openglAPIPointer->glUniform1ui(location, val);
}

void ShaderProgram::setfloat(const char *id, float val)
{
    int location = GetLocation(id);
    if (location < 0){
        qDebug()<<id<<" not found!";
      return;
    }
    openglAPIPointer->glUniform1f(location, val);
}

void ShaderProgram::setvec2(const char *name, const vec2 &val)
{
    int location = GetLocation(name);
    if (location < 0){
        qDebug()<<name<<" not found!";
      return;
    }
    openglAPIPointer->glUniform2fv(location,1,(GLfloat*)&val);
}

void ShaderProgram::setvec3(const char *name, const vec3 &val)
{
    int location = GetLocation(name);
    if (location < 0){
        qDebug()<<name<<" not found!";
      return;
    }
    openglAPIPointer->glUniform3fv(location,1,(GLfloat*)&val);
}

void ShaderProgram::setvec4(const char *name, const GLfloat *val)
{
    int location = GetLocation(name);
    if (location < 0){
        qDebug()<<name<<" not found!";
      return;
    }
    openglAPIPointer->glUniform4fv(location,1,val);
}

void ShaderProgram::setSsbo(const char *name, uint idx)
{
    GLuint location = openglAPIPointer->glGetProgramResourceIndex(id,GL_SHADER_STORAGE_BLOCK, name);
    if (location < 0){
        qDebug()<<name<<" not found!";
      return;
    }
    openglAPIPointer->glShaderStorageBlockBinding(id,location,idx);
}

void ShaderProgram::bind()
{
    openglAPIPointer->glUseProgram(id);
}

ShaderProgram::~ShaderProgram()
{
    openglAPIPointer->glDeleteProgram(id);
}
