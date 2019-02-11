#version 150

in vec4 color;
in float lightIntensity;

out vec4 outColor;

void main(void)
{;
	outColor = lightIntensity*vec4(1.0,1.0,1.0,0.0);
}
