#version 150

in  vec3 in_Position;
in vec4 color;

out vec4 colorv;

void main(void)
{
	colorv = color;
	gl_Position = vec4(in_Position, 1.0);
}
