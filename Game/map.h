#ifndef MAP_H
#define MAP_H
#include"globalsource.h"
#include"food.h"
#include"unordered_map"
using namespace std;
struct Map
{
    Tree<vector<tuple<unordered_map<uint,Food>,VertexArrayObj*,uint,uint>>>* FoodTree;
    vector<VertexArrayObj*>FoodVao;
    vector<VertexBufferObj*>FoodVbo;
    vector<ElementBufferObj*>FoodEbo;
    ShaderStorageBufferObj*FoodResume;
    ShaderStorageBufferObj*Rotate;
    ShaderProgram*FoodProgram;
    uint FoodCnt;
    ////////////////////////
    Map();
    ~Map();
    void RenderFood();
};

#endif // MAP_H
