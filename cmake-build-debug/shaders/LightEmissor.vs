#version 330 core

layout (location = 0) in vec3 LVertexPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 lightColor;

void main(){
	gl_Position = projection * view * model * vec4( LVertexPos.xyz, 1 );
}
