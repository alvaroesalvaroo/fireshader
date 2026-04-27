// Billboard.fs
#version 330 core

in vec2 FragTexture;
in vec3 FragPosition;
out vec4 FragColor;


uniform sampler2D billboardTex;
uniform sampler2D noiseTex;
uniform float time;

#define MAX_LIGHTS 50

// Very strong attenuation
const float attConstant = 0.8;
const float attLinear = 20.0;
const float attQuadratic = 800.0;
const vec3 lightColor = vec3(1, 0.7, 0);
// const float dyeIntensity = 0.3;

const float smokeInfluence = 0.00f;

// UNIFORMS
uniform int lightCount;
uniform vec3 lightPositions[MAX_LIGHTS];

// modelo simplificado de luces
float calcAttenuationCum() {
    vec3 tint = vec3(0.0);
    float attenuationCum;
    for (int i = 0; i < lightCount; i++) {
        float dist        = length(lightPositions[i] - FragPosition);
        dist -= smokeInfluence;
        if (dist < 0) {
           dist = 0;
        }

        float attenuation = 1.0 / (attConstant
        + attLinear * dist
        + attQuadratic * dist * dist);
        attenuationCum +=  attenuation;
    }
    return attenuationCum;
}

void main() {
    // Scroll del noise en el tiempo para que se mueva
    vec2 noiseUV = FragTexture + vec2(0.0, time * 0.3);
    vec2 distortion = texture(noiseTex, noiseUV).rg * 2.0 - 1.0; // [-1, 1]

    // Aplicar distorsión + scroll vertical a las UVs del humo
    vec2 smokeUV = FragTexture;
    smokeUV.y   -= time * 0.08;           // sube
    smokeUV     += distortion * 0.06;     // se distorsiona

    vec4 smoke = texture(billboardTex, smokeUV);

//    FragColor = smoke;
//    FragColor = texture(billboardTex, smokeUV);
    //    FragColor = vec4(vec3(0.85), smoke.a * fade);


    // ====== FADE ======== //
    vec2 center = FragTexture - 0.5;
    float distortionForFade = distortion.x * 0.1 + 0.4; // [-1,1] → [0.2, 0.5]
    float fade  = 1.0 - smoothstep(0, distortionForFade, length(center)); //

    // Fade lateral (X) — simétrico
//    float fadeSides = 1.0 - smoothstep(0.2, 0.5, abs(center.x));
//
//    // Fade inferior más corto, superior más suave
//    float fadeBottom = 1.0 - smoothstep(0.1, 0.4, max(0.0, -center.y)); // aplica cundo estamos en la mitad inferior
//
//    float distortionForFade = distortion.x * 0.1 + 0.4; // [-1,1] → [0.2, 0.5]
//    float fadeTop    = 1.0 - smoothstep(0.1, distortionForFade, max(0.0,  center.y)); // aplica cundo estamos en la mitad superior
//    float fade = fadeSides * fadeBottom * fadeTop;

//    fade *= (distortion.x + 1) / 2;

    //    FragColor = smoke;
    float smokeColor = (smoke.r + 0.3 ) /2; // Media entre gris y el color de la textura

    float att = calcAttenuationCum(); // [0, 1]
//    att = att / 2 + att * noiseUV.x / 2;
//    att = (distortion.x + 1)/2;
//    att -= (distortion.x + 1 ) / 2 * 0.2; // [0, 1] - [0, 0.2]

//    att = clamp(att, 0, 0.8);


    vec3 finalColor = vec3(smokeColor) * (1 - att) + lightColor * att;
//    vec3 finalColor  = vec3(smokeColor) + dyeIntensity * lightColor  * calcAttenuationCum(); // 0.3 = intensidad del tinte

    //FragColor = vec4(vec3(0.85), smoke.a * fade);
    FragColor = vec4(finalColor, smoke.r * fade);
//    FragColor = vec4(dyeIntensity * calcLightTint(), smoke.r * fade);
}