# Fire Shader

En esta práctica, experimentaremos con el uso de diferentes shaders para crear una escena 3D con un fuego.

El resultado final incluye partículas (_point lights_), humo, llamas y un efecto de post-procesado que simula la distorsión por efecto del calor.

![Fire demo scene](./media/FireDemo.mp4)

## Clases y organización del código

Para evitar el desastre de intentos anteriores con OpenGL, esta vez decidí adoptar una estructura de clases basada en el tutorial de [Breakout por Joey de Vrie](https://learnopengl.com/In-Practice/2D-Game/Breakout), aunque adaptada al entorno 3D. También decidí centralizar la mayoría de la lógica del _setup_ en la clase **FireScene** 

Las siguientes clases son en su gran mayoría recicladas:

- **Shader**: clase que permite crear y compilar los shaders, mostrando errores de compilación si existen. También permite asignar _uniforms_ por nombre.
- **Texture:** permite dar el formato correcto a las texturas, además de _bindearlas_ al bus adecuado de datos.
- **Resource Manager:** permite cargar desde la memoria los Shaders y Texturas. Centraliza la liberación de recursos.

Por otro lado, decidí añadir las siguientes clases:

- **Mesh**: prepara los arrays de datos VAO, VBO y EBO al crear la malla. Tiene funciones que permiten crear mallas básicas como `generateTrapezium`, `generatePlane`, `generateCubeWithNormals`, ... También tiene un loader de mallas en formato .obj que creé yo mismo.
Además, su método `draw()` llama `glBindVertexArray` y a `glDrawArrays` o `glDrawElements` según corresponda. 
- **Material**: modelo simple de material (ambient, diffuse, specular y shininess). Hemos decidido que no gestione la textura, simplemente actualiza estos parámetros en el shader pertinente.
- **Object3D**: gestiona la posición, rotación y escala del objeto Tiene posibles referencias a una **malla**, un **material**, un **shader** y hasta **dos texturas** (no necesité más de 2 texturas en ningún objeto, aunque la generalización hubiera sido trivial).
También guarda las uniforms id de Model, Projection y View para actualizarlas de manera optimizada en el `render()`.

## Shaders

Para conseguir el efecto final de fuego, hemos usado los siguientes shaders:
- **NormalMapMultilit**: shader del suelo. Recibe iluminación de diversas _point lights_. La iluminación está basada en el modelo ambient-diffuse-specular. Para los normalmaps, se hace uso de un pequeño truco para no tener que calcular la matriz TBN (tangent, bitangent, normal): se asume que la mayoría de polígonos están en el plano XZ, y se colocan los vectores de acuerdo a esta posición `norm = normalize(vec3(norm.x, norm.z, norm.y))`
- **Smoke**
- **LightEmissor**
- **Fire**
- **Postprocess**


## Dependencies
When making the build, three `.dll` files should be placed in the same directory as the executable: Freetype, SDL and SDL_mixer. For Windows OS, these can be found in:
- **Freetype**: [freetype/lib/win64(32)/freetype.dll](freetype/lib)
- **SDL2**: [SDL/SDL2-2.32.0/lib/x64(x86)/SDL2.dll](SDL/SDL2-2.32.0/lib)
- **SDL2_mixer**: [SDL/SDL2_mixer-2.8.1/lib/x64(x86)/SDL2_mixer.dll](SDL/SDL2_mixer-2.8.1/lib)

## Build 
Windows-x64 release build is in [build/Release-x64](build/Release-x64/) directory. I packaged a one-file build using [Enigma Virtual Box](https://enigmaprotector.com/en/aboutvb.html) software, which can be found in [build/](build/) directory.

## Assets
All assets included are from the amazing [LearnOpenGL by Joey de Vries](https://learnopengl.com) website, except for the music which was produced by me. It is worth mentioning that it uses samples from [Cab Calloway And His Orchestra - At The Clambake Carnival (1938)](https://www.youtube.com/watch?v=IaPFlQwf6hU&list=RDIaPFlQwf6hU&start_radio=1)