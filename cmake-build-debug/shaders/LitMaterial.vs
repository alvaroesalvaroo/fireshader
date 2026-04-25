#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 FragPosition;
out vec3 Normal;
out	vec2 FragTexture;


void main(){
	gl_Position = projection * view * model * vec4( aPos, 1 );

	FragPosition = vec3(model * vec4(aPos, 1.0));  // Pasar posicion en world space
	Normal = mat3(transpose(inverse(model))) * aNormal;                              // Pasar normals
	FragTexture = aUV;                             // Pasar UVs
}
