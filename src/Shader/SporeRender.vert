#version 330 core
layout(location=0)in vec3 data;
layout(location=1)in vec2 pos;
uniform vec2 ViewAreaSize;
uniform vec2 ViewCenter;
const float scale=2.0;
out vec2 TexCoord;
out float  Type;
void main()
{
		TexCoord=vec2(pos.x*0.5+0.5,0.5-pos.y*0.5);
		vec2 realPos=(data.xy+pos*scale-ViewCenter)*2/ViewAreaSize;
		Type=data.z;
		gl_Position=vec4(realPos,0.0,1.0);
}
