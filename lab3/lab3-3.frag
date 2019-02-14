#version 150

in vec2 texCoordv;
uniform sampler2D sampler;
in float lightIntensity;
out vec4 outColor;

void main(void)
{
	outColor = lightIntensity*texture(sampler, texCoordv);
}
