#version 330 core
layout(location=0)in vec2 pos;
uniform vec3 OffsetAndScale;
void main()
{
	gl_Position=vec4(OffsetAndScale.xy+pos*OffsetAndScale.z,0.0,1.0);
}
