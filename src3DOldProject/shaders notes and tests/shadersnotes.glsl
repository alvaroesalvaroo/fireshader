// Data types:
// int, unsigned int, float, double, bool and vectors:
/*
vecn: POR DEFECTO SON FLOATS
bvecn: a vector of n booleans.
ivecn: a vector of n integers.
uvecn: a vector of n unsigned integers.
dvecn: a vector of n double components.
*/

//Swizzling: una manera curiosa de acceder a componentes
// Constructor
//vec2 vector2d = vec2(0.5, 0.7);
//vec4 vector4d = vector2d.xyxy; // quiere decir algo como vector4d = vec4(vector2d.x, vector2d.y, vector2d.x, vector2d.y);

#version 330 core					// Necesario
layout (location = 0) in vec3 aPos; // Indicas que la variable "aPos" tiene el atributo posicion de 0. Opcional pero optimiza y recomendado

out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
}
