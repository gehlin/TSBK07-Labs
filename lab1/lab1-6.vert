#version 150

in vec3 in_Position;
in vec3 in_Normal;
out vec4 colorv;

void main(void)
{
	colorv = (vec4(1.0, 1.0, 1.0, 0.0) + vec4(in_Normal, 1.0)) * 0.5;
	gl_Position = vec4(in_Position, 1.0);
}
