// Flame Billboard.fs
#version 330 core

in vec2 FragTexture;
in vec3 FragPosition;
out vec4 FragColor;


uniform sampler2D billboardTex; // The fire texture
uniform sampler2D noiseTex;
uniform float time;
//uniform float brightness;
const float brightness = 2;

const float noiseSpeed = 1.87;
const float billboardSpeed = 1;
void main() {
    // Scroll del noise en el tiempo para que se mueva
    vec2 noiseUV = FragTexture;// + vec2(0.0, time * 0.3);
    vec2 distortion = texture(noiseTex, noiseUV).rg * 2.0 - 1.0; // [-1, 1]

    // Aplicar distorsión + scroll vertical a las UVs dela distorsion
    vec2 distortedUV = FragTexture;
    distortedUV.y   -= time * billboardSpeed;           // sube
    distortedUV     += distortion * 0.1;     // se distorsiona

    // vec4 flame = texture(billboardTex, distortedUV);
     vec4 flame = texture(billboardTex, distortedUV);

//    vec2 upUV = vec2(FragTexture.x, FragTexture.y - noiseSpeed * time);

//    distortedUV.y   += time * noiseSpeed / 2;           // corrige el movimiento del alpha

//    vec2 alphaUV = 1 - vec2(distortedUV.x, distortedUV.y);
    vec2 alphaUV = FragTexture;
    alphaUV.y   -= time * noiseSpeed;           // sube
    alphaUV.x += 0.5;
    float alpha = texture(noiseTex, alphaUV).r; // blanco = agujero, negro = llama

//    alpha *= brightness;

    // ====== FADE on the corners ======== //
    vec2 center = FragTexture - 0.5;
    float distortionForFade = distortion.y * 0.1 + 0.8; // [-1,1] → [0.2, 0.5]
    float fade  = 1.0 - smoothstep(0, distortionForFade, length(center)); //
    // Fade independiente por eje
    float fadeX = 1.0 - smoothstep(0.2, 0.5, abs(center.x));           // se corta en los lados
    float fadeY = 1.0 - smoothstep(0.1, 0.6, max(0.0, center.y));      // se desvanece por arriba
    float fadeBottom = 1.0 - smoothstep(0.3, 0.5, max(0.0, -center.y)); // se corta por abajo

    fade = fadeX * fadeY * fadeBottom;
//    FragColor = vec4(flame.rgb, alpha * flame.a); // No fade
    FragColor = vec4(flame.rgb, alpha * flame.a * fade); // Fade
//    FragColor = vec4(vec3(0, 0, fade), 1); // Debug

    //     flame *= fade;
}