#version 330 core

in vec3 FragPosition;
in vec3 Normal;
in vec2 FragTexture;
out vec4 FragColor;

// UNIFORMS
uniform sampler2D textura;  // Por el momento solo una textura
uniform vec3 viewPosition;  // The camera position
uniform vec3 lightColor;
uniform vec3 lightPosition;

// Constants del material (hacer un material mas adelante)
const float materialShininess = 32;
const float materialAmbient = 0.1;
const float materialDiffuse = 0.4;
const float materialSpecular = 0.8;

const float attConstant  = 0.5;
const float attLinear    = 0.09;
const float attQuadratic = 0.032;



void main() {
    vec3 texColor  = vec3(texture(textura, FragTexture));
    vec3 norm      = normalize(Normal);
    vec3 lightDir  = normalize(lightPosition - FragPosition);
//     lightDir  = vec3(0.0, 1.0, 0.0);
    vec3 viewDir   = normalize(viewPosition - FragPosition);
    vec3 reflDir   = reflect(-lightDir, norm);
//     reflDir   = vec3(0.0, 1.0, 0.0);

    // Atenuación
    float dist        = length(lightPosition - FragPosition);
    float attenuation = 1.0 / (attConstant + attLinear * dist + attQuadratic * dist * dist);

    // Componentes
    vec3 ambient  = materialAmbient  * lightColor * texColor;
    vec3 diffuse  = materialDiffuse  * max(dot(norm, lightDir), 0.0) * lightColor * texColor * attenuation;
    vec3 specular = materialSpecular * pow(max(dot(viewDir, reflDir), 0.0), materialShininess) * lightColor * attenuation;
//     vec3 specular = materialSpecular * pow(max(dot(viewDir, norm), 0.0), materialShininess) * lightColor * attenuation;

    FragColor = vec4(ambient + diffuse + specular, 1.0);

//     FragColor = vec4(viewDir * 0.5 + 0.5, 1.0);
//     float spec = pow(max(dot(viewDir, reflDir), 0.0), 2.0); // en vez de 32
//     FragColor = vec4(vec3(spec), 1.0);
// FragColor = vec4(reflDir * 0.5 + 0.5, 1.0);

}
