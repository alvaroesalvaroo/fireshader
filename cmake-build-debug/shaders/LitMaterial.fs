#version 330 core

in vec3 FragPosition;
in vec3 Normal;
in vec2 FragTexture;
out vec4 FragColor;

// UNIFORMS
uniform sampler2D textura;  // Por el momento solo una textura

uniform vec3 viewPosition;  // The camera position


// uniform material
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

uniform Material material;
uniform Light light;

void main() {
    vec3 texColor  = vec3(texture(textura, FragTexture));
    vec3 norm      = normalize(Normal);
    vec3 lightDir  = normalize(light.position - FragPosition);
    vec3 viewDir   = normalize(viewPosition - FragPosition);
    vec3 reflDir   = reflect(-lightDir, norm);

    // Atenuación
    float dist        = length(light.position - FragPosition);
    float attenuation = 1.0 / (light.attConstant + light.attLinear * dist + light.attQuadratic * dist * dist);

    // Componentes
    vec3 ambient  = material.ambient  * light.color * texColor;
    vec3 diffuse  = material.diffuse  * max(dot(norm, lightDir), 0.0) * light.color * texColor * attenuation;
    vec3 specular = material.specular * pow(max(dot(viewDir, reflDir), 0.0), material.shininess) * light.color * attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
