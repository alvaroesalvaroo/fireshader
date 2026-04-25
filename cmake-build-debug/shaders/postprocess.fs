
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

uniform sampler2D noiseMap;   // La textura de ruido (Perlin)
uniform bool      distort;    // El interruptor para este efecto
uniform float     time;       // El tiempo para el movimiento

void main()
{
    vec2 finalTexCoords = TexCoords;
    if (distort) {
        vec2 noiseCoords = TexCoords + vec2(time * 0.1, 0.0);

        // 2. Obtener el valor de distorsión (usamos el canal R y G)
        // texture(noiseMap, noiseCoords).rg nos devuelve valores [0.0, 1.0]
        vec2 distortion = texture(noiseMap, noiseCoords).rg;

        // 3. Convertir a rango [-1.0, 1.0] y aplicar una fuerza pequeña (0.005)
        // Si es muy alta, la imagen se romperá demasiado
        finalTexCoords += (distortion * 2.0 - 1.0) * 0.005;
    }

    color = vec4(0.0f);
    vec3 texSample[9];
    // texSample from texture offsets if using convolution matrix
    if(chaos || shake)
        for(int i = 0; i < 9; i++)
            texSample[i] = vec3(texture(scene, TexCoords.st + offsets[i]));

    color =  texture(scene, finalTexCoords);

    // process legacy (broken) effects
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
