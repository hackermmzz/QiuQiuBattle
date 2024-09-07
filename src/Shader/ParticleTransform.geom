#version 330 core
layout(points)in;
layout(points,max_vertices=10)out;
uniform sampler2D random;
uniform float Time;
uniform float fac;
in vec4 Data0[];
in vec3 Data1[];
in vec4 Data2[];
out vec2 pos;
out vec2 tarPos;
out vec3 color;
out float ShootTime;
out float TotalTime;
out vec2 initPos;
const float MinTime=3000.0;
const float MaxTime=6000.0;
float GetRandVal(vec2 pos)
{
	vec4 ret=texture(random,pos+vec2(fac));
	return ret.r;
}
float GetRandVal_(vec2 pos){
return GetRandVal(pos)*2.0-1.0;
}
void main(){
	pos=Data0[0].xy;
	tarPos=Data0[0].zw;
	color=Data1[0];
	ShootTime=Data2[0].x;
	TotalTime=Data2[0].y;
	initPos=Data2[0].zw;
	ShootTime+=Time;
	float fac=ShootTime/TotalTime;
	pos=mix(initPos,tarPos,min(fac,1.0));
	if(ShootTime>=TotalTime)
	{
		initPos=tarPos;
		TotalTime=mix(MinTime,MaxTime,GetRandVal(pos));
		ShootTime=0.0;
		tarPos=vec2(GetRandVal_(pos),GetRandVal_(pos+vec2(fac)));
	}
	EmitVertex();
	EndPrimitive();

}
