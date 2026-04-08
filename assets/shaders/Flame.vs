#version 330 core

layout (location = 0) in vec3 VertexPos0;
layout (location = 1) in vec3 VertexPos1;
layout (location = 2) in vec3 VertexPos2;
layout (location = 3) in vec2 texCoor;
layout (location = 4) in vec3 ColorVertex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform float time;

out	vec2 texFragment;
out	vec3 colorFragment;

void main(){

	gl_Position = projection * view * model * vec4( VertexPos0.xyz, 1 );
	texFragment = texCoor;
	colorFragment = vec3(1.0, 1.0, 1.0); //ColorVertex;
}

void main() {
    float factor = (sin(time) + 1.0) * 0.5; // Oscilación entre 0 y 1
    vec3 interpolatedPos = mix(VertexPos0, VertexPos1, factor);
    gl_Position = projection * view * model * vec4(interpolatedPos, 1.0);
}

