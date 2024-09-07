#version 330 core
layout(location=0)in vec2 pos;
layout(location=1)in float size;
uniform vec2 ViewAreaSize;
uniform vec2 ViewCenter;
uniform vec2 BallPos;
uniform float BallRadian;
void main(){
	float fac=float(length(pos)>=1.0);
	vec2 realPos=(pos*BallRadian+BallPos-ViewCenter)*2/ViewAreaSize;
	realPos.x=mix(realPos.x,100.0,fac);
	gl_PointSize=size;
	gl_Position=vec4(realPos,0.0,1.0);
}
