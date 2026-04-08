#version 330 core
out vec4 FragColor;


in vec3 FragPos;             // Posicion de los vertices
in vec3 Normal;              // Especificado en la data pasada de vertices
// in vec3 colorFragment; // para especificar color en cada vertice (descartado)

// uniform sampler2D textura; // not for now

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPosition;  // The camera position

float ambientStrength = 0.1;
float specularStrength = 0.8;

void main() {
    // ambient

	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDirection = normalize(viewPosition - FragPos);
    vec3 reflectDirection = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // add 3 values
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
//     FragColor = vec4(norm * 0.5 + 0.5, 1.0);
}
