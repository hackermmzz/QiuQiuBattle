#version 330 core
layout(location=0)in vec3 data;
layout(location=1)in vec2 vertex;
uniform vec2 ViewAreaSize;
uniform vec2 ViewCenter;
out vec2 TexCoord;
void main(){
	  TexCoord=vec2(vertex.x*0.5+0.5,0.5-vertex.y*0.5);
	  vec2 off=data.xy;
	  float r=data.z;
	  gl_Position=vec4((vertex*r+off-ViewCenter)*2/ViewAreaSize,0.0,1.0);
}
