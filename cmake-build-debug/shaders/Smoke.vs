// Billboard.vs
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 FragTexture;
out vec3 FragPosition;


void main() {
    vec3 worldPos = vec3(model[3]);
//    vec3 worldPos = vec3(0);

    float scaleX = length(vec3(model[0]));
    float scaleY = length(vec3(model[1]));

    vec3 camRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 camUp    = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 vertexWorld = worldPos
    + camRight * aPos.x * scaleX
    + camUp    * aPos.y * scaleY;

    gl_Position = projection * view * vec4(vertexWorld, 1.0);

    FragTexture = aUV;
    FragPosition = vertexWorld;
}