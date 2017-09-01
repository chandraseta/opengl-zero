#version 330 core

in vec3 custColor;
out vec4 color;

void main() {
	color = vec4(custColor, 1.0f);
}
