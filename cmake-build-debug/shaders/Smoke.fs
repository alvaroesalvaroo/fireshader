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
const vec3 lightColor = vec3(1, 0.5, 0);
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
        if (dist > 0.2) continue; // Not enough influence
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
    float smokeColor = (smoke.r + 0.3 ) /2; // Media entre gris y el color de la textura

    // Light attenuation
    float att = calcAttenuationCum(); // [0, 1]

    vec3 finalColor = vec3(smokeColor) * (1 - att) + lightColor * att;
//    vec3 finalColor  = vec3(smokeColor) + dyeIntensity * lightColor  * calcAttenuationCum(); // 0.3 = intensidad del tinte

    // ====== FADE EN BORDES ======== //
    vec2 center = FragTexture - 0.5;
    float distortionForFade = distortion.x * 0.1 + 0.4; // [-1,1] → [0.2, 0.5]
    float fade  = 1.0 - smoothstep(0, distortionForFade, length(center)); //

    //FragColor = vec4(vec3(0.85), smoke.a * fade);
    FragColor = vec4(finalColor, smoke.r * fade);
//    FragColor = vec4(dyeIntensity * calcLightTint(), smoke.r * fade);
}