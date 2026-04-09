#version 330 core

in vec3 colorFragment;
in vec2 texFragment;
out vec4 out_color;

uniform sampler2D textura;

void main() {

	out_color = vec4(colorFragment, 1.0);
	// Goal is to use texture:
	//texture(textura,texFragment) * vec4(colorFragment,1.0);
}
