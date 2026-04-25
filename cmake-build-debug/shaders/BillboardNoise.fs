// Billboard.fs
#version 330 core

in vec2 FragTexture;
out vec4 FragColor;

uniform sampler2D billboardTex;
uniform sampler2D noiseTex;
uniform float time;

void main() {
    // Scroll del noise en el tiempo para que se mueva
    vec2 noiseUV = FragTexture + vec2(0.0, time * 0.05);
    vec2 distortion = texture(noiseTex, noiseUV).rg * 2.0 - 1.0; // [-1, 1]

    // Aplicar distorsión + scroll vertical a las UVs del humo
    vec2 smokeUV = FragTexture;
    smokeUV.y   -= time * 0.08;           // sube
    smokeUV     += distortion * 0.06;     // se distorsiona

    vec4 smoke = texture(billboardTex, smokeUV);

//    FragColor = smoke;
//    FragColor = texture(billboardTex, smokeUV);
    //    FragColor = vec4(vec3(0.85), smoke.a * fade);


    // Fade en bordes
    vec2 center = FragTexture - 0.5;
    float fade  = 1.0 - smoothstep(0.3, 0.5, length(center));

    FragColor = vec4(vec3(0.85), smoke.a * fade);
}