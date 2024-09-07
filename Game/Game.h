#ifndef GAME_H
#define GAME_H
#include"Packaged/include.h"
#include"Packaged/texture2d.h"
#include"Packaged/Shaderprogram.h"
#include"globalsource.h"
#include"iostream"
#include"map.h"
#include"controller.h"
#include"actionbar.h"
#include"set"
#include"qlabel.h"
#include"qsound.h"
#include"effect.h"
#include"boom.h"
#include"qmediaplayer.h"
#include"ai.h"
#include"meteorshowereffect.h"
#include"particleeffect.h"
#include"qmessagebox.h"
#include"vortexeffect.h"
//////////////////////////////////////////
extern QLabel*Score;
extern QLabel*Fps;
extern Map*WorldMap;
extern Controller*controller;
extern vector<Role*>AllRoles;
extern vector<Boom*>AllBooms;
extern ActionBar actionBar;
/////////////////////////////////////////
using BallTreeType=vector<Ball*>;
using SporeTreeType=list<Spore>;
using BoomTreeType=vector<Boom*>;
//////////////////////////////////////////
void GameInit();
void GameUpdate();
void GameRender();
vector<Ball*>& GetNeedRenderBalls();
vector<Boom*>& GetNeedRenderBooms();
void RenderBall(vector<Ball*>&NeedRenderedBalls,vector<Boom*>&NeedRenderedBooms);
void RenderSpore();
void BallEatingFood();
void BallEatingSpore();
void BallEatingBoom();
void BallEatingBall();
void SporeUpdate();
void RoleMove(Role&role);
void BoomMove();
void BallLoseWeight();
void InitAllProgram();
GLuint RandomEffect();
tuple<Texture2D*,uint,uint> LoadAllImgsToOnlyOneTexture(const vector<string>&path);
#endif // GAME_H
