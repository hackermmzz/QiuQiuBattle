#version 330 core
layout(location=0)in vec4 data;
uniform vec2 off;
out vec2 TexCoord;
void main(){
	TexCoord=data.zw;
	gl_Position=vec4(data.xy+off,0.0,1.0);
}
