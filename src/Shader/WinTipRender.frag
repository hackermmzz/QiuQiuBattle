#version 330 core
uniform sampler2D sampler;
out vec4 color;
in vec2 TexCoord;
void main(){
	color=texture2D(sampler,TexCoord);
}
