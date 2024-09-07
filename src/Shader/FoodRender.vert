#version 430 core
layout(location=0)in vec4 data;
layout(location=1)in vec3 VertexColor;
layout(location=2)in vec2 pos;
uniform vec2 ViewAreaSize;
uniform vec2 ViewCenter;
uniform uint curTime;
layout(std430)buffer Rotate{
vec2 rotate[];
};
layout(std430)buffer Resume{
uint resume[];
};
out vec3 color;
void main()
{
        uint idx=uint(data.w);
        vec2 off=data.xy;
	    float degree=data.z;
        vec2 fac=rotate[uint(degree)];
        vec2 transformPos=vec2(pos.x*fac.x-pos.y*fac.y,pos.x*fac.y+pos.y*fac.x);
        vec2 realPos=(transformPos+off-ViewCenter)*2/ViewAreaSize;
        bool fac0=resume[idx]>curTime;
        realPos.x=mix(realPos.x,1.0,float(fac0));
        gl_Position=vec4(realPos,0.0,1.0);
        color=VertexColor;
}
