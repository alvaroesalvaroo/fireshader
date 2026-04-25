#version 330 core

in vec3 FragPosition;
in vec3 Normal;
in vec2 FragTexture;
out vec4 FragColor;

// Structs
struct Material
{
    float shininess;
    float ambient;
    float diffuse;
    float specular;
};

struct Light
{
    vec3 color;
    vec3 position;
    float intensity;

    float attConstant;
    float attLinear;
    float attQuadratic;
};
#define MAX_LIGHTS 50

// UNIFORMS
uniform sampler2D textura;
uniform sampler2D normalMap;

uniform vec3 viewPosition;  // The camera position
uniform Material material;
uniform int lightCount;
uniform Light lights[MAX_LIGHTS];

vec3 calcDiffuseSpecular(Light light, vec3 norm, vec3 viewDir, vec3 texColor) {
    if (light.attConstant == 0) return vec3(0.0); // Non initialized light

    vec3 lightDir  = normalize(light.position - FragPosition);
    vec3 reflDir   = reflect(-lightDir, norm);

    float dist        = length(light.position - FragPosition);
    float attenuation = 1.0 / (light.attConstant + light.attLinear * dist + light.attQuadratic * dist * dist);
    vec3 lightEff     = light.color * light.intensity;

    vec3 diffuse  = material.diffuse  * max(dot(norm, lightDir), 0.0) * lightEff * texColor * attenuation;
    vec3 specular = material.specular * pow(max(dot(viewDir, reflDir), 0.0), material.shininess) * lightEff * attenuation;

    return diffuse + specular;
}

void main() {
    vec3 texColor  = vec3(texture(textura, FragTexture));

    //=========NORMALS========//
//     vec3 norm      = normalize(Normal); // No normalmap

    // textura normalMap
    vec3 norm = texture(normalMap, FragTexture).rgb;
    norm = normalize(norm * 2.0 - 1.0);

    // Rotar de tangent space a world space para plano XZ:
    norm = normalize(vec3(norm.x, norm.z, norm.y));
// ==============================//

    // Ambient: only once
    vec3 ambient  = material.ambient * texColor;

    // Cumulative diffuse and specular
    vec3 viewDir   = normalize(viewPosition - FragPosition);
    vec3 diffuseSpecularSum = vec3(0.0);
    for (int i = 0; i < lightCount; i++) {
        diffuseSpecularSum += calcDiffuseSpecular(lights[i], norm, viewDir, texColor);  // Sin ambient aquí
    }
//    FragColor = vec4(norm * 0.5 + 0.5, 1.0); // Debug normalMap

    FragColor = vec4(ambient + diffuseSpecularSum, 1.0);

}
