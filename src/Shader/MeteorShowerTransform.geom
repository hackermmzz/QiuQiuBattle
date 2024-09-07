#version 330 core
layout(points)in;
layout(points,max_vertices=10)out;
uniform sampler2D random;
uniform float Time;
uniform float fac;
in vec4 Data[];
out vec2 pos;
out float type;
out float size;
const float MinTime=50.0;
const float MaxTime=200.0;
const float BeginDegree=0.0;
const float EndDegree=acos(-1.0);
const float MinSize=1;
const float MaxSize=5;
const float MinSpeed=2.0/5000.0;
const float MaxSpeed=2.0/1000.0;
const vec2 dir=normalize(vec2(-1.0,-1.0));
float GetRandVal(vec2 pos)
{
	vec4 ret=texture(random,pos+vec2(fac));
	return ret.r;
}
float GetRandVal_(vec2 pos){
return GetRandVal(pos)*2.0-1.0;
}
void main()
{
	pos=Data[0].xy;
	type=Data[0].z;
	size=Data[0].w;
	//如果是流星雨产生器
	if(type==0.0){
		if(size<=0.0){//产生流星雨
			size=mix(MinTime,MaxTime,GetRandVal(pos+vec2(fac)*3.0));
			EmitVertex();
			EndPrimitive();
			size=mix(MinSize,MaxSize,GetRandVal(pos+vec2(fac)));
			type=mix(MinSpeed,MaxSpeed,GetRandVal(pos+vec2(fac*fac)));//type不为0表示为流星的速度
			float degree=mix(BeginDegree,EndDegree,GetRandVal(pos+vec2(fac*2.0)));
			pos=vec2(cos(degree),sin(degree));
			EmitVertex();
			EndPrimitive();
		}
		else{
			size-=Time;
			EmitVertex();
			EndPrimitive();
		}
	}
	//反之为流星
	else{
		pos+=dir*(type*Time);
		if(pos.y>=-1.1)
		{
			EmitVertex();
			EndPrimitive();
		}
	}	
}
