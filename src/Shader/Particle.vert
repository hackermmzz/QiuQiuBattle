#version 330 core
layout(location=0)in vec2 pos;
layout(location=1)in vec3 color_;
uniform vec2 ViewAreaSize;
uniform vec2 ViewCenter;
out vec3 color;
void main(){
	color=color_;
	gl_Position=vec4(pos,0.0,1.0);
}
