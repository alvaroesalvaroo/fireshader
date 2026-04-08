// Shader file, written in GLSL

#version 330 core                     // Especificamos la versión de GLSL
layout (location = 0) in vec3 aPos;   // Declaramos una variable de entrada para las posiciones de los vértices

// Vamos a hacer que el vertexshader decida el color. Variable de salida
out vec4 vertexColor;


void main()
{
    gl_Position = vec4(aPos, 1.0);  // Transformamos el vértice de 3D a 4D. El último numero indica la escala
                                    // Optimiza muchos calculos de matrices posteriores
    vertexColor = vec4(0, 0, 1, 1); 

}
