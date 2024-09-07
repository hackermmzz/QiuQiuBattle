#version 330 core
uniform sampler2D sampler;
in vec2 TexCoord;
out vec4 fragColor;
void main(){
        fragColor=texture2D(sampler,TexCoord);
}
