#version 330 core
layout(location=0)in vec3 data0;
layout(location=1)in vec2 data1;
out vec3 Data0;
out vec2 Data1;
void main(){
	Data0=data0;
	Data1=data1;
}
