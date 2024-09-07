#include "game.h"
//////////////////////////////////
QLabel*Score;
QLabel*Fps;
Map*WorldMap;
Controller*controller;
vector<Role*>AllRoles;
ActionBar actionBar;
vector<Boom*>AllBooms;
//////////////////////////////////
void GameInit()
{
    //////////////////加载所有特效
    AllEffects=vector<void*>{new ParticleEffect(),new MeteorShowerEffect(),new VortexEffect()};
    /////////////////加载所有的球的形象贴图,包括刺球
    auto&&BallTypesRet=LoadAllImgsToOnlyOneTexture({"src/BallType/","src/Boom/"});
    BallTypes=get<0>(BallTypesRet);
    BallTypesRealCnt=get<1>(BallTypesRet);
    BallTypesCnt=get<2>(BallTypesRet);
    //////////////////加载所有孢子的形象贴图
    auto&&sporeLoadRes=LoadAllImgsToOnlyOneTexture({"src/Spore/"});
    SporeTypes=get<0>(sporeLoadRes);
    SporeTypesRealCnt=get<1>(sporeLoadRes);
    SporeTypesCnt=get<2>(sporeLoadRes);
    //////////////////加载所有着色器
    InitAllProgram();
    /////////////////生成地图
    WorldMap=new Map();
    /////////////////生成主角色
    controller=new Controller(vec2{0,0},random(0,BallTypesRealCnt-2),random(0,SporeTypesRealCnt-1),RandomEffect());
    ////////////////生成其他角色
    static const int MapHalfSize=MapSize/2;
    auto RoleRandomPos=[&]{
        static const int fac=1000;
        return random(-MapHalfSize*fac,MapHalfSize*fac)*1.0/fac;
    };
    for(int i=1;i<RolesMaxCount;++i)
    {
        AllRoles.push_back(new Role(random(0,BallTypesRealCnt-2),random(0,SporeTypesRealCnt-1),RandomEffect(),vec2{RoleRandomPos(),RoleRandomPos()}));
    }
    AllRoles.push_back(&controller->role);
    /////////////////生成刺球
    for(int i=0;i<BoomInitCnt;++i)
    {
        static const int mh=MapSize/2;
        AllBooms.push_back(new Boom(vec2{
                                    random(-mh,mh),
                                    random(-mh,mh)
                                    },
                                    random(BoomMinWeight,BoomMaxWeight)));
    }
    /////////////////初始化球的四叉树加速裁剪
    for(uint i=AllBalls.NodeDataBeginIdx;i<AllBalls.NodeDataEndIdx;++i)
        AllBalls.nodes[i].data=(void**)new BallTreeType();
    /////////////////初始化刺球的四叉树
    for(uint i=BoomsTree.NodeDataBeginIdx;i<BoomsTree.NodeDataEndIdx;++i)
        BoomsTree.nodes[i].data=(void**)new BoomTreeType();
    /////////////////初始化孢子的四叉树
    for(uint i=AllStopSpores.NodeDataBeginIdx;i<AllStopSpores.NodeDataEndIdx;++i)AllStopSpores.nodes[i].data=(void**)new SporeTreeType();
    for(uint i=AllMoveSpores.NodeDataBeginIdx;i<AllMoveSpores.NodeDataEndIdx;++i)AllMoveSpores.nodes[i].data=(void**)new SporeTreeType();
    /////////////////播放音乐
    QMediaPlayer*player=new QMediaPlayer();
    player->setMedia(QUrl::fromLocalFile("src/BackGroundMusic/bgm.wav"));
    player->connect(player,&QMediaPlayer::stateChanged,player,[=](QMediaPlayer::State state){
       if(state==QMediaPlayer::StoppedState)
       {
           player->setPosition(0);
           player->play();
       }
    });
    player->connect(player,&QMediaPlayer::positionChanged,player,[player,&IsGameFinish](qint64 pos)
    {
        if(IsGameFinish==1)
        {
            player->pause();
        }
    });
    player->play(); // 播放音乐

}
///////////////////////////////////
void GameUpdate()
{
    //球减少体重
    static Timer*timer=new Timer(BallWeightLoseIntervals,[&](){
        BallLoseWeight();
        timer->Reset();
    });
    timer->start();
    //更新准星坐标
    controller->SetCrossHairPos(actionBar);
    //更新主角的移动方向
    auto CrossHairWorldPos=controller->CrossHairPos*ViewAreaSize*0.5+ViewCenter;
    for(auto&ball:controller->role.balls)
    {
        ball.moveDir=(CrossHairWorldPos-ball.pos).normalize();
    }
    //角色移动
    for(auto*role:AllRoles)
    {
       if(role==&(controller->role))continue;
       RoleMove(*role);
    }
    controller->Move(actionBar);
    //更新视野范围和视野中心
    controller->SetViewArea();
    //球吃食物
    BallEatingFood();
    //孢子更新
    SporeUpdate();
    //刺球的移动
    BoomMove();
    //球吃孢子
    BallEatingSpore();
    //球扎刺
    BallEatingBoom();
    //球吃球
    BallEatingBall();
    //更新角色所拥有的球（可能被吃了，可能扎刺了，可能合并了）
    for(auto*role:AllRoles)
    {
        for(auto itr=role->balls.begin();itr!=role->balls.end();)
        {
            if(itr->weight==-1)
            {
                itr=role->balls.erase(itr);
            }
            else ++itr;
        }
        //如果一个角色所有的球被吃光了,复活
        if(role->balls.size()==0)
        {
            static const int mh=MapSize/2;
            static const int fac=1000;
            auto ball=Ball(role->ballApperance,role->sporeApperance,role->EffectIdx,vec2{random(-mh*fac,mh*fac)*1.0/fac,random(-mh*fac,mh*fac)*1.0/fac});
            ball.weight=BallInitWeight;
            ball.master=role;
            role->balls.push_front(ball);
            if(role->data)
            {
               ((Ai*)(role->data))->time=0;
            }
        }
    }
    //更新刺球
    for(auto*boom:AllBooms)
    {
        if(boom->weight==-1)
        {
            static const int mh=MapSize/2;
            boom->pos=vec2{random(-mh,mh),random(-mh,mh)};
            boom->weight=random(BoomMinWeight,BoomMaxWeight);
        }
    }
    //清空球的四叉树
    for(uint i=AllBalls.NodeDataBeginIdx;i<AllBalls.NodeDataEndIdx;++i)
    {
        auto&data=*(BallTreeType*)(AllBalls.nodes[i].data);
        data.clear();
    }
    //清空刺球的四叉树
    for(uint i=BoomsTree.NodeDataBeginIdx;i<BoomsTree.NodeDataEndIdx;++i)
    {
        auto&data=*(BoomTreeType*)(BoomsTree.nodes[i].data);
        data.clear();
    }
    //重新生成球的四叉树
    for(auto*role:AllRoles)role->AddToTree();
    //重新生成刺球的四叉树
    for(auto*boom:AllBooms)boom->AddToTree();
}
///////////////////////////////////
void GameRender()
{
    WorldMap->RenderFood();
    RenderSpore();
    //////////////////////
    auto&RenderBalls=GetNeedRenderBalls();
    auto&RenderBooms=GetNeedRenderBooms();
    RenderBall(RenderBalls,RenderBooms);
    /////////////////////
    controller->RenderCrossHair();
    actionBar.Render();
}


void RenderBall(vector<Ball*>&NeedRenderedBalls,vector<Boom*>&NeedRenderedBooms)
{
    //////////////////////
    static vector<Ball*> RenderBalls(RoleMaxBalls*RolesMaxCount+BoomMaxCnt+16);
    uint ballCnt=NeedRenderedBalls.size()+NeedRenderedBooms.size();
    RenderBalls.clear();
    RenderBalls.resize(ballCnt);
    int idx=0;
    for(auto*ball:NeedRenderedBalls)
        RenderBalls[idx++]=ball;
    for(auto*boom:NeedRenderedBooms)
        RenderBalls[idx++]=boom;
    sort(RenderBalls.begin(),RenderBalls.end(),[&](Ball*a,Ball*b)
    {
        return a->GetRadian()<b->GetRadian();
    });
    /////////////////////////////////////////////
    BallSurFaceRenderProgram->bind();
    BallSurFaceRenderProgram->setvec2("ViewAreaSize",ViewAreaSize);
    BallSurFaceRenderProgram->setvec2("ViewCenter",ViewCenter);
    for(auto*ball:RenderBalls)ball->Render();
}

vector<Ball*>& GetNeedRenderBalls()
{
    auto&BallTree=*((Tree<vector<Ball*>>*)&AllBalls);
    ///////////////////////////////////////
    set<Ball*>NeedRenderedBalls;
    auto&&r=GetViewAreaTwoNodes(AllBalls);
    auto minblock=AllBalls.MinBlockSize.x;
    float xl=r.first->pos.x+minblock/2,xr=r.second->pos.x+minblock/2;
    float yt=r.first->pos.y-minblock/2,yb=r.second->pos.y-minblock/2;
    for(;xl<=xr;xl+=minblock)
    {
        for(auto y=yt;y>=yb;y-=minblock)
        {
            auto&data=*(BallTreeType*)(AllBalls.GetNode(vec2{xl,y})->data);
            for(auto&ball:data)NeedRenderedBalls.insert(ball);
        }
    }
    static vector<Ball*>res;
    res.clear();
    for(auto*ball:NeedRenderedBalls)res.push_back(ball);
    return res;
}

void BallEatingFood()
{
    static vector<Ball*>balls;
    balls.clear();
    for(auto*role:AllRoles)
        for(auto&ball:role->balls)
            balls.push_back(&ball);
    /////////////////////////////对球进行排序
    sort(balls.begin(),balls.end(),[&](Ball*b0,Ball*b1)
    {
        return b0->weight<b1->weight;
    });
    //////////////////////////////映射食物的信息
    auto*ssbo=WorldMap->FoodResume;
    ssbo->bind();
    auto*ptr=(uint*)ssbo->map(0,WorldMap->FoodCnt*sizeof(uint));
    //////////////////////////////
    auto&tree=*(WorldMap->FoodTree);
    for(auto*ball:balls)
    {
        //////////////////////////获取每个球覆盖食物的区块
        auto r=ball->GetRadian();
        auto&p=ball->pos;
        auto&&nodes=GetTreeTwoNodes(tree,p.x-r,p.x+r,p.y+r,p.y-r);
        GLfloat xbegin=nodes.first->pos.x+tree.MinBlockSize.x/2,xend=nodes.second->pos.x+tree.MinBlockSize.x/2;
        GLfloat ytop=nodes.first->pos.y-tree.MinBlockSize.y/2,ybottom=nodes.second->pos.y-tree.MinBlockSize.y/2;
        auto rPow=r*r;
        for(GLfloat x=xbegin;x<=xend;x+=tree.MinBlockSize.x)
        {
            for(GLfloat y=ytop;y>=ybottom;y-=tree.MinBlockSize.y)
            {
                for(auto&tp:*(tree.GetNode(vec2{x,y})->data)){
                    auto&mp=get<0>(tp);
                    for(auto&ele:mp)
                    {
                        auto&food=ele.second;
                        vec2 pp=food.pos-p;
                        uint idx=ele.first;
                        if(ptr[idx]<=GlobalClock&&(pp.x*pp.x+pp.y*pp.y)<=rPow)//被吃了
                        {
                            ball->weight+=FoodSingleScore;
                            ptr[idx]=food.resume+GlobalClock;
                        }
                    }
                }
            }
        }
    }
    ssbo->unmap();
}


void SporeUpdate()
{
    auto*tree=(Tree<SporeTreeType>*)&AllMoveSpores;
    static vector<uint>nodeSize;
    nodeSize.clear();
    for(uint i=tree->NodeDataBeginIdx;i<tree->NodeDataEndIdx;++i)
    {
        auto*list=(SporeTreeType*)(tree->nodes[i].data);
        nodeSize.push_back(list->size());
    }
    for(uint i=tree->NodeDataBeginIdx,idx=0;i<tree->NodeDataEndIdx;++i,++idx)
    {
        auto*list=(SporeTreeType*)(tree->nodes[i].data);
        uint cnt=0;
        for(auto itr=list->begin();itr!=list->end()&&cnt<nodeSize[idx];++cnt)
        {
            auto&spore=*itr;
            if(spore.state=SPORE_SHOOT)
            {
                spore.Shoot();
                auto*tarList=(SporeTreeType*)(tree->GetNode(spore.CurPos)->data);
                if(list!=tarList){
                    tarList->push_back(spore);
                    itr=list->erase(itr);
                }
                else ++itr;
            }
            else{
                auto*tarList=(SporeTreeType*)(AllStopSpores.GetNode(spore.CurPos)->data);
                tarList->push_back(spore);
                itr=list->erase(itr);
            }
        }
    }
}

void BallEatingBall()
{
    static auto mb=AllBalls.MinBlockSize.x;
    static auto mh=mb/2;
    static vector<Ball*>allBalls;
    allBalls.clear();
    for(auto*role:AllRoles)
        for(auto&ball:role->balls)
            allBalls.push_back(&ball);
    /////////////////////////////大球先吃，小球后吃人
    sort(allBalls.begin(),allBalls.end(),[&](Ball*a,Ball*b)
    {
        return a->weight>b->weight;
    });
    for(int i=0;i<allBalls.size();++i)
    {
        auto&ball=allBalls[i];
        if(ball->weight==-1)continue;
        auto r=ball->GetRadian();
        auto r_pow=r*r;
        bool canMerge=ball->GetMergeTime()<=ball->aliveTime;
        for(int j=allBalls.size()-1;j>i;--j)
        {
            auto&tar=allBalls[j];
            if(tar->weight==-1)continue;
            ///////////////////////////如果是自己的球
            if(tar->master==ball->master)
            {
                vec2 off=ball->pos-tar->pos;
                float dd=off.x*off.x+off.y*off.y;
                if(canMerge||tar->GetMergeTime()<=tar->aliveTime)
                {
                    if(dd<=r_pow){
                        canMerge=0;
                        ball->aliveTime=0;
                        ball->weight+=tar->weight;
                        tar->weight=-1;
                    }
                    else{

                    }
                }
                else if(ball->ballState!=BALL_SHOOT&&tar->ballState!=BALL_SHOOT){
                    float rr=r+tar->GetRadian();
                    float rr_pow=rr*rr;
                    if(dd<rr_pow)//如果俩球不能合并却靠的太近，球向相反方向弹开
                    {
                        auto tarPos=off.normalize()*(-rr)+ball->pos;
                        auto tarPos1=off.normalize()*(rr)+tar->pos;
                        if(CheckIsOverEdge(tarPos)==0)tar->pos=tarPos;
                        else if(CheckIsOverEdge(tarPos1)==0)ball->pos=tarPos1;
                    }
                }
            }
            ///////////////////////////如果是敌人的球
            else{
                vec2 off=ball->pos-tar->pos;
                if(ball->weight>=tar->weight*BallCanEatBallWeightTime&&off.x*off.x+off.y*off.y<r_pow)
                {
                    ball->weight+=tar->weight;
                    tar->weight=-1;
                }
            }
        }
    }
}
void RenderSpore()
{
    static const uint vertexCnt=20;
    static const float scale=1.0;
    static ShaderProgram*p=0;
    static VertexArrayObj*vao=0;
    if(!p)
    {
        p=new ShaderProgram("src/Shader/SporeRender.vert","src/Shader/SporeRender.frag");
        vao=new VertexArrayObj();
        ////////////////////////////
        vector<vec2>vertex(vertexCnt);
        vector<tuple<uint,uint,uint>>indices(vertexCnt-2);
        for(int i=0;i<vertexCnt;++i)vertex[i]=vec2{cos(i*2*PI/vertexCnt),sin(i*2*PI/vertexCnt)}*scale;
        for(int i=0;i<vertexCnt-2;++i)indices[i]=tuple<uint,uint,uint>{0,i+1,i+2};
        VertexBufferObj*vbo0=new VertexBufferObj(vertex.size()*sizeof(vec2),vertex.data());
        vbo0->setpoint(1,GL_FLOAT,2,sizeof(vec2),0);
        ElementBufferObj*ebo=new ElementBufferObj(sizeof(uint)*3*indices.size(),indices.data());
        ////////////////////////////
        vao->unbind();
        ////////////////////////////
        p->setfloat("TexOff",1.0/SporeTypesCnt);
    }
    ///////////////////////////////////////////
    static vector<vec3>data;
    data.clear();
    auto fun=[&](Tree<void*>&tree)->void
    {
        auto&&nodes=GetViewAreaTwoNodes(tree);
        float mb=tree.MinBlockSize.x;
        float xb=nodes.first->pos.x+mb/2,xe=nodes.second->pos.x+mb/2;
        float yt=nodes.first->pos.y-mb/2,yb=nodes.second->pos.y-mb/2;
        for(auto x=xb;x<=xe;x+=mb)
        {
            for(auto y=yt;y>=yb;y-=mb)
            {
                auto&node=*(SporeTreeType*)(tree.GetNode(vec2{x,y})->data);
                for(auto&spore:node)
                {
                    data.push_back(vec3{spore.CurPos.x,spore.CurPos.y,spore.appearance});
                }
            }
        }
    };
    fun(AllStopSpores);
    fun(AllMoveSpores);
    //////////////////////////////////////////
    vao->bind();
    VertexBufferObj*vbo=new VertexBufferObj(data.size()*sizeof(vec3),data.data());
    vbo->setpoint(0,1);
    vbo->setpoint(0,GL_FLOAT,3,sizeof(vec3),0);
    p->bind();
    p->setvec2("ViewAreaSize",ViewAreaSize);
    p->setvec2("ViewCenter",ViewCenter);
    p->setuint("sampler",0);
    SporeTypes->bind(0);
    openglAPIPointer->glDrawElementsInstanced(GL_TRIANGLES,(vertexCnt-2)*3,GL_UNSIGNED_INT,0,data.size());
}

tuple<Texture2D *, uint, uint> LoadAllImgsToOnlyOneTexture(const vector<string>&paths)
{
    vector<QImage>images;
    for(auto&pathS:paths){
        auto*path=pathS.c_str();
        QDir dir(path);
        auto fileList=dir.entryList();
        fileList.removeOne(".");
        fileList.removeOne("..");
        ///////加载所有image
        for(auto name:fileList)
        {
            QImage img=QImage(QString(path)+"\\"+name);
            images.push_back(img);
        }
    }
    ///////////合并所有image
    uint cnt=images.size();
    uint mnW=-1,mnH=-1;
    for(auto&img:images)
    {
        mnW=min(mnW,uint(img.width()));
        mnH=min(mnH,uint(img.height()));
    }
    //把所有图片放缩成一个大小
    for(auto&img:images)img=img.scaled(mnW, mnH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_RGBA8888);
    //为了保证最后纹理的个数为2的幂，补全纹理
    uint tarCnt=1<<uint(ceil(log2(cnt)));
    uint singleSize=mnW*mnH*4;
    vector<unsigned char>data(singleSize*tarCnt);
    uint idx=0;
    for(auto&img:images)
    {
        memcpy(&data[idx],img.bits(),singleSize);
        idx+=singleSize;
    }

    return tuple<Texture2D*, uint, uint>{new Texture2D(mnW,mnH*tarCnt,data.data()),cnt,tarCnt};
}

void BallEatingSpore()
{
    for(auto*role:AllRoles)
    {
        for(auto&ball:role->balls)
        {
            if(ball.weight<BallCanEatSporeMinWeight)continue;
            float r=ball.GetRadian();
            float r_pow=r*r;
            float xl=ball.pos.x-r,xr=ball.pos.x+r;
            float yt=ball.pos.y+r,yb=ball.pos.y-r;
            auto fun=[&](Tree<void*>&tree)->void{
                auto&&ret=GetTreeTwoNodes(tree,xl,xr,yt,yb);
                float mb=tree.MinBlockSize.x;
                float mh=mb/2;
                float xb=ret.first->pos.x+mh,xe=ret.second->pos.x+mh;
                float yb=ret.first->pos.y-mh,ye=ret.second->pos.y-mh;
                for(auto x=xb;x<=xe;x+=mb)
                {
                    for(auto y=yb;y>=ye;y-=mb)
                    {
                        auto*data=(SporeTreeType*)tree.GetNode(vec2{x,y})->data;
                        for(auto itr=data->begin();itr!=data->end();)
                        {
                            auto&spore=*itr;
                            auto xo=spore.CurPos.x-ball.pos.x,yo=spore.CurPos.y-ball.pos.y;
                            if(xo*xo+yo*yo<=r_pow)
                            {
                                ball.weight+=SporeSingleWeight;
                                itr=data->erase(itr);
                            }
                            else ++itr;
                        }
                    }
                }
            };
            fun(AllMoveSpores);
            fun(AllStopSpores);
        }
    }
}

void RoleMove(Role &role)
{
    if(role.data==0)role.data=new Ai(role);
    ((Ai*)role.data)->move();
}

void BoomMove()
{
    ////////////////////////////////////////////////////////移动
    for(auto*boomP:AllBooms)boomP->Shoot();
    ////////////////////////////////////////////////////////刺球会去吃孢子
    for(auto*boomP:AllBooms)
    {
        Boom&boom=*boomP;
        auto r=boom.GetRadian();
        auto r_pow=r*r;
        auto xl=boom.pos.x-r,xr=boom.pos.x+r,yt=boom.pos.y+r,yb=boom.pos.y-r;
        auto fun=[&](Tree<void*>&tree)
        {
            auto&&ret=GetTreeTwoNodes(tree,xl,xr,yt,yb);
            static const auto mb=tree.MinBlockSize.x;
            static const auto mh=mb/2;
            float xb=ret.first->pos.x+mh,xe=ret.second->pos.x+mh;
            float yt=ret.first->pos.y-mh,yb=ret.second->pos.y-mh;
            for(auto x=xb;x<=xe;x+=mb)
            {
                for(auto y=yt;y>=yb;y-=mb)
                {
                    auto*d=(SporeTreeType*)(tree.GetNode(vec2{x,y})->data);
                    auto&data=*(SporeTreeType*)(tree.GetNode(vec2{x,y})->data);
                    for(auto itr=data.begin();itr!=data.end();)
                    {
                        auto&spore=*itr;
                        auto off=boom.pos-spore.CurPos;
                        if(off.x*off.x+off.y*off.y<=r_pow)
                        {
                           static GLuint BoomSpitWeight=2*BoomMinWeight;
                           vec2 dir=off.normalize();
                           boom.weight+=SporeSingleWeight;
                           if(boom.weight>=BoomSpitWeight)
                           {
                               auto spit=boom.Spit(dir);
                               if(AllBooms.size()<BoomMaxCnt)
                                AllBooms.push_back(new Boom(spit));
                           }
                           boom.shootTime=0;
                           boom.ballState=BALL_SHOOT;
                           vec2 tarPos=dir*BoomShootDis+boom.pos;
                           if(CheckIsOverEdge(tarPos))tarPos=GetValidPos(tarPos,dir,BoomShootDis);
                           boom.ShootPos=tarPos;
                           itr=data.erase(itr);
                        }
                        else ++itr;
                    }
                }
            }
        };
        fun(AllStopSpores);
        fun(AllMoveSpores);
    }
}

void BallEatingBoom()
{
    static const float mb=AllBalls.MinBlockSize.x;
    static const float mh=mb/2;

    for(auto*boom:AllBooms)
    {
        bool beEat=0;
        auto r=boom->GetRadian();
        float EatWeight=boom->weight*BallCanEatBoomWeightTime;
        float xl=boom->pos.x-r,xr=boom->pos.x+r;
        float yt=boom->pos.y+r,yb=boom->pos.y-r;
        auto&&ret=GetTreeTwoNodes(AllBalls,xl,xr,yt,yb);
        float xb=ret.first->pos.x+mh,xe=ret.second->pos.x+mh;
        yt=ret.first->pos.y-mh,yb=ret.second->pos.y-mh;
        for(auto x=xb;x<=xe;x+=mb)
        {
            for(auto y=yt;y>=yb;y-=mb)
            {

                auto&ballv=*(BallTreeType*)(AllBalls.GetNode(vec2{x,y})->data);
                Ball*tar=0;
                for(auto*ball:ballv)
                {
                    if(ball->weight>EatWeight&&(!tar||tar->weight<ball->weight)){
                        auto off=ball->pos-boom->pos;
                        auto r=ball->GetRadian();
                        if(off.x*off.x+off.y*off.y<=r*r)
                            tar=ball;
                    }
                }
                if(tar)
                {
                    static const uint SpitMaxCnt=5;
                    auto*role=(Role*)tar->master;
                    auto alreadyOwnBalls=role->balls.size();
                    auto spit=min(RoleMaxBalls-alreadyOwnBalls,SpitMaxCnt);
                    ////////////////////////////////
                    static const GLuint spitBallMinWeight=BoomMinWeight/SpitMaxCnt;
                    static const GLuint spitBallMaxWeight=BoomMaxWeight/SpitMaxCnt;
                    static const auto w0=BoomMinWeight*BallCanEatBoomWeightTime;
                    static const auto w1=BoomMaxWeight*BallCanEatBoomWeightTime;
                    static const auto div_w=w1-w0;
                    float fac=(tar->weight-w1)/div_w;
                    fac=fmax(fmin(fac,1),0);
                    tar->weight+=boom->weight;
                    float singleWeight=spitBallMinWeight*fac+(1-fac)*spitBallMaxWeight;
                    auto&balls=role->balls;
                    for(int i=0;i<spit;++i)
                    {
                        balls.push_back(tar->Shoot(singleWeight,vec2{
                                                       cos(i*2*PI/spit),
                                                       sin(i*2*PI/spit)
                                                   }));
                    }
                    ////////////////////////////////
                    boom->weight=-1;
                    beEat=1;
                }
                if(beEat)break;
            }
            if(beEat)break;
        }
    }
}

vector<Boom*>& GetNeedRenderBooms()
{
    ///////////////////////////////算出哪些刺球要被渲染
    auto&&ret=GetViewAreaTwoNodes(BoomsTree);
    set<Boom*>NeedRenderBooms;
    static const float mb=BoomsTree.MinBlockSize.x;
    static const float mh=mb/2;
    float xb=ret.first->pos.x+mh,xe=ret.second->pos.x+mh;
    float yt=ret.first->pos.y-mh,yb=ret.second->pos.y-mh;
    for(auto x=xb;x<=xe;x+=mb)
    {
        for(auto y=yt;y>=yb;y-=mb)
        {
            auto&v=*(BoomTreeType*)BoomsTree.GetNode(vec2{x,y})->data;
            for(auto*boom:v)NeedRenderBooms.insert(boom);
        }
    }
    static vector<Boom*>booms;
    booms.clear();
    for(auto*boom:NeedRenderBooms)booms.push_back(boom);
    return booms;
}

void BallLoseWeight()
{
    for(auto*role:AllRoles)
    {
        for(auto&ball:role->balls)
        {
            ball.weight-=ball.LoseWeight();
        }
    }
}


GLuint RandomEffect()
{
    static const auto cnt=AllEffects.size();
    return random(0,cnt-1);
}

void InitAllProgram()
{
    ///////
    BallSurFaceRenderProgram=new ShaderProgram("src/Shader/BallSurfaceRender.vert","src/Shader/BallSurfaceRender.frag");
    BallSurFaceRenderProgram->setfloat("TexOff",1.0/BallTypesCnt);
    //////
}
