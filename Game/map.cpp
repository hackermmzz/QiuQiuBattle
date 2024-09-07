#include "map.h"

Map::Map()
{
    //////////////////////////构建食物的四叉树
    using FoodTreeType=remove_pointer_t<decltype(FoodTree)>;
    FoodTree=new FoodTreeType(FoodTreeDeep,MapSize,MapSize);
    //////////////////////////初始化食物
    int MapHalfSize=MapSize/2;
    FoodCnt=0;
    for(int x=-MapHalfSize;x<MapHalfSize;++x)
    {
        for(int y=-MapHalfSize;y<MapHalfSize;++y)
        {
            if(random(0,1000)/1000.0<=FoodWeight){
                Food food;
                food.color=vec3{random(0,100)/100.0,random(0,100)/100.0,random(0,100)/100.0};
                food.pos=vec2{(random(0,100)/100.0+x),(random(0,100)/100.0+y)};
                food.rotate=random(0,360);
                food.resume=random(FoodResumeMinTime,FoodResumeMaxTime);
                uint sides=random(3,FoodMaxSides);
                auto*node=FoodTree->GetNode(food.pos);
                if(node){
                    auto&data=node->data;
                    if(data==0)
                    {
                        data=new FoodTreeType::Type;
                        data->resize(FoodMaxSides+1);
                    }
                    get<0>((*data)[sides])[FoodCnt++]=food;
                }
            }
        }
    }
    //////////////////////////加载食物数据
    //所有食物刚开始的状态都是存活状态
    GLuint FoodAliveSize=sizeof(uint)*FoodCnt;
    FoodResume=new ShaderStorageBufferObj(FoodAliveSize);
    auto FoodResumeBufferBinding=BufferBindingBase++;
    FoodResume->bindBase(FoodResumeBufferBinding);
    auto*FoodSsboMapPtr=(uint*)FoodResume->map(0,FoodAliveSize);
    memset(FoodSsboMapPtr,0,FoodAliveSize);
    FoodResume->unmap();
   //生成n边形
    vector<vec2>ShapeVertex[FoodMaxSides+1];
    vector<tuple<int,int,int>>ShapeVertexIdx[FoodMaxSides+1];
    for(int i=3;i<=FoodMaxSides;++i)
    {
        float perDegree=2*PI/i;
        for(int j=0;j<i;++j)
        {
            float degree=perDegree*j;
            ShapeVertex[i].push_back(vec2{cos(degree),sin(degree)});
        }
        for(int j=2;j<i;++j)
        {
            ShapeVertexIdx[i].push_back(tuple<int,int,int>{0,j-1,j});
        }
    }
    //生成vao vbo ebo
    for(uint nodeidx=FoodTree->NodeDataBeginIdx;nodeidx<FoodTree->NodeDataEndIdx;++nodeidx){
        if((FoodTree->nodes[nodeidx]).data==0)
        {
            (FoodTree->nodes[nodeidx]).data=new FoodTreeType::Type;
            continue;
        }
        for(int i=3;i<=FoodMaxSides;++i){
            auto&data=(*((FoodTree->nodes[nodeidx]).data))[i];
            using VboFoodDataType=tuple<vec3,GLfloat,GLfloat,vec2>;
            vector<VboFoodDataType>foodData;
            for(auto&ele:get<0>(data)){
                auto&food=ele.second;
                foodData.emplace_back(VboFoodDataType{food.color,ele.first,food.rotate,food.pos});
            }
                if(foodData.size()==0)continue;
                auto*vao=new VertexArrayObj();
                auto*vbo=new VertexBufferObj(sizeof(VboFoodDataType)*foodData.size(),&(foodData.front()));
                vbo->setpoint(0,1);
                vbo->setpoint(0,GL_FLOAT,4,sizeof(VboFoodDataType),0);
                vbo->setpoint(1,1);
                vbo->setpoint(1,GL_FLOAT,3,sizeof(VboFoodDataType),sizeof(GLfloat)*4);
                auto*vbo1=new VertexBufferObj(sizeof(vec2)*i,&ShapeVertex[i].front());
                vbo1->setpoint(2,GL_FLOAT,2,sizeof(vec2),0);
                auto*ebo=new ElementBufferObj(sizeof(GLuint)*3*(i-2),&ShapeVertexIdx[i].front());
                vao->unbind();
                FoodVao.push_back(vao);
                FoodVbo.push_back(vbo);
                FoodVbo.push_back(vbo1);
                FoodEbo.push_back(ebo);
                get<1>(data)=vao;
                get<2>(data)=(i-2)*3;
                get<3>(data)=foodData.size();
            }
        }
    ////////////////////旋转ssbo
    Rotate=new ShaderStorageBufferObj(360*sizeof(vec2));
    auto RotateBufferBinding=BufferBindingBase++;
    Rotate->bindBase(RotateBufferBinding);
    auto*RotateMapPtr=(vec2*)Rotate->map(0,360*sizeof(vec2));
    for(int i=0;i<360;++i)RotateMapPtr[i]=vec2{cos(i*PI/180),sin(i*PI/180)};
    Rotate->unmap();
    ////////////////////
    FoodProgram=new ShaderProgram("src/Shader/FoodRender.vert","src/Shader/FoodRender.frag");
    FoodProgram->setSsbo("Rotate",RotateBufferBinding);
    FoodProgram->setSsbo("Resume",FoodResumeBufferBinding);
    //////////////////////////
    //////////////////////////
    ViewCenter=vec2{0,0};
}

Map::~Map()
{
    for(auto&v:FoodVao)delete v;
    for(auto&v:FoodVbo)delete v;
    for(auto&v:FoodEbo)delete v;
}

void Map::RenderFood()
{
    FoodProgram->bind();
    FoodProgram->setvec2("ViewAreaSize",ViewAreaSize);
    FoodProgram->setvec2("ViewCenter",ViewCenter);
    FoodProgram->setuint("curTime",GlobalClock);
    ///////////////////////////裁剪
    static const auto esp=std::numeric_limits<float>::epsilon();
    const static float MapHalfSize=MapSize/2;
    const static GLfloat MinBlockSize=MapSize/pow(2,FoodTreeDeep-1);
    GLfloat HalfMinBlockSize=MinBlockSize/2;
    vec2 p0=vec2{ViewCenter.x-ViewAreaSize.x/2,ViewCenter.y+ViewAreaSize.y/2};
    vec2 p1=vec2{ViewCenter.x+ViewAreaSize.x/2,ViewCenter.y-ViewAreaSize.y/2};
    p0.x=min(MapHalfSize-esp,max(-MapHalfSize+esp,p0.x));
    p0.y=min(MapHalfSize-esp,max(-MapHalfSize+esp,p0.y));
    p1.x=min(MapHalfSize-esp,max(-MapHalfSize+esp,p1.x));
    p1.y=min(MapHalfSize-esp,max(-MapHalfSize+esp,p1.y));
    auto*node0=FoodTree->GetNode(p0),*node1=FoodTree->GetNode(p1);
    float x_begin,x_end,y_begin,y_end;
    x_begin=node0->pos.x+HalfMinBlockSize;
    x_end=node1->pos.x+HalfMinBlockSize;
    y_begin=node0->pos.y-HalfMinBlockSize;
    y_end=node1->pos.y-HalfMinBlockSize;
    for(;x_begin<=x_end;x_begin+=MinBlockSize)
    {
        for(float y=y_begin;y>=y_end;y-=MinBlockSize){
            auto*node=FoodTree->GetNode(vec2{x_begin,y});
            auto&v=node->data;
            for(auto&data:(*v)){
                auto*vao=get<1>(data);
                if(vao==0)continue;
                auto cnt0=get<2>(data);
                auto cnt1=get<3>(data);
                vao->bind();
                openglAPIPointer->glDrawElementsInstanced(GL_TRIANGLES,cnt0,GL_UNSIGNED_INT,0,cnt1);
                vao->unbind();
            }
        }
    }
}
