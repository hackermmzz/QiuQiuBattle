#version 330 core
layout(location=0)in vec4 data0;
layout(location=1)in vec3 data1;
layout(location=2)in vec4 data2;
out vec4 Data0;
out vec3 Data1;
out vec4 Data2;
void main(){
	Data0=data0;
	Data1=data1;
	Data2=data2;
}
