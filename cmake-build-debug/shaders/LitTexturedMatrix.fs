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
const float materialDiffuse = 0.8;
const float materialSpecular = 0.4;

// Point light structure
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;   // default: 0.1, 0.1, 0.1
    vec3 diffuse;   // default: 0.8, 0.8, 0.8
    vec3 specular;
};
uniform PointLight pointLight;

vec3 CalcPointLight(PointLight light, vec3 lightPos, vec3 lightColor, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 lightPos, vec3 lightColor, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    light.position = lightPos;

    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));

    // combine results
    vec3 ambient  = light.ambient  * lightColor * vec3(texture(textura, FragTexture));
    vec3 diffuse  = light.diffuse  * lightColor * diff * vec3(texture(textura, FragTexture));
    vec3 specular = light.specular * lightColor * spec * vec3(texture(textura, FragTexture));
    // ambient  *= attenuation; // Decided not to attenuate ambient
    ambient *= materialAmbient;
    diffuse  *= attenuation * materialDiffuse;
    specular *= attenuation * materialSpecular;
    return (ambient + diffuse + specular);
}

void main() {
    // Texture
	vec4 textureColor = texture(textura, FragTexture);
    // Two parameters we need befoore calculating light influence
	vec3 norm = normalize(Normal);
	vec3 viewDirection = normalize(viewPosition - FragPosition);

	// Light influence value
    vec3 result = CalcPointLight(pointLight, lightPosition, lightColor, norm, FragPosition, viewDirection);

    // add texture??
//     FragColor = vec4(result, 1.0);
FragColor = vec4(viewDirection, 1.0);
//     FragColor = vec4(1.0, 0.0, 0.0, 1.0); // debug rojo sólido
    // FragColor = vec4(result, 1.0) * textureColor;
}
