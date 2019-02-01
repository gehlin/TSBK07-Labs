#version 150

in vec4 colorv;
in vec2 texCoordv;
in float time;
out vec4 outColor;

void main(void)
{
	float a = sin(texCoordv.s*0.001*time);
	float b = sin(texCoordv.t*0.001*time);
	outColor = vec4(a, a, a, 0.0);
}
