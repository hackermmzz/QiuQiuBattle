#ifndef GLOBAL_SOURCE
#define GLOBAL_SOURCE
#include"Packaged/include.h"
#include"Packaged/texture2d.h"
#include"Packaged/Shaderprogram.h"
#include"Packaged/VertexArrayObj.h"
#include"Packaged/VertexBufferObj.h"
#include"Packaged/ElementBufferObj.h"
#include"Packaged/ShaderStorageBufferObj.h"
#include"Packaged/vec.h"
#include"Packaged/tree.hpp"
#include"Packaged/timer.h"
#include"Packaged/other.h"
#include"Packaged/particlesystem.h"
/////////////////////////////////
extern const uint GameFps;
extern const GLuint MapSize;
extern const GLfloat FoodWeight;
extern const GLfloat FoodSingleScore;
extern const uint FoodResumeMinTime;
extern const uint FoodResumeMaxTime;
extern GLuint BufferBindingBase;
extern const GLuint FoodMaxSides;
extern const GLuint FoodTreeDeep;
extern vec2 ViewAreaSize;
extern vec2 ViewCenter;
extern GLuint RenderTime;
extern GLuint SporeSingleWeight;
extern const GLuint SporeShootDistance;
extern const GLuint SporeShootTime;
extern const GLfloat BallDensity;
extern const GLfloat BallInitWeight;
extern const GLuint RoleMaxBalls;
extern const GLfloat BallShootDistance;
extern const GLuint BallShootTime;
extern const GLuint BallMinWeight;
extern const GLuint BallAndSporeTreeDeep;
extern const GLuint RolesMaxCount;
extern Tree<void*>AllBalls;
extern Tree<void*>AllStopSpores;
extern Tree<void*>AllMoveSpores;
extern Texture2D*SporeTypes;
extern uint SporeTypesCnt;
extern uint SporeTypesRealCnt;
extern Texture2D*BallTypes;
extern uint BallTypesCnt;
extern uint BallTypesRealCnt;
extern const uint BallCanEatSporeMinWeight;
extern const float BallCanEatBallWeightTime;
extern const GLuint BoomMinWeight;
extern const GLuint BoomMaxWeight;
extern const GLuint BoomInitCnt;
extern const GLuint BoomMaxCnt;
extern Tree<void*>BoomsTree;
extern const GLuint BoomShootTime;
extern const GLuint BoomShootDis;
extern const GLfloat BallCanEatBoomWeightTime;
extern const GLuint WeightWin;
extern unsigned long long GlobalClock;
extern const uint BallWeightLoseIntervals;
extern const uint BallMaxSpeed;
extern const uint BallMinSpeed;
extern bool IsGameFinish;
extern const GLuint BallCanShowEffectMinWeight;
extern vector<void*>AllEffects;
extern ShaderProgram*BallSurFaceRenderProgram;
/////////////////////////////////
//判断物体是否超出边界
bool CheckIsOverEdge(const vec2&v);
//计算合法的目的地
vec2 GetValidPos(const vec2&p,const vec2&dir,GLfloat dis);
//获取指定四叉树上的最左上和最右下俩节点
template<class T>
pair<typename Tree<T>::NodeType*,typename Tree<T>::NodeType*> GetTreeTwoNodes(Tree<T>&tree,GLfloat xl,GLfloat xr,GLfloat yt,GLfloat yb)
{
    static const auto esp=std::numeric_limits<float>::epsilon();
    const static float MapHalfSize=MapSize/2;
    vec2 p0=vec2{xl,yt};
    vec2 p1=vec2{xr,yb};
    p0.x=min(MapHalfSize-esp,max(-MapHalfSize+esp,p0.x));
    p0.y=min(MapHalfSize-esp,max(-MapHalfSize+esp,p0.y));
    p1.x=min(MapHalfSize-esp,max(-MapHalfSize+esp,p1.x));
    p1.y=min(MapHalfSize-esp,max(-MapHalfSize+esp,p1.y));
    return {tree.GetNode(p0),tree.GetNode(p1)};
}
//获取当前视区域的树上俩节点
template<class T>
pair<typename Tree<T>::NodeType*,typename Tree<T>::NodeType*> GetViewAreaTwoNodes(Tree<T>&tree)
{
    return GetTreeTwoNodes(tree,ViewCenter.x-ViewAreaSize.x/2,ViewCenter.x+ViewAreaSize.x/2,ViewCenter.y+ViewAreaSize.y/2,ViewCenter.y-ViewAreaSize.y/2);
}
//获取指定重量下面的半径
float GetTargetWeightRadian(GLuint weight);
/////////////////////////////////
#endif
