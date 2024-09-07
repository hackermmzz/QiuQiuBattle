#include"globalsource.h"
#include"QMainWindow"
const uint GameFps=90;
const GLuint MapSize=512;
const GLfloat FoodWeight=0.03;
const uint FoodResumeMinTime=3*1000;
const uint FoodResumeMaxTime=5*1000;
const GLfloat FoodSingleScore=1;
GLuint BufferBindingBase=0;
const GLuint FoodMaxSides=10;
vec2 ViewAreaSize;
vec2 ViewCenter;
GLuint RenderTime=0;
const GLuint FoodTreeDeep=5;
GLuint SporeSingleWeight=10;
const GLuint SporeShootDistance=10;
const GLuint SporeShootTime=520;
const GLfloat BallDensity=1;
const GLfloat BallInitWeight=20;
const GLuint RoleMaxBalls=16;
const GLfloat BallShootDistance=5;
const GLuint BallShootTime=100;
const GLuint BallMinWeight=10;
const GLuint BallAndSporeTreeDeep=6;
const GLuint RolesMaxCount=100;
Tree<void*>AllBalls(BallAndSporeTreeDeep,MapSize,MapSize);
Tree<void*>AllStopSpores(BallAndSporeTreeDeep,MapSize,MapSize);
Tree<void*>AllMoveSpores(BallAndSporeTreeDeep,MapSize,MapSize);
Texture2D*SporeTypes;
uint SporeTypesCnt;
uint SporeTypesRealCnt;
Texture2D*BallTypes;
uint BallTypesCnt;
uint BallTypesRealCnt;
const uint BallCanEatSporeMinWeight=SporeSingleWeight+5;
const float BallCanEatBallWeightTime=1.2;
const GLuint BoomMinWeight=100;
const GLuint BoomMaxWeight=150;
const GLuint BoomInitCnt=40;
const GLuint BoomMaxCnt=100;
const GLuint BoomTreeDeep=6;
Tree<void*>BoomsTree(BoomTreeDeep,MapSize,MapSize);
const GLuint BoomShootTime=200;
const GLuint BoomShootDis=5;
const GLfloat BallCanEatBoomWeightTime=1.2;
const GLuint WeightWin=uint(2e4);
unsigned long long GlobalClock=0;
const uint BallWeightLoseIntervals=1000;
extern const uint BallMaxSpeed=15;
extern const uint BallMinSpeed=2;
bool IsGameFinish=0;
const GLuint BallCanShowEffectMinWeight=200;
vector<void*>AllEffects;
ShaderProgram*BallSurFaceRenderProgram=0;
//////////////////////////////////////////////
bool CheckIsOverEdge(const vec2 &v)
{
    static const float MapHalfSize=MapSize/2;
    static const auto esp=std::numeric_limits<float>::epsilon();
    if(v.x-esp<=-MapHalfSize)return 1;
    if(v.x+esp>=MapHalfSize)return 1;
    if(v.y-esp<=-MapHalfSize)return 1;
    if(v.y+esp>=MapHalfSize)return 1;
    return 0;
}

vec2 GetValidPos(const vec2 &p, const vec2 &dir,GLfloat dis)
{
    static const float MapHalfSize=MapSize/2;
    static const auto esp=std::numeric_limits<float>::epsilon();
    auto tarPy=dis*dir.y+p.y;
    if(tarPy+esp>MapHalfSize)
        dis=(MapHalfSize-p.y-esp)/dir.y;
    else if(tarPy-esp<-MapHalfSize)
        dis=(esp-MapHalfSize-p.y)/dir.y;
    auto tarPx=dis*dir.x+p.x;
    if(tarPx+esp>MapHalfSize)
        dis=(MapHalfSize-p.x-esp)/dir.x;
    else if(tarPx-esp<-MapHalfSize)
        dis=(esp-MapHalfSize-p.x)/dir.x;
    return p+dir*dis;
}

float GetTargetWeightRadian(GLuint weight)
{
     return sqrt(weight/(PI));
}

