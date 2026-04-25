
#version 330 core
in  vec2  TexCoords;
out vec4  color;

uniform sampler2D scene;
uniform vec2      offsets[9];
uniform int       edge_kernel[9];
uniform float     blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

uniform sampler2D noiseTex;   // La textura de ruido (Perlin)
uniform bool      distort;    // El interruptor para este efecto
uniform float     time;       // El tiempo para el movimiento

const float distortionStrength = 0.006;
const float timeFactor = 0.014;
uniform float distortionInfluence = 0.1;
uniform vec2 worldOriginScreen;

const bool debugInfluenceArea = false;

void main()
{
    vec2 finalTexCoords = TexCoords;
    float distX = distance(TexCoords.x, worldOriginScreen.x);
    float minY = worldOriginScreen.y - 0.1;


    float maskX = smoothstep(distortionInfluence, distortionInfluence * 0.2, distX);
    float maskY = smoothstep(minY, minY + 0.05, TexCoords.y);
    float mask = maskX * maskY;
    // Mask es 1 en el centro, 0 fuera del radio
//    float mask = smoothstep(distortionInfluence, distortionInfluence * 0.8, dist);

    vec2 noiseCoords = TexCoords + vec2(0.0, time * timeFactor);

     vec2 distortion = texture(noiseTex, noiseCoords).rg;
     finalTexCoords.x += mask * (distortion.x * 2.0 - 1.0) * distortionStrength;
//    color = vec4(distortion, 0, 1);
//    return;
//    finalTexCoords.y = TexCoords.y;
    // Debug influence area:
//    bool shouldDistort = (distX < distortionInfluence) && (TexCoords.y > minY);
    if (debugInfluenceArea) {
        color = vec4(mask, mask, mask, 1);
        return;
    }

    // texSample from texture offsets if using convolution matrix

    color =  texture(scene, finalTexCoords);
    return;

    // process legacy (broken) effects
    vec3 texSample[9];
    if (chaos)
    {
        for(int i = 0; i < 9; i++)
            color += vec4(texSample[i] * edge_kernel[i], 0.0f);
        color.a = 1.0f;
    }
    else if (confuse)
    {
        color = vec4(1.0 - texture(scene, TexCoords).rgb, 1.0);
    }
    else if (shake)
    {
        for(int i = 0; i < 9; i++)
            color += vec4(texSample[i] * blur_kernel[i], 0.0f);
        color.a = 1.0f;
    }

}
