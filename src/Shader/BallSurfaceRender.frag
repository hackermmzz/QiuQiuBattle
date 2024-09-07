#version 330 core
uniform sampler2D sampler;
uniform float TexOff;
in float Type;
in vec2 TexCoord;
out vec4 fragColor;
void main(){
        vec2 texcoord=vec2(TexCoord.x,(TexCoord.y+Type)*TexOff);
        fragColor=texture2D(sampler,texcoord);
}
