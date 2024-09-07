#ifndef TREE_H
#define TREE_H
template<class T>
struct Tree
{
    using Type=T;
    struct Node{
        vec2 pos;
        double w,h;
        T*data;
    };
   using NodeType=Node;
    Node*nodes;
    uint depth;
    uint NodeDataBeginIdx;
    uint NodeDataEndIdx;
    vec2 MinBlockSize;
    Tree(uint deep,double w,double h);
    Node*GetNode(const vec2&p);
    ~Tree();
};
///////////////////////////////////
template<class T>
Tree<T>::Tree(uint deep,double w,double h)
{
    nodes=new Node[uint((pow(4,deep)-1)/3+1)];
    depth=deep;
    NodeDataBeginIdx=uint(pow(4,depth-1)-1)/3+1;
    NodeDataEndIdx=uint(pow(4,depth)-1)/3+1;
    MinBlockSize=vec2{w/pow(2,depth-1),h/pow(2,depth-1)};
    function<void(double,double,double,double,uint,uint)>build=[&](double x,double y,double w,double h,uint deep,uint idx){
        auto&node=nodes[idx];
        node.pos=vec2{x,y};
        node.w=w,node.h=h;
        node.data=0;
        if(deep>1){
            auto ch_idx=(idx<<2)-2;
            build(x,y,w/2,h/2,deep-1,ch_idx);
            build(x+w/2,y,w/2,h/2,deep-1,ch_idx+1);
            build(x+w/2,y-h/2,w/2,h/2,deep-1,ch_idx+2);
            build(x,y-h/2,w/2,h/2,deep-1,ch_idx+3);
        }
    };
    build(-w/2,h/2,w,h,deep,1);
}

template<class T>
typename Tree<T>::NodeType* Tree<T>::GetNode(const vec2 &p)
{
    auto point_inArea=[&](Node&node,const vec2&p)->bool{
        static const auto esp=std::numeric_limits<float>::epsilon();
        auto xo=double(p.x)-node.pos.x,yo=node.pos.y-double(p.y);
        if(xo>=-esp&&yo>=-esp&&xo<=node.w+esp&&yo<=node.h+esp)return 1;
        return 0;
    };
    uint curP=1;
    uint deep=1;
    if(point_inArea(nodes[1],p)==0)return 0;
    while(deep<depth)
    {
        uint ch=(curP<<2)-2;
        for(int i=0;i<4;++i)
        {
            if(point_inArea(nodes[ch+i],p))
            {
                curP=ch+i;
                break;
            }
        }
        ++deep;
    }
    return &nodes[curP];
}

template<class T>
Tree<T>::~Tree()
{
    delete[]nodes;
}

///////////////////////////////////
#endif // TREE_H
