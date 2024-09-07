#version 330 core
layout(location=0)in vec2 pos_;
layout(location=1)in vec3 color_;
uniform vec2 ViewAreaSize;
uniform vec2 ViewCenter;
uniform vec2 BallPos;
uniform float BallRadian;
out vec3 color;
void main(){
	color=color_;
	float fac=float(length(pos_)>=1.0);
	vec2 realPos=(pos_*BallRadian+BallPos-ViewCenter)*2/ViewAreaSize;
	realPos.x=mix(realPos.x,100.0,fac);
	gl_PointSize=3.0;
	gl_Position=vec4(realPos,0.0,1.0);
}
