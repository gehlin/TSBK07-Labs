#version 150

in vec3 in_Position;
in vec4 color;
out vec4 colorv;
uniform mat4 myMatrix;

void main(void)
{
	colorv = color;
	gl_Position = myMatrix * vec4(in_Position, 1.0);
}
