#version 330 core
uniform sampler2D sampler;
uniform float TexOff;
in float  Type;
in vec2 TexCoord;
out vec4 color;
void main(){
	vec2 coord=vec2(TexCoord.x,(TexCoord.y+Type)*TexOff);
	color=texture(sampler,coord);
}
