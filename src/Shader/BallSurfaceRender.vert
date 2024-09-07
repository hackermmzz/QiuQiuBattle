#version 330 core
layout(location=0)in vec2 vertex;
uniform vec2 ViewAreaSize;
uniform vec2 ViewCenter;
uniform vec4 data;
out float Type;
out vec2 TexCoord;
void main(){
    Type=data.w;
    TexCoord=vec2(vertex.x*0.5+0.5,0.5-vertex.y*0.5);
	vec2 off=data.xy;
    float r=data.z;
    vec2 realPos=(vertex*r+off-ViewCenter)*2/ViewAreaSize;
    gl_Position=vec4(realPos,0.0,1.0);
}
