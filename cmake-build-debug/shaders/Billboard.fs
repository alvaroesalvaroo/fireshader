// Billboard.fs
#version 330 core

in vec2 FragTexture;
out vec4 FragColor;

uniform sampler2D billboardTex;


void main() {
    vec4 smoke = texture(billboardTex, FragTexture);
    FragColor = smoke;

}