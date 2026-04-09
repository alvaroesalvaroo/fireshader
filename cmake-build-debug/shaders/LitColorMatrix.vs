#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 Normal;
out vec3 FragPos;

//out	vec2 texFragment;   // Para texturas
//out	vec3 colorFragment; // Si especificamos data de color en cada vertice

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1 );
	FragPos = vec3(model * vec4(aPos, 1.0)); // Para evitar pasarle la posicion en local space, debe ser en world space
	Normal = aNormal;

	// será util para añaidr textures/colores
	// texFragment = texCoor;
	// colorFragment = ColorVertex;
}
