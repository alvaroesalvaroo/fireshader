// Shader file, written in GLSL

#version 330 core               // Especificamos la versión de GLSL
out vec4 FragColor;             // Variable de salida, representando el color en RGBA [0-1]
in vec4 vertexColor;            // Variable de entrada

// Podriamos usar una uniform. Una uniform es comun en todos los shaders y puede ser accedida y modificada por cualquiera
uniform vec4 uniformColor = vec4(1, 0, 0, 1);
// Se modifica desde el codigo usando 
// int uniformLocation = glGetUniformLocation(shaderProgram, "nombredelavariableuniforme");
// glUseProgram(shaderProgram);
// glUniform4f(uniformLocation, valor1, valor2, valor3, ...); // Tantos como dimension del vector

void main()
{
    //FragColor = vertexColor; // en funcion de la posicion
    FragColor = uniformColor;   // en el codigo principal (en funcion del tiempo)
}
