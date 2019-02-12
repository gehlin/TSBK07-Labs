#version 150

in vec2 texCoordv;
in vec4 color;
in float lightIntensity;
uniform sampler2D in_texUnit;

out vec4 outColor;

void main(void)
{;
	outColor = lightIntensity*texture(in_texUnit, texCoordv);
}
