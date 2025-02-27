#version 410 core
out vec4 frag_colour;
uniform vec4 in_color;
void main() {
	frag_colour = in_color;
}