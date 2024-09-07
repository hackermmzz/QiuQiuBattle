#version 330 core
layout(location=0)in float idx;
const vec2 pos[4]={vec2(-1.0,1.0),vec2(1.0,1.0),vec2(1.0,-1.0),vec2(-1.0,-1.0)};
const vec2 tex[4]={vec2(0.0,0.0),vec2(1.0,0.0),vec2(1.0,1.0),vec2(0.0,1.0)};
out vec2 texCoord;
void main(){
	texCoord=tex[uint(idx)];
	vec2 p=pos[uint(idx)];
	gl_Position=vec4(p,0.0,1.0);

}
