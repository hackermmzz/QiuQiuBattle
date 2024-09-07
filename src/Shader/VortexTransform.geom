#version 330 core
layout(points)in;
layout(points,max_vertices=10)out;
uniform sampler2D random;
uniform float Time;
uniform float fac;
in vec3 Data0[];
in vec2 Data1[];
out vec2 pos;
out vec3 color;
out float r;
out float degree;
const float ColorExchangeFac=0.1;
const vec3 ColorChangeRangeBegin=vec3(0.2,0.2,1.0);
const vec3 ColorChangeRangeEnd=vec3(0.6,0.2,0.6);
const float PI=acos(-1.0);
const float DPI=2*PI;
const float Speed=PI/18000.0;
float GetRandVal(vec2 pos)
{
	vec4 ret=texture(random,pos+vec2(fac));
	return ret.r;
}
void main(){
	color=Data0[0];
	r=Data1[0].x;
	degree=Data1[0].y;
	float random=GetRandVal(pos+vec2(fac));
	degree+=(mix(0.3+random,1.0,r))*Speed*Time;
	degree=mod(degree,DPI);
	pos=r*vec2(cos(degree),sin(degree));
	vec3 bonusColor=vec3(GetRandVal(pos),GetRandVal(pos+vec2(fac)),GetRandVal(pos+vec2(fac*2.0)));
	color=mix(ColorChangeRangeBegin,ColorChangeRangeEnd,degree/DPI);
	color=mix(color,bonusColor,ColorExchangeFac);
	EmitVertex();
	EndPrimitive();
}
